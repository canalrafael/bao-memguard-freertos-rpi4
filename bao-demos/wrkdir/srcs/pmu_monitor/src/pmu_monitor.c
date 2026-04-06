#include "../inc/pmu_monitor.h"
#include "../../VMs/vm_0/src/tasks/inc/globals.h"
#include <stdio.h>
#include <string.h>

#define MAX_SAMPLES 512

static FANN_sample pmu_history[MAX_SAMPLES];
static uint32_t current_sample_index = 0;

//==============================================================================
// IPC — 3 canais isolados (VM0 acessa todos)
//==============================================================================
#define IPC_VM1_ADDR 0x70000000   // Canal 1: VM0 <-> VM1
#define IPC_VM2_ADDR 0x70010000   // Canal 2: VM0 <-> VM2
#define IPC_VM3_ADDR 0x70020000   // Canal 3: VM0 <-> VM3

typedef struct {
    volatile uint32_t signal_ready;   // VMx -> VM0: "pause feito"
    volatile uint32_t resume;         // VM0 -> VMx: "pode continuar"
    volatile uint32_t current_label;  // VMx -> VM0: label do ataque/benchmark
} IPC_Channel;

// Labels armazenados localmente pela VM0
static uint32_t label_vm1 = 0;
static uint32_t label_vm2 = 0;
static uint32_t label_vm3 = 0;

// Labels válidos são números pequenos (IDs de ataques/benchmarks).
// Valores grandes são lixo de memória não-inicializada.
#define MAX_VALID_LABEL 100

// Helpers de cache para manter coerência com o Linux que usa Uncached Memory
static inline void cache_clean_invalidate(volatile void* addr) {
    asm volatile("dc civac, %0" : : "r" (addr) : "memory");
    asm volatile("dsb sy" ::: "memory");
}

// Inicializa as regiões IPC com zeros (chamada uma vez na startup)
void ipc_init_channels(void) {
    IPC_Channel* ch_vm1 = (IPC_Channel*) IPC_VM1_ADDR;
    IPC_Channel* ch_vm2 = (IPC_Channel*) IPC_VM2_ADDR;
    IPC_Channel* ch_vm3 = (IPC_Channel*) IPC_VM3_ADDR;

    ch_vm1->signal_ready = 0;
    ch_vm1->resume = 0;
    ch_vm1->current_label = 0;

    ch_vm2->signal_ready = 0;
    ch_vm2->resume = 0;
    ch_vm2->current_label = 0;

    ch_vm3->signal_ready = 0;
    ch_vm3->resume = 0;
    ch_vm3->current_label = 0;

    cache_clean_invalidate((void*)ch_vm1);
    cache_clean_invalidate((void*)ch_vm2);
    cache_clean_invalidate((void*)ch_vm3);
}

void dump_history_to_serial(void) {
    if (current_sample_index == 0) {
        return;
    }

    // Ler frequencia do timer para converter ticks em tempo
    uint64_t timer_freq;
    asm volatile("mrs %0, cntfrq_el0" : "=r"(timer_freq));

    // BUILD_EPOCH é injetado pelo Makefile via -DBUILD_EPOCH=$(date +%s)
    // Representa o Unix timestamp do momento da compilação
    // Usamos como offset para calcular o horário real
    #ifndef BUILD_EPOCH
    #define BUILD_EPOCH 0
    #endif
    const uint64_t build_epoch_secs = (uint64_t)BUILD_EPOCH;

    printf("==================================================\n");
    printf("START_OF_CSV_DATA\n");
    printf("CORE_ID,TIMESTAMP,CPU_CYCLES,INSTRUCTIONS,CACHE_MISSES,BRANCH_MISSES,LABEL\n");

    for (uint32_t i = 0; i < current_sample_index; i++) {
        // Converter timestamp (ticks) para tempo real
        uint64_t ticks = pmu_history[i].data.timestamp;
        uint64_t elapsed_ms = (ticks * 1000) / timer_freq;
        uint64_t elapsed_secs = elapsed_ms / 1000;
        uint32_t ms = (uint32_t)(elapsed_ms % 1000);

        // Somar tempo de boot ao epoch da compilação para obter horário real
        uint64_t total_secs = build_epoch_secs + elapsed_secs;

        // Extrair HH:MM:SS do horário do dia (mod 24h)
        // Ajuste de fuso horário: -3h para BRT (Brasília)
        uint64_t day_secs = (total_secs - 3 * 3600) % 86400;
        uint32_t hh = (uint32_t)(day_secs / 3600);
        uint32_t mm = (uint32_t)((day_secs % 3600) / 60);
        uint32_t ss = (uint32_t)(day_secs % 60);

        printf("%lu,%02lu:%02lu:%02lu:%03lu,%lu,%lu,%lu,%lu,%lu\n",
            pmu_history[i].core_id,
            (unsigned long)hh, (unsigned long)mm,
            (unsigned long)ss, (unsigned long)ms,
            pmu_history[i].data.cpu_cycles,
            pmu_history[i].data.instructions,
            pmu_history[i].data.cache_misses,
            pmu_history[i].data.branch_misses,
            (unsigned long)pmu_history[i].label);
    }

    printf("END_OF_CSV_DATA\n");
    printf("==================================================\n");
    fflush(stdout);

    current_sample_index = 0;
}

unsigned long hypercall(unsigned long hc_id, unsigned long arg) {
    const unsigned long full_id = SMCC64_FID_VND_HYP_SRVC | hc_id;

    register unsigned long r0 asm("x0") = full_id;
    register unsigned long r1 asm("x1") = arg;
  
    asm volatile(
        "hvc #0"
        : "+r"(r0)
        : "r"(r1)
        : "memory", "x2", "x3"
    );
    return r0;
}

void bao_get_pmu_data(uint8_t target_cpu, PMU_data *data) {
    const unsigned long full_id = SMCC64_FID_VND_HYP_SRVC | HC_SEC_MONITOR;
  
    register unsigned long r0 asm("x0") = full_id;
    register unsigned long r1 asm("x1") = target_cpu; 
    register unsigned long r2 asm("x2");
    register unsigned long r3 asm("x3");
    register unsigned long r4 asm("x4");
  
    asm volatile(
        "hvc #0"
        : "+r"(r0), "+r"(r1), "=r"(r2), "=r"(r3), "=r"(r4) 
        : 
        : "memory"
    ); 
      
    data->cpu_cycles = r0;
    data->cache_misses = r1;
    data->instructions = r2;
    data->branch_misses = r3;
    data->timestamp = r4;
}

void collect_and_process_pmu_sample(uint64_t timer_freq) {
    IPC_Channel* ch_vm1 = (IPC_Channel*) IPC_VM1_ADDR;
    IPC_Channel* ch_vm2 = (IPC_Channel*) IPC_VM2_ADDR;
    IPC_Channel* ch_vm3 = (IPC_Channel*) IPC_VM3_ADDR;

    // Forçar a leitura descarregando/invalidando a linha de cache para o endereço de struct
    // (cada VM tem 64KB de canal, a estrutua cabe numa linha de 64-bytes tranquilo)
    cache_clean_invalidate((void*)ch_vm1);
    cache_clean_invalidate((void*)ch_vm2);
    cache_clean_invalidate((void*)ch_vm3);

    // Ler labels atuais dos canais IPC
    // Só atualiza se o valor for válido (não é lixo de memória)
    uint32_t tmp;
    tmp = ch_vm1->current_label;
    if (tmp <= MAX_VALID_LABEL) label_vm1 = tmp;
    tmp = ch_vm2->current_label;
    if (tmp <= MAX_VALID_LABEL) label_vm2 = tmp;
    tmp = ch_vm3->current_label;
    if (tmp <= MAX_VALID_LABEL) label_vm3 = tmp;

    // Verificar barreira: todas as 3 VMs sinalizaram?
    if (ch_vm1->signal_ready == 1 &&
        ch_vm2->signal_ready == 1 &&
        ch_vm3->signal_ready == 1) {

        // Fazer print dos dados coletados
        dump_history_to_serial();

        // Resetar sinais
        ch_vm1->signal_ready = 0;
        ch_vm2->signal_ready = 0;
        ch_vm3->signal_ready = 0;

        // Sinalizar resume para todas
        ch_vm1->resume = 1;
        ch_vm2->resume = 1;
        ch_vm3->resume = 1;
        
        // Forçar a escrita em RAM para que o Linux (VM3) uncached enxergue instantaneamente
        cache_clean_invalidate((void*)ch_vm1);
        cache_clean_invalidate((void*)ch_vm2);
        cache_clean_invalidate((void*)ch_vm3);

        return;
    }

    // Coleta normal de PMU — armazenar em historico
    if (current_sample_index < MAX_SAMPLES) {
        // Mapeamento core físico → VM (conforme cpu_affinity no rpi4.c):
        //   Core 0 físico = VM0 (cpu_affinity=0b1)   → monitor (não coleta)
        //   Core 1 físico = VM1 (cpu_affinity=0b10)  → ataques
        //   Core 2 físico = VM2 (cpu_affinity=0b100) → benchmarks
        //   Core 3 físico = VM3 (cpu_affinity=0b1000)→ Linux (benchmarks/ataques)
        for (uint8_t core = 1; core <= 3; core++) {
            if (current_sample_index >= MAX_SAMPLES) break;

            FANN_sample sample;
            sample.core_id = core;
            
            bao_get_pmu_data(core, &sample.data);
            
            // Atribuir label correto de acordo com o core físico
            if (core == 1) {
                sample.label = label_vm1;  // VM1 = ataques
            } else if (core == 2) {
                sample.label = label_vm2;  // VM2 = benchmarks
            } else {
                sample.label = label_vm3;  // VM3 = Linux
            }
            
            sample.output = g_label_atual;
            
            pmu_history[current_sample_index] = sample;
            current_sample_index++;
            
            xQueueSend(xPmuQueue, &sample, 0);
        }
    }
}

void init_pmu_registers(void) {
    #define PMU_EVT_INST_RETIRED     0x08
    #define PMU_EVT_L1D_CACHE_REFILL 0x03
    #define PMU_EVT_BR_MIS_PRED      0x10

    asm volatile("msr pmselr_el0, %0" :: "r" (0));
    asm volatile("isb");
    asm volatile("msr pmxevtyper_el0, %0" :: "r" (PMU_EVT_BR_MIS_PRED));

    asm volatile("msr pmselr_el0, %0" :: "r" (1));
    asm volatile("isb");
    asm volatile("msr pmxevtyper_el0, %0" :: "r" (PMU_EVT_L1D_CACHE_REFILL));

    asm volatile("msr pmselr_el0, %0" :: "r" (3));
    asm volatile("isb");
    asm volatile("msr pmxevtyper_el0, %0" :: "r" (PMU_EVT_INST_RETIRED));

    asm volatile("msr pmccfiltr_el0, %0" :: "r" (0));
    asm volatile("msr pmcntenset_el0, %0" : : "r" (0x8000000B));
    asm volatile("msr pmcr_el0, %0" : : "r" (0x07));
}