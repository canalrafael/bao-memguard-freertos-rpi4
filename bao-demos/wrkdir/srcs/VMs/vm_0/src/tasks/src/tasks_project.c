#include <stdio.h>
#include <stdlib.h>
#include "FreeRTOS.h"
#include "tasks_project.h"
#include "queue.h"

#include "../inc/globals.h"

#include "pmu_monitor.h"
#include "neural_network.h"




//==============================================================================
//monitor
//==============================================================================

void task_monitor(void *arg) {
    const TickType_t xPeriod = pdMS_TO_TICKS(200);
    TickType_t xLastWakeTime = xTaskGetTickCount();
    
    printf("TIMESTAMP, CPU_CYCLES, INSTRUCTIONS, CACHE_MISSES, BRANCH_MISSES, LABEL\n");
    fflush(stdout);

    uint64_t timer_freq;
    asm volatile("mrs %0, cntfrq_el0" : "=r"(timer_freq));

    init_pmu_registers();

    while(1) {
        vTaskDelayUntil(&xLastWakeTime, xPeriod);

        collect_and_process_pmu_sample(timer_freq);
    }
}

//==============================================================================
//fann
//==============================================================================

void task_fann(void *arg) {
    struct fann *ann = init_fann_model();
    int iteracao = 0;
    
    vTaskDelay(pdMS_TO_TICKS(500));
    const TickType_t xPeriod = pdMS_TO_TICKS(500);
    TickType_t xLastWakeTime = xTaskGetTickCount();
    
    while(1) {
        vTaskDelayUntil(&xLastWakeTime, xPeriod);
      
        int amostras = uxQueueMessagesWaiting(xPmuQueue);
      
        if (amostras > 0) {
            process_fann_batch(ann, amostras, &iteracao);
        }
    }
}
