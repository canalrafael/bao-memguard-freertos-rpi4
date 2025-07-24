#ifndef BENCH_H
#define BENCH_H

#include "budget.h"

#define BENCH_ARRAY_INDEX 0

typedef struct {
  int index;
  void (*pointer)(void);
  const char *name;
} Function;

typedef struct {
  Function function;
  int task_num;
  void *task_handle;
  formula_t budget_formula;
  int periodicity;
  int task_overruns;
  int task_underruns;
} BenchInfo;

BenchInfo *get_benchmark_info(int vm_num, int task_num);

// formula handling
formula_t get_budget_formula();
void set_budget_formula(formula_t formula);

void init_bench();
void destroy_bench();

#endif
