#include "../inc/tasks.h"
#include "FreeRTOS.h"
#include "queue.h"
#include <stdio.h>

#include "../inc/globals.h"

#include "benchmarks_wrappers.h"
#include "neural_network.h"

// Context for bandwidth benchmark
static bandwidth_context_fann_t bw_ctx;

void run_bandwidth(void) {
  bandwidth_wrapper_fann(&bw_ctx);
  // evita otimizacao do compilador
  asm volatile("" : : "r"(bw_ctx.sum) : "memory");
}

void run_disparity(void) {
  disparity_wrapper_fann();
}

void run_fft(void) {
  fft_wrapper_fann();
}

void run_qsort_bench(void) {
  qsort_wrapper_fann();
}

void run_dijkstra(void) {
  dijkstra_wrapper_fann();
}

void run_sha(void) {
  sha_wrapper_fann();
}

void run_sorting(void) {
  sorting_wrapper_fann();
}

void task_orchestrator(void *arg) {
  IPC_Channel *ipc = (IPC_Channel *)IPC_BASE_ADDR;

  bw_ctx.mem_ptr = (int *)pvPortMalloc(1024 * 1024);
  if (bw_ctx.mem_ptr == NULL) {
    printf("[VM1][ERRO] Sem memoria Heap para o Bandwidth!\n");
    vTaskDelete(NULL);
  }
  bw_ctx.sum = 0;

  int current_bench = LABEL_BANDWIDTH; // Start with bandwidth

  printf("[VM1] Task Orquestradora iniciada. Canal IPC: 0x%x\n", IPC_BASE_ADDR);
  fflush(stdout);

  while (1) {
    TickType_t start_time = xTaskGetTickCount();
    TickType_t next_sync = start_time + pdMS_TO_TICKS(SYNC_INTERVAL_MS);
    TickType_t end_time = start_time + pdMS_TO_TICKS(BENCHMARK_DURATION_MS);

    g_label_atual = (float)current_bench;
    printf("[VM1] Rodando benchmark %d por 10 minutos...\n", current_bench);
    fflush(stdout);

    while (xTaskGetTickCount() < end_time) {
      switch (current_bench) {
        case LABEL_BANDWIDTH: run_bandwidth(); break;
        case LABEL_DISPARITY: run_disparity(); break;
        case LABEL_FFT: run_fft(); break;
        case LABEL_QSORT: run_qsort_bench(); break;
        case LABEL_DIJKSTRA: run_dijkstra(); break;
        case LABEL_SHA: run_sha(); break;
        case LABEL_SORTING: run_sorting(); break;
      }

      // Check if it's time to sync
      if (xTaskGetTickCount() >= next_sync) {
        // Prepare data for VM0
        ipc->current_label = current_bench;
        asm volatile("dsb sy" ::: "memory");
        
        // Signal VM0
        ipc->signal_ready = 1;
        
        // Wait for VM0 to log and send resume
        while (ipc->resume == 0) {
          vTaskDelay(pdMS_TO_TICKS(1));
        }

        // Clear resume
        ipc->resume = 0;
        asm volatile("dsb sy" ::: "memory");

        // Schedule next sync 15 seconds from now
        next_sync = xTaskGetTickCount() + pdMS_TO_TICKS(SYNC_INTERVAL_MS);
        
        // Extend end_time slightly to account for time spent paused, ensuring full 10m of real work
        // (Optional, for now we let it be strictly wall-clock based without trying to be perfect)
      }
    }

    current_bench++;
    if (current_bench > NUM_BENCHMARKS) {
      current_bench = LABEL_BANDWIDTH;
    }
  }
}