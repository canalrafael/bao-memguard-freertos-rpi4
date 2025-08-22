#ifndef BENCH_H
#define BENCH_H

#include "budget.h"
#include <stdint.h>

#define BENCH_ARRAY_INDEX 98

typedef struct {
  int index;
  void (*pointer)(void);
  const char *name;
} Function;

typedef struct {
  Function function;
  int task_num;
  // TaskHandle_t task_handle;
  // formula_t budget_formula;
  int periodicity;
  int task_overruns;
  int task_underruns;
} BenchInfo;

BenchInfo *add_benchmark_info(int vm_num, int task_num, int periodicity);
BenchInfo *get_benchmark_info(int vm_num, int task_num);
void set_array_index(int index);

// formula handling
formula_t get_budget_formula();
void set_budget_formula(formula_t formula);

void init_bench();
void destroy_bench();

#endif
