// Filename: misc.c
// Author: Everaldo Pereira Gomes

#include "../inc/misc.h"
// #include "../inc/bench.h"
#include "../inc/regulation.h"

#define PRINT_CSV 1

// void print_csv_task_data(BenchInfo info);
//
// static void print_clock_cycle(const uint8_t vm_num, const uint8_t task_num) {
//   printf("\tClock per period \n\t\t");
//   uint64_t total_clock = 0;
//
//   for (uint8_t i = 0; i < PERIOD_QNT; i++) {
//     total_clock += vm_info[vm_num].cycle_per_period[task_num][i];
//     printf("%u, ", vm_info[vm_num].cycle_per_period[task_num][i]);
//   }
//
//   printf("\n\tTotal clock %lu\n\n", total_clock);
// }
//
// static void print_used_budget(const uint8_t vm_num, const uint8_t task_num) {
//   printf("\tUsed per period \n\t\tr ");
//
//   uint64_t total_used = 0;
//   for (uint8_t i = 0; i < PERIOD_QNT; i++) {
//     total_used += vm_info[vm_num].used_r_budget_period[task_num][i];
//     printf("%u, ", vm_info[vm_num].used_r_budget_period[task_num][i]);
//   }
//
//   printf("\n\t\tw ");
//   for (uint8_t i = 0; i < PERIOD_QNT; i++) {
//     total_used += vm_info[vm_num].used_w_budget_period[task_num][i];
//     printf("%u, ", vm_info[vm_num].used_w_budget_period[task_num][i]);
//   }
//
//   printf("\n\tTotal used per index \n\t\t");
//   for (uint8_t i = 0; i < PERIOD_QNT; i++) {
//     printf("%u, ", vm_info[vm_num].used_r_budget_period[task_num][i] +
//                        vm_info[vm_num].used_w_budget_period[task_num][i]);
//   }
//
//   printf("\n\tTotal used %lu\n\n", total_used);
// }
//
// static void print_calc_budget(const uint8_t vm_num, const uint8_t task_num) {
//   printf("\tCalc per period \n\t\tr ");
//
//   uint64_t total_calc = 0;
//   for (uint8_t i = 0; i < PERIOD_QNT; i++) {
//     total_calc += vm_info[vm_num].calc_r_budget_period[task_num][i];
//     printf("%u, ", vm_info[vm_num].calc_r_budget_period[task_num][i]);
//   }
//
//   printf("\n\t\tw ");
//
//   for (uint8_t i = 0; i < PERIOD_QNT; i++) {
//     total_calc += vm_info[vm_num].calc_w_budget_period[task_num][i];
//     printf("%u, ", vm_info[vm_num].calc_w_budget_period[task_num][i]);
//   }
//
//   printf("\n\tTotal calc per index \n\t\t");
//   for (uint8_t i = 0; i < PERIOD_QNT; i++) {
//     printf("%u, ", vm_info[vm_num].calc_r_budget_period[task_num][i] +
//                        vm_info[vm_num].calc_w_budget_period[task_num][i]);
//   }
//
//   printf("\n\tTotal calc %lu\n\n", total_calc);
// }
//
// void print_timeline(int vm_num, int task_num, const char *task_name) {
//   printf("<VM%d, T%d> %s\n", vm_num, task_num, task_name);
// }
//
// void print_end_info_no_reg(const uint8_t vm_num, const uint8_t task_num,
//                            const char *task_name) {
//   for (volatile int i = 0; i < 10000000; i++)
//     ;
//   print_timeline(vm_num, task_num, task_name);
//   // print_clock_cycle(vm_num, task_num);
// }
//
// void print_end_info_reg(const uint8_t vm_num, const uint8_t task_num,
//                         const char *task_name) {
//   print_timeline(vm_num, task_num,
//                  (task_name == NULL ? (char *)"--noname--" : task_name));
//   print_used_budget(vm_num, task_num);
//   print_calc_budget(vm_num, task_num);
//   print_clock_cycle(vm_num, task_num);
// }
//
// void print_end_bench_info_reg(BenchInfo info) {
//   if (info.budget_formula < 0 || info.budget_formula >= FORMULA_COUNT) {
//     printf("invalid budget formula %d, not printing\n", info.budget_formula);
//     return;
//   }
//   print_csv_task_data(info);
// }
//
/// CSV related
///
///
typedef struct {
  int64_t total_used_budget_r;
  int64_t total_used_budget_w;

  int64_t total_calc_r;
  int64_t total_calc_w;

  int64_t used_budget_per_period_read[BENCHMARK_MAX_EXE];
  int64_t used_budget_per_period_write[BENCHMARK_MAX_EXE];
  // int64_t total_used_budget_rw_per_index[BENCHMARK_MAX_EXE];

  int64_t calc_per_period_read[BENCHMARK_MAX_EXE];
  int64_t calc_per_period_write[BENCHMARK_MAX_EXE];
  // int64_t total_calc_rw_per_index[BENCHMARK_MAX_EXE];

  // int clock_per_period[BENCHMARK_MAX_EXE];
  // int total_clock;
} BenchmarkData;

void write_used_budget(struct VM vm_info, BenchmarkData *d) {
  // printf("\tUsed budget per period \n\t\tr ");

  int64_t total_read_used = 0;
  int64_t total_write_used = 0;
  for (uint8_t i = 0; i < PERIOD_QNT; i++) {
    int64_t period_read_used = vm_info.used_r_budget_period[i];
    int64_t period_write_used = vm_info.used_w_budget_period[i];

    total_read_used += period_read_used;
    total_write_used += period_write_used;

    d->used_budget_per_period_read[i] = period_read_used;
    d->used_budget_per_period_write[i] = period_write_used;
    // d->total_used_budget_rw_per_index[i] = period_read_used +
    // period_write_used;
  }
  d->total_used_budget_r = total_read_used;
  d->total_used_budget_w = total_write_used;
}

void write_calc_budget(struct VM vm_info, BenchmarkData *d) {
  // printf("\tCalc per period \n\t\tr ");

  uint64_t total_calc_read = 0;
  uint64_t total_calc_write = 0;
  for (uint8_t i = 0; i < PERIOD_QNT; i++) {
    uint64_t period_calc_read = vm_info.calc_r_budget_period[i];
    uint64_t period_calc_write = vm_info.calc_w_budget_period[i];

    total_calc_read += period_calc_read;
    total_calc_write += period_calc_write;

    d->calc_per_period_read[i] = period_calc_read;
    d->calc_per_period_write[i] = period_calc_write;
    // d->total_calc_rw_per_index[i] = period_calc_read + period_calc_write;
  }
  d->total_calc_r = total_calc_read;
  d->total_calc_w = total_calc_write;
}

// void write_clock_cycle(struct VM info, BenchmarkData *d) {
//   // printf("\tClock per period \n\t\t");
//
//   uint64_t total_clock = 0;
//   for (uint8_t i = 0; i < PERIOD_QNT; i++) {
//     uint64_t clock_period =
//         vm_info[info.vm_num].cycle_per_period[info.task_num][i];
//     total_clock += clock_period;
//
//     d->clock_per_period[i] = clock_period;
//   }
//
//   d->total_clock = total_clock;
// }

// void print_value_array(int *values, int size, char buffer[], char current[])
// {
//   for (int i = 0; i < size; ++i) {
//     printf("%d,", values[i]);
//   }
// }
//
// #define append_array(arr) \
//   do { \
//     for (size_t __i = 0; __i < BENCHMARK_MAX_EXE; ++__i) { \
//       current += snprintf(current, buffer + sizeof(buffer) - current, "%d", \
//                           (arr)[__i]); \
//       if (__i + 1 < BENCHMARK_MAX_EXE) \
//         *current++ = ','; \
//     } \
//     *current++ = ','; /* comma after array */ \
//   } while (0)
//
// void print_csv_task_data(BenchInfo info) {
//   // Benchmark Calculations
//   BenchmarkData d;
//   write_used_budget(info, &d);
//   write_calc_budget(info, &d);
//   write_clock_cycle(info, &d);
//
//   char buffer[2048];
//   char *current = buffer;
//
//   // BenchInfo
//   // printf("%d,%d,%d,%s,%s,", info.vm_num, info.task_num,
//   info.function_index,
//   //        info.function.name, get_formula_name(info.budget_formula));
//   current +=
//       snprintf(current, buffer + sizeof(buffer) - current, "%d,%d,%d,%s,%s,",
//                info.vm_num, info.task_num, info.function_index,
//                info.function.name, get_formula_name(info.budget_formula));
//
//   // BenchmarkData
//   append_array(d.used_budget_per_period_read);
//   append_array(d.used_budget_per_period_write);
//   append_array(d.total_used_budget_rw_per_index);
//   // printf("%d,", d.total_used_budget_rw);
//   current += snprintf(current, buffer + sizeof(buffer) - current, "%d,",
//                       d.total_used_budget_rw);
//   append_array(d.calc_per_period_read);
//   append_array(d.calc_per_period_write);
//   append_array(d.total_calc_rw_per_index);
//   // printf("%d,", d.total_calc_rw);
//   current += snprintf(current, buffer + sizeof(buffer) - current, "%d,",
//                       d.total_calc_rw);
//   append_array(d.clock_per_period);
//   // printf("%d\n", d.total_clock); // no end comma
//   current += snprintf(current, buffer + sizeof(buffer) - current, "%d\n",
//                       d.total_clock);
//   fputs(buffer, stdout);
//
//   if (info.budget_formula == (FORMULA_COUNT - 1)) {
//     fflush(stdout);
//   }
// }
//
// void print_header_array(const char *name, int size) {
//   for (int i = 0; i < size; ++i) {
//     printf("%s[%d],", name, i);
//   }
// }
//
// void print_csv_header() {
//   // Info Header
//   printf("%s,%s,%s,%s,%s,", "VM", "Task", "Function Index", "Task Name",
//          "Budget Function");
//
//   // benchmark header
//   print_header_array("used_budget_per_period_read", BENCHMARK_MAX_EXE);
//   print_header_array("used_budget_per_period_write", BENCHMARK_MAX_EXE);
//   print_header_array("total_used_budget_rw_per_index", BENCHMARK_MAX_EXE);
//   printf("total_used_budget_rw,");
//   print_header_array("calc_per_period_read", BENCHMARK_MAX_EXE);
//   print_header_array("calc_per_period_write", BENCHMARK_MAX_EXE);
//   print_header_array("total_calc_rw_per_index", BENCHMARK_MAX_EXE);
//   printf("total_calc_rw,");
//   print_header_array("clock_per_period", BENCHMARK_MAX_EXE);
//   printf("total_clock\n"); // no end comma
// }

void print_vm_header() {
  printf("budget_function,");
  printf("total_used_budget_r,total_used_budget_w,");
  printf("total_calc_r,total_calc_w,");

  for (int i = 0; i < PERIOD_QNT; i++) {
    printf("used_budget_per_period_read[%d],", i);
  }
  for (int i = 0; i < PERIOD_QNT; i++) {
    printf("used_budget_per_period_write[%d],", i);
  }
  for (int i = 0; i < PERIOD_QNT; i++) {
    printf("calc_per_period_read[%d],", i);
  }
  for (int i = 0; i < PERIOD_QNT; i++) {
    printf("calc_per_period_write[%d],", i);
  }

  printf("\n");
}

void print_array(int64_t values[PERIOD_QNT]) {
  for (int i = 0; i < PERIOD_QNT; i++) {
    printf("%lu,", values[i]);
  }
}

void print_vm_info(struct VM vm_info) {
  // printf("%u,%u,%u,%u,", vm_info.pmu_counter_pair_rw, vm_info.r_fac,
  //        vm_info.w_fac, vm_info.sgi_suspend_task_budget);

  BenchmarkData info;
  write_used_budget(vm_info, &info);
  write_calc_budget(vm_info, &info);

  printf("%s,", get_formula_name(get_budget_formula()));
  printf("%lu,%lu,", info.total_used_budget_r, info.total_used_budget_w);
  printf("%lu,%lu,", info.total_calc_r, info.total_calc_w);
  print_array(info.used_budget_per_period_read);
  print_array(info.used_budget_per_period_write);
  print_array(info.calc_per_period_read);
  print_array(info.calc_per_period_write);

  printf("\n");
}
