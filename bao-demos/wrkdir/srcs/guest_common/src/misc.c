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
  // int64_t total_used_budget_r;
  // int64_t total_used_budget_w;
  //
  int64_t total_calc_r;
  int64_t total_calc_w;
  //
  int64_t PMU_counter_used_budget;
  int64_t PMU_counter_used_budget_r;
  int64_t PMU_counter_used_budget_w;
  // int64_t total_calc_rw_per_index[PERIOD_QNT];
  //
  int64_t PMU_counter_used_budget_r_per_period[PERIOD_QNT];
  int64_t PMU_counter_used_budget_w_per_period[PERIOD_QNT];
  // // int64_t total_used_budget_rw_per_index[PERIOD_QNT];
  //
  int64_t calc_per_period_read[PERIOD_QNT];
  int64_t calc_per_period_write[PERIOD_QNT];
  //
  int64_t total_cycles;
  int64_t total_instructions;
  int64_t total_cache_misses;
  int64_t total_mispredicts;
  int64_t cycles[PERIOD_QNT];
  int64_t instructions[PERIOD_QNT];
  int64_t cache_misses[PERIOD_QNT];
  int64_t mispredicts[PERIOD_QNT];
  //
  // int64_t period_duration[PERIOD_QNT];
  // int64_t period_duration_total;
  //
  // int64_t clock_per_period[PERIOD_QNT];
  // int64_t total_clock;
  //
  // int64_t completed_runs_per_task[TASK_QUANTITY];
  // int64_t has_overflowed[PERIOD_QNT];
} BenchmarkData;

void write_used_budget(struct VM vm_info, BenchmarkData *d) {
  //
  d->PMU_counter_used_budget_r = 0;
  d->PMU_counter_used_budget_w = 0;
  d->PMU_counter_used_budget = 0;
  for (uint8_t i = 0; i < PERIOD_QNT; i++) {
    int64_t period_read_used = vm_info.used_r_budget_period[i];
    int64_t period_write_used = vm_info.used_w_budget_period[i];

    d->PMU_counter_used_budget_r_per_period[i] = period_read_used;
    d->PMU_counter_used_budget_w_per_period[i] = period_write_used;
    d->PMU_counter_used_budget_r += period_read_used;
    d->PMU_counter_used_budget_w += period_write_used;
    d->PMU_counter_used_budget += period_read_used + period_write_used;
  }
}

void write_calc_budget(struct VM vm_info, BenchmarkData *d) {
  // printf("\tCalc per period \n\t\tr ");

  d->total_calc_r = 0;
  d->total_calc_w = 0;
  for (uint8_t i = 0; i < PERIOD_QNT; i++) {
    int64_t period_calc_read = vm_info.calc_r_budget_period[i];
    int64_t period_calc_write = vm_info.calc_w_budget_period[i];

    d->calc_per_period_read[i] = period_calc_read;
    d->calc_per_period_write[i] = period_calc_write;
    d->total_calc_r += period_calc_read;
    d->total_calc_w += period_calc_write;
  }
}

void write_raw(struct VM vm_info, BenchmarkData *d) {
  d->total_cycles = 0;
  d->total_instructions = 0;
  d->total_cache_misses = 0;
  d->total_mispredicts = 0;
  for (uint8_t i = 0; i < PERIOD_QNT; i++) {
    d->cycles[i] = vm_info.PMU_raw_values[2][i];
    d->instructions[i] = vm_info.PMU_raw_values[3][i];
    d->cache_misses[i] = vm_info.PMU_raw_values[4][i];
    d->mispredicts[i] = vm_info.PMU_raw_values[5][i];
    //
    d->total_cycles += d->cycles[i];
    d->total_instructions += d->instructions[i];
    d->total_cache_misses += d->cache_misses[i];
    d->total_mispredicts += d->mispredicts[i];
  }
}

// void write_period_duration(struct VM vm_info, BenchmarkData *d) {
//   // printf("\tCalc per period \n\t\tr ");
//
//   d->period_duration_total = 0;
//   for (uint8_t i = 0; i < PERIOD_QNT; i++) {
//     d->period_duration[i] = vm_info.period_duration[i];
//     d->period_duration_total += vm_info.period_duration[i];
//   }
// }

// void write_others(struct VM vm_info, BenchmarkData *d) {
//
//   // runs
//   for (uint8_t i = 0; i < TASK_QUANTITY; i++) {
//     d->completed_runs_per_task[i] = vm_info.completed_runs_per_task[i];
//   }
//
//   for (uint8_t i = 0; i < PERIOD_QNT; i++) {
//     d->has_overflowed[i] = vm_info.has_overflowed[i];
//   }
// }

// void write_clock_cycle(struct VM vm_info, BenchmarkData *d) {
//   // printf("\tClock per period \n\t\t");
//
//   int64_t total_clock = 0;
//   for (int8_t i = 0; i < PERIOD_QNT; i++) {
//     int64_t clock_period = vm_info.cycle_per_period[i];
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
//     for (size_t __i = 0; __i < PERIOD_QNT; ++__i) { \
//       current += snprintf(current, buffer + sizeof(buffer) - current, "%d", \
//                           (arr)[__i]); \
//       if (__i + 1 < PERIOD_QNT) \
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
//   print_header_array("used_budget_per_period_read", PERIOD_QNT);
//   print_header_array("used_budget_per_period_write", PERIOD_QNT);
//   print_header_array("total_used_budget_rw_per_index", PERIOD_QNT);
//   printf("total_used_budget_rw,");
//   print_header_array("calc_per_period_read", PERIOD_QNT);
//   print_header_array("calc_per_period_write", PERIOD_QNT);
//   print_header_array("total_calc_rw_per_index", PERIOD_QNT);
//   printf("total_calc_rw,");
//   print_header_array("clock_per_period", PERIOD_QNT);
//   printf("total_clock\n"); // no end comma
// }

void print_vm_header() {
  printf("budget_function,PMU_counter_used_budget,");
  printf("PMU_counter_used_budget_r,PMU_counter_used_budget_w,");
  printf("total_calc_r,total_calc_w,");

  printf("total_cycles,");
  printf("total_instructions,");
  printf("total_cache_misses,");
  printf("total_mispredicts,");
  //
  for (int i = 0; i < PERIOD_QNT; i++) {
    printf("cycles[%d],", i);
  }
  for (int i = 0; i < PERIOD_QNT; i++) {
    printf("instructions[%d],", i);
  }
  for (int i = 0; i < PERIOD_QNT; i++) {
    printf("cache_misses[%d],", i);
  }
  for (int i = 0; i < PERIOD_QNT; i++) {
    printf("mispredicts[%d],", i);
  }
  //
  for (int i = 0; i < PERIOD_QNT; i++) {
    printf("used_budget_read[%d],", i);
  }
  for (int i = 0; i < PERIOD_QNT; i++) {
    printf("used_budget_write[%d],", i);
  }
  for (int i = 0; i < PERIOD_QNT; i++) {
    printf("calculated_read[%d],", i);
  }
  for (int i = 0; i < PERIOD_QNT; i++) {
    printf("calculated_write[%d],", i);
  }
  printf("\n");

  // printf("budget_function,period_duration_total,");
  // printf("has_overflowed,");
  //
  // for (int i = 0; i < TASK_QUANTITY; i++) {
  //   printf("completed_runs_per_task[%d],", i);
  // }
  // for (int i = 0; i < PERIOD_QNT; i++) {
  //   printf("period_duration[%d],", i);
  // }
}

void print_array(int64_t values[PERIOD_QNT]) {
  for (int i = 0; i < PERIOD_QNT; i++) {
    printf("%ld,", values[i]);
  }
}

void print_task_array(int64_t values[TASK_QUANTITY]) {
  for (int i = 0; i < TASK_QUANTITY; i++) {
    printf("%ld,", values[i]);
  }
}

void print_compact(int64_t values[PERIOD_QNT]) {
  for (int i = 0; i < PERIOD_QNT; i++) {
    printf("%s", values[i] ? "O" : "-");
  }
  printf(",");
}

void print_vm_info(struct VM vm_info) {
  BenchmarkData info;
  write_used_budget(vm_info, &info);
  write_calc_budget(vm_info, &info);
  write_raw(vm_info, &info);
  // write_period_duration(vm_info, &info);
  // write_others(vm_info, &info);
  // write_clock_cycle(vm_info, &info);

  // printf("%ld,", info.period_duration_total);
  printf("%s,", get_formula_name(get_budget_formula()));
  printf("%ld,", info.PMU_counter_used_budget);
  printf("%ld,", info.PMU_counter_used_budget_r);
  printf("%ld,", info.PMU_counter_used_budget_w);
  printf("%ld,", info.total_calc_r);
  printf("%ld,", info.total_calc_w);

  printf("%ld,", info.total_cycles);
  printf("%ld,", info.total_instructions);
  printf("%ld,", info.total_cache_misses);
  printf("%ld,", info.total_mispredicts);

  print_array(info.cycles);
  print_array(info.instructions);
  print_array(info.cache_misses);
  print_array(info.mispredicts);

  print_array(info.PMU_counter_used_budget_r_per_period);
  print_array(info.PMU_counter_used_budget_w_per_period);
  // print_compact(info.has_overflowed);
  // printf("%lu,", info.total_clock);
  // print_task_array(info.completed_runs_per_task);
  // print_array(info.used_budget_per_period_read);
  // print_array(info.used_budget_per_period_write);
  print_array(info.calc_per_period_read);
  print_array(info.calc_per_period_write);
  // print_array(info.period_duration);
  // print_array(info.clock_per_period);

  printf("\n");
}
