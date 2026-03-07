#include <stdio.h>
#include "FreeRTOS.h"
#include "tasks_project.h"
#include "queue.h"

#include "../inc/globals.h"

#include "pmu_monitor.h"
#include "neural_network.h"
#include "benchmarks_wrappers.h"

#include "spectre_attack.h"
#include "meltdown.h"
#include "armageddon.h"
#include "zombieload.h"



//==============================================================================
//monitor
//==============================================================================

void task_monitor(void *arg) {
    const TickType_t xPeriod = pdMS_TO_TICKS(250);
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
    
    vTaskDelay(pdMS_TO_TICKS(1000));
    const TickType_t xPeriod = pdMS_TO_TICKS(2000);
    TickType_t xLastWakeTime = xTaskGetTickCount();
    
    while(1) {
        vTaskDelayUntil(&xLastWakeTime, xPeriod);
      
        int amostras = uxQueueMessagesWaiting(xPmuQueue);
      
        if (amostras > 0) {
            process_fann_batch(ann, amostras, &iteracao);
        }
    }
}

//==============================================================================
//ataques
//==============================================================================

void task_spectre(void *arg) {
    const TickType_t xPeriod = pdMS_TO_TICKS(1000); 
    TickType_t xLastWakeTime = xTaskGetTickCount();
    
    //configuracoes do ataque
    int cache_hit_threshold = 20;
    int bytes_to_read = 10;

    printf("[SPECTRE] Task Iniciada...\n");

    while(1) {
        vTaskDelayUntil(&xLastWakeTime, xPeriod);

        size_t malicious_x = (size_t)(secret - (char *)array1);

        g_label_atual = 1.0f; 
        
        execute_spectre_attack(cache_hit_threshold, malicious_x, bytes_to_read);

        g_label_atual = 0.0f; 
    }
}

void task_meltdown(void *arg) {
    const TickType_t xPeriod = pdMS_TO_TICKS(1000);
    uint32_t attack_duration_ms = 1000;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    
    int cache_hit_threshold = 60;

    //inicializa o array de prova na memoria
    for (int i = 0; i < 256 * 512; i++) {
        array[i] = 1;
    }

    printf("[MELTDOWN] Iniciando Task (Simulada)...\n");

    while(1) {
        vTaskDelayUntil(&xLastWakeTime, xPeriod);
        
        g_label_atual = 1.0f;
        
        // Chama a função passando os ms brutos
        execute_meltdown_attack(cache_hit_threshold, attack_duration_ms);
        
        g_label_atual = 0.0f; 
    }
}

void task_flush_reload(void *arg) {
    const TickType_t xPeriod = pdMS_TO_TICKS(500);          // Frequência do ciclo da task
    uint32_t attack_duration_ms = 500;  // Tempo que o ataque fica rodando
    TickType_t xLastWakeTime = xTaskGetTickCount();
    
    *target_addr = 0xAA;
    
    vTaskDelay(pdMS_TO_TICKS(500));
    uint64_t threshold = calibrate_threshold();

    printf("[ARMAGEDDON] Iniciando Monitoramento Flush+Reload...\n");

    while(1) {
        vTaskDelayUntil(&xLastWakeTime, xPeriod);
        
        g_label_atual = 1.0f; 
        
        execute_flush_reload_attack(threshold, attack_duration_ms);
        
        g_label_atual = 0.0f;
    }
}

void task_zombieload(void *arg) {
    const TickType_t xPeriod = pdMS_TO_TICKS(1000);
    uint32_t attack_duration_ms = 1000;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    
    // configuracao do ataque
    uint8_t secret_val = 88;
    int cache_hit_threshold = 60;

    for (int i = 0; i < 256 * 512; i++) {
        array[i] = 1;
    }

    printf("[ZOMBIELOAD] Iniciando Task (Simulacao de MDS Traffic)...\n");

    while(1) {
        vTaskDelayUntil(&xLastWakeTime, xPeriod);
        
        g_label_atual = 1.0f;
        
        // Passa os milissegundos diretos para o operário
        execute_zombieload_attack(secret_val, cache_hit_threshold, attack_duration_ms);
        
        g_label_atual = 0.0f; 
    }
}

//==============================================================================
//benchmarks
//==============================================================================

void task_bandwidth(void *arg) {
  const TickType_t xPeriod = pdMS_TO_TICKS(1000); 
  TickType_t xLastWakeTime = xTaskGetTickCount();
  
  bandwidth_context_fann_t ctx;
  
  ctx.mem_ptr = (int *)pvPortMalloc(1024 * 1024);
  if (ctx.mem_ptr == NULL) {
      printf("[ERRO] Sem memoria Heap para o Bandwidth!\n");
      vTaskDelete(NULL); //mata a task se nao tiver memoria
  }
  
  ctx.sum = 0;

  printf("[BENCHMARK] Task Bandwidth iniciada...\n");
  fflush(stdout);

  while(1) {
    vTaskDelayUntil(&xLastWakeTime, xPeriod);
    
    g_label_atual = 0.0f;
    
    TickType_t end_bench = xTaskGetTickCount() + pdMS_TO_TICKS(1000);
    
    while (xTaskGetTickCount() < end_bench) {
      bandwidth_wrapper_fann(&ctx);
      //evita otimizacao do compilador
      asm volatile("" : : "r"(ctx.sum) : "memory");
    }
  }
}

void task_disparity(void *arg) {
  const TickType_t xPeriod = pdMS_TO_TICKS(1000); 
  TickType_t xLastWakeTime = xTaskGetTickCount();

  printf("[BENCHMARK] Task Disparity iniciada...\n");
  fflush(stdout);

  while(1) {
    vTaskDelayUntil(&xLastWakeTime, xPeriod);
    
    g_label_atual = 0.0f;
    
    TickType_t end_bench = xTaskGetTickCount() + pdMS_TO_TICKS(1000);
    
    while (xTaskGetTickCount() < end_bench) {
      disparity_wrapper_fann();
    }
  }
}

void task_qsort(void *arg) {
  const TickType_t xPeriod = pdMS_TO_TICKS(1000); 
  TickType_t xLastWakeTime = xTaskGetTickCount();

  printf("[BENCHMARK] Task Qsort iniciada...\n");
  fflush(stdout);

  while(1) {
    vTaskDelayUntil(&xLastWakeTime, xPeriod);
    
    g_label_atual = 0.0f;
    
    TickType_t end_bench = xTaskGetTickCount() + pdMS_TO_TICKS(1000);
    
    while (xTaskGetTickCount() < end_bench) {
      qsort_wrapper_fann();
    }
  }
}

void task_dijkstra(void *arg) {
  const TickType_t xPeriod = pdMS_TO_TICKS(1000); 
  TickType_t xLastWakeTime = xTaskGetTickCount();

  printf("[BENCHMARK] Task Dijkstra iniciada...\n");
  fflush(stdout);

  while(1) {
    vTaskDelayUntil(&xLastWakeTime, xPeriod);
    
    g_label_atual = 0.0f;
    
    TickType_t end_bench = xTaskGetTickCount() + pdMS_TO_TICKS(1000);
    
    while (xTaskGetTickCount() < end_bench) {
      dijkstra_wrapper_fann();
    }
  }
}

void task_sha(void *arg) {
  const TickType_t xPeriod = pdMS_TO_TICKS(1000); 
  TickType_t xLastWakeTime = xTaskGetTickCount();

  printf("[BENCHMARK] Task SHA iniciada...\n");
  fflush(stdout);

  while(1) {
    vTaskDelayUntil(&xLastWakeTime, xPeriod);
    
    g_label_atual = 0.0f;
    
    TickType_t end_bench = xTaskGetTickCount() + pdMS_TO_TICKS(1000);
    
    while (xTaskGetTickCount() < end_bench) {
      sha_wrapper_fann();
    }
  }
}

void task_fft(void *arg) {
  const TickType_t xPeriod = pdMS_TO_TICKS(1000); 
  TickType_t xLastWakeTime = xTaskGetTickCount();

  printf("[BENCHMARK] Task FFT iniciada...\n");
  fflush(stdout);

  while(1) {
    vTaskDelayUntil(&xLastWakeTime, xPeriod);
    
    g_label_atual = 0.0f;
    
    TickType_t end_bench = xTaskGetTickCount() + pdMS_TO_TICKS(1000);
    
    while (xTaskGetTickCount() < end_bench) {
      fft_wrapper_fann();
    }
  }
}

void task_sorting(void *arg) {
  const TickType_t xPeriod = pdMS_TO_TICKS(1000); 
  TickType_t xLastWakeTime = xTaskGetTickCount();

  printf("[BENCHMARK] Task Sorting iniciada...\n");
  fflush(stdout);

  while(1) {
    vTaskDelayUntil(&xLastWakeTime, xPeriod);
    
    g_label_atual = 0.0f;
    
    TickType_t end_bench = xTaskGetTickCount() + pdMS_TO_TICKS(1000);
    
    while (xTaskGetTickCount() < end_bench) {
      sorting_wrapper_fann();
    }
  }
}