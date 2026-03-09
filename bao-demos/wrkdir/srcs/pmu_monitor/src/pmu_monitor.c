#include "../inc/pmu_monitor.h"
#include "../../VMs/vm_0/src/tasks/inc/globals.h"
#include <stdio.h>


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

void bao_get_pmu_data(PMU_data *data) {
  
    const unsigned long full_id = SMCC64_FID_VND_HYP_SRVC | HC_SEC_MONITOR;
  
    register unsigned long r0 asm("x0") = full_id;
    register unsigned long r2 asm("x2");
    register unsigned long r1 asm("x1");
    register unsigned long r3 asm("x3");
    register unsigned long r4 asm("x4");
  
  // Executa a Hypercall e diz ao compilador que x0-x3 terão as saídas
  asm volatile(
    "hvc #0"
    : "+r"(r0), "=r"(r1), "=r"(r2), "=r"(r3), "=r"(r4) // Saídas
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
    FANN_sample sample;

    bao_get_pmu_data(&sample.data);
    
    sample.output = g_label_atual;
    
    xQueueSend(xPmuQueue, &sample, 0);

    uint64_t boot_seconds = sample.data.timestamp / timer_freq;
    uint64_t ticks_remainder = sample.data.timestamp % timer_freq;

    #ifndef BUILD_EPOCH
      #define BUILD_EPOCH 0
    #endif

    uint64_t real_time_seconds = BUILD_EPOCH + boot_seconds - 10800;

    uint32_t milliseconds = (ticks_remainder * 1000) / timer_freq;
    uint32_t hours = (real_time_seconds % 86400) / 3600;
    uint32_t minutes = (real_time_seconds % 3600) / 60;
    uint32_t seconds = real_time_seconds % 60;

    printf("%02u:%02u:%02u:%03u %lu %lu %lu %lu %.1f\n",
            hours, minutes, seconds, milliseconds,
            sample.data.cpu_cycles,
            sample.data.instructions,
            sample.data.cache_misses,
            sample.data.branch_misses,
            sample.output);
}

void init_pmu_registers(void) {
    //configura os mesmos eventos que o Hypervisor espera ler
    #define PMU_EVT_INST_RETIRED     0x08
    #define PMU_EVT_L1D_CACHE_REFILL 0x03
    #define PMU_EVT_BR_MIS_PRED      0x10

    //contador 0
    asm volatile("msr pmselr_el0, %0" :: "r" (0));
    asm volatile("isb");
    asm volatile("msr pmxevtyper_el0, %0" :: "r" (PMU_EVT_BR_MIS_PRED));

    //contador 1
    asm volatile("msr pmselr_el0, %0" :: "r" (1));
    asm volatile("isb");
    asm volatile("msr pmxevtyper_el0, %0" :: "r" (PMU_EVT_L1D_CACHE_REFILL));

    //contador 3
    asm volatile("msr pmselr_el0, %0" :: "r" (3));
    asm volatile("isb");
    asm volatile("msr pmxevtyper_el0, %0" :: "r" (PMU_EVT_INST_RETIRED));

    //remove filtros de excecao
    asm volatile("msr pmccfiltr_el0, %0" :: "r" (0));

    //habilita os contadores especificos
    asm volatile("msr pmcntenset_el0, %0" : : "r" (0x8000000B));

    asm volatile("msr pmcr_el0, %0" : : "r" (0x07));
}