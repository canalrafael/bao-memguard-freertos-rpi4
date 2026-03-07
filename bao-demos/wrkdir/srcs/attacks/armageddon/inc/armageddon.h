#ifndef ARMAGEDDON_H
#define ARMAGEDDON_H

#include <stdint.h>

extern uint8_t shared_probe_data[4096]; 
extern volatile uint8_t *target_addr;

//primitiva de flush
__attribute__((always_inline)) static inline void arm_flush(void *addr) {
    asm volatile("dc civac, %0" : : "r"(addr) : "memory");
    asm volatile("dsb sy \n isb"); //garante que o flush terminou
}

//primitiva de medição de tempo usando o contador de ciclos virtual (cntvct_el0)
__attribute__((always_inline)) static inline uint64_t arm_measure_access(void *addr) {
    uint64_t start, end;
    asm volatile("isb \n mrs %0, cntvct_el0" : "=r"(start));
    
    volatile uint8_t junk = *(volatile uint8_t *)addr;
    
    //o dsb sy forca a CPU a esperar o dado ser carregado da memoria
    asm volatile("dsb sy \n isb \n mrs %0, cntvct_el0" : "=r"(end) : : "memory");
    
    //evita otimizacao do compilador
    asm volatile("" : : "r"(junk) : "memory");
    
    return end - start;
}

uint64_t calibrate_threshold();
void simulate_victim_access();
void execute_flush_reload_attack(uint64_t threshold, uint32_t duration_ms);
void task_flush_reload(void *arg);


#endif