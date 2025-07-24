// Filename: regulation.h
// Author: Everaldo Pereira Gomes

#ifndef REGULATION_H
#define REGULATION_H

#include <stdint.h>

#define DEBUG 0

// =======================================
// Beginning do not change
// =======================================
#define UNUSED_ARG 0
#define MAX_INT 4294967295U

#define NO 0
#define YES 1

#define READ 0
#define WRITE 1

#define GUEST_SUSPEND_BUDGET_ID 0
// #define GUEST_RESUME_TASK_0_ID 0
// #define GUEST_SUSPEND_TASK_0_BUDGET_ID 1
//
// #define GUEST_RESUME_TASK_1_ID 2
// #define GUEST_SUSPEND_TASK_1_BUDGET_ID 3

#define VM_0 0
#define VM_1 1
#define VM_2 2
#define VM_3 3

#define OTHER_TASK_PRIORITY 0
#define CTRL_TASK_PRIORITY 1

// #define TASK_0 0
// #define TASK_1 1
//
// #define TASK_PRIORITY_0 0
// #define TASK_PRIORITY_1 1
// #define TASK_PRIORITY_2 2
// #define TASK_PRIORITY_3 3
// #define TASK_PRIORITY_4 4
// #define TASK_PRIORITY_5 5
// #define TASK_PRIORITY_6 6
// #define TASK_PRIORITY_7 7

#define PMU_COUNTER0 0
#define PMU_COUNTER1 1
#define PMU_COUNTER2 2
#define PMU_COUNTER3 3
#define PMU_COUNTER4 4
#define PMU_COUNTER5 5

// #define BENCHMARK_BANDWIDTH 0
// #define BENCHMARK_DISPARITY 1
// #define BENCHMARK_MSER 2
// #define BENCHMARK_FFT 3
// #define BENCHMARK_SORTING 4
// #define BENCHMARK_QSORT 5
// #define BENCHMARK_DIJKSTRA 6
// #define BENCHMARK_SHA 7

#define TASK_STACK_SIZE 400
// #define BANDWIDTH_TASK_STACK_SIZE 400
// #define DISPARITY_TASK_STACK_SIZE 400
// #define MSER_TASK_STACK_SIZE 400
// #define FFT_TASK_STACK_SIZE 400
// #define SORTING_TASK_STACK_SIZE 400
// #define QSORT_TASK_STACK_SIZE 400
// #define DIJKSTRA_TASK_STACK_SIZE 400
// #define SHA_TASK_STACK_SIZE 400
//
// #define BANDWIDTH_TASK_NAME "Bandwidth_task"
// #define DISPARITY_TASK_NAME "Disparity_task"
// #define MSER_TASK_NAME "MSER_task"
// #define FFT_TASK_NAME "FFT_task"
// #define SORTING_TASK_NAME "Sorting_task"
// #define QSORT_TASK_NAME "QSort_task"
// #define DIJKSTRA_TASK_NAME "Dijkstra_task"
// #define SHA_TASK_NAME "SHA_task"
// =======================================
// End do not change
// =======================================

#define EXEC_VM_0 1
#define EXEC_VM_1 1
#define EXEC_VM_2 1
#define EXEC_VM_3 1

#define VM_0_REGULATION 1
#define VM_1_REGULATION 1
#define VM_2_REGULATION 1
#define VM_3_REGULATION 1

#define PERIOD_QNT 10

// Periodicity of each task
// #define PERIOD_MS_TASK_0 30
#define PERIOD_MS_TASK_ANY 30
#define PERIOD_MS_TASK_CTRL 10

// The time of the period for each task to run (old value 100)
// #define VM_0_PERIOD_REGULATION_MS_TASK_0 PERIOD_MS_TASK_0
// #define VM_0_PERIOD_REGULATION_MS_TASK_1 PERIOD_MS_TASK_1
//
// #define VM_1_PERIOD_REGULATION_MS_TASK_0 PERIOD_MS_TASK_0
// #define VM_1_PERIOD_REGULATION_MS_TASK_1 PERIOD_MS_TASK_1
//
// #define VM_2_PERIOD_REGULATION_MS_TASK_0 PERIOD_MS_TASK_0
// #define VM_2_PERIOD_REGULATION_MS_TASK_1 PERIOD_MS_TASK_1
//
// #define VM_3_PERIOD_REGULATION_MS_TASK_0 PERIOD_MS_TASK_0
// #define VM_3_PERIOD_REGULATION_MS_TASK_1 PERIOD_MS_TASK_1

// #define DEADLINE_MS_TASK_0 5
// #define DEADLINE_MS_TASK_1 10

#define BENCHMARK_MAX_EXE 10
#define CACHE_COLORING 1

// =======================================
// Beginning do not change
// =======================================
#define INCREMENT_IF_EXEC_VM_0 (EXEC_VM_0 ? 1 : 0)
#define INCREMENT_IF_EXEC_VM_1 (EXEC_VM_1 ? 1 : 0)
#define INCREMENT_IF_EXEC_VM_2 (EXEC_VM_2 ? 1 : 0)
#define INCREMENT_IF_EXEC_VM_3 (EXEC_VM_3 ? 1 : 0)

#define RUNNING_VMs                                                            \
  (INCREMENT_IF_EXEC_VM_0 + INCREMENT_IF_EXEC_VM_1 + INCREMENT_IF_EXEC_VM_2 +  \
   INCREMENT_IF_EXEC_VM_3)

#define VM_QNT 4
#define TASK_QUANTITY 2
// #define BENCHMARK_QNT 8

struct VM {
  // uint8_t pmu_counter_read;
  // uint8_t pmu_counter_write;

  const uint32_t r_fac;
  const uint32_t w_fac;
  uint8_t sgi_suspend_task_budget;
  const uint32_t total_read_allowed_budget;
  const uint32_t total_write_allowed_budget;
  uint32_t new_read_budget;
  uint32_t new_write_budget;
  uint32_t deadline_met_counter;
  uint32_t deadline_missed_counter;
  uint32_t cycle_per_period[PERIOD_QNT];
  uint32_t task_start_time[PERIOD_QNT];
  uint32_t task_end_time[PERIOD_QNT];
  uint32_t task_deadline[PERIOD_QNT];
  uint32_t used_r_budget_period[PERIOD_QNT];
  uint32_t used_w_budget_period[PERIOD_QNT];
  uint32_t calc_r_budget_period[PERIOD_QNT];
  uint32_t calc_w_budget_period[PERIOD_QNT];
};

extern struct VM vm_conf[VM_QNT];

// typedef struct {
//   // uint8_t task_num;
//   uint8_t pmu_counter_a;
//   uint8_t pmu_counter_b;
//   uint8_t priority;
//   uint16_t periodicity;
//   uint8_t pmu_cpu_cycles_counter;
//   uint8_t show_exe_info;
//   uint32_t total_cpu_cycles;
//   uint32_t deadline_ms;
// } TASK;

// extern TASK task_conf[TASK_QNT];

// =======================================
// End do not change
// =======================================

#endif
