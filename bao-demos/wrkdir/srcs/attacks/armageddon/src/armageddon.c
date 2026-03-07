#include "../inc/armageddon.h"
#include "../../../VMs/vm_0/src/tasks/inc/globals.h"

#include <stdio.h>

//area de monitoramento compartilhada para o ataque flush+reload
uint8_t shared_probe_data[4096] __attribute__((aligned(4096))); 
volatile uint8_t *target_addr = &shared_probe_data[2048];

uint64_t calibrate_threshold() {
    uint64_t hit_time = 0, miss_time = 0;
    int samples = 1000;

    for(int i=0; i<samples; i++) {
        volatile uint8_t reload = *target_addr;
        asm volatile("" : : "r"(reload) : "memory"); //evita otimizacao
        hit_time += arm_measure_access((void*)target_addr);
    }
    hit_time /= samples;

    for(int i=0; i<samples; i++) {
        arm_flush((void*)target_addr);
        miss_time += arm_measure_access((void*)target_addr);
    }
    miss_time /= samples;

    uint64_t threshold = (hit_time + miss_time) / 2;
    //printf("[ARMAGEDDON] Calibração: Hit=%lu, Miss=%lu, Threshold=%lu\n", hit_time, miss_time, threshold);
    return threshold;
}

void simulate_victim_access() {
    volatile uint8_t dummy = *target_addr;
    asm volatile("" : : "r"(dummy) : "memory"); //forca a leitura real
}

void execute_flush_reload_attack(uint64_t threshold, uint32_t duration_ms) {
    uint64_t start_time = get_hardware_timer_count();
    uint64_t timer_freq = get_hardware_timer_freq();
    
    uint64_t duration_hardware_ticks = (timer_freq / 1000) * duration_ms;
    uint64_t end_time = start_time + duration_hardware_ticks;

    int hits = 0;
    int probes = 0;

    while(get_hardware_timer_count() < end_time) {
        arm_flush((void*)target_addr);
        
        // 30% de chance da vitma acessar o endereco alvo
        if ((probes % 3) == 0) { 
            simulate_victim_access();
        }

        uint64_t time = arm_measure_access((void*)target_addr);
        probes++;

        if (time < threshold) {
            hits++;
        }
    }

    // printf("[ARMAGEDDON] Stats: %d Hits em %d Probes (Ratio: %.2f)\n", hits, probes, (float)hits/probes);
}