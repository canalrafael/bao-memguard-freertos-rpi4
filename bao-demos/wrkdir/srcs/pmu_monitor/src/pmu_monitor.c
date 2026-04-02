#include "../inc/pmu_monitor.h"
#include "../../VMs/vm_0/src/tasks/inc/globals.h"
#include <stdio.h>
#include <string.h>

#define MAX_SAMPLES 300 // ~20s de dados (3 cores * 5 amostras/s * 20s)

static FANN_sample pmu_history[MAX_SAMPLES];
static uint32_t current_sample_index = 0;

//endereco base do IPC
#define IPC_BASE_ADDR 0x70000000 

typedef struct {
    volatile uint32_t dump_request;
} IPC_Payload;

void dump_history_to_serial(void) {
    if (current_sample_index == 0) {
        return;
    }

    printf("==================================================\n");
    printf("START_OF_CSV_DATA\n");
    printf("CORE_ID,TIMESTAMP,CPU_CYCLES,INSTRUCTIONS,CACHE_MISSES,BRANCH_MISSES,LABEL\n");

    for (uint32_t i = 0; i < current_sample_index; i++) {
        printf("%lu,%lu,%lu,%lu,%lu,%lu,%f\n",
            pmu_history[i].core_id,
            pmu_history[i].data.timestamp,
            pmu_history[i].data.cpu_cycles,
            pmu_history[i].data.instructions,
            pmu_history[i].data.cache_misses,
            pmu_history[i].data.branch_misses,
            pmu_history[i].output);
    }

    printf("END_OF_CSV_DATA\n");
    printf("==================================================\n");

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
    IPC_Payload* shared_memory = (IPC_Payload*) IPC_BASE_ADDR;

    if (shared_memory->dump_request == 1) {
        dump_history_to_serial(); 
        
        asm volatile("dsb sy" ::: "memory");
        shared_memory->dump_request = 0;
        
        return;
    }

    if (current_sample_index < MAX_SAMPLES) {
        for (uint8_t core = 1; core <= 3; core++) {
            if (current_sample_index >= MAX_SAMPLES) break;

            FANN_sample sample;
            sample.core_id = core;
            
            bao_get_pmu_data(core, &sample.data);
            
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