#ifndef BENCH_H
#define BENCH_H

#include "budget.h"

typedef struct {
  volatile int (*pointer)(void);
  const char *name;
} Function;

typedef struct {
  Function function;
  int function_index;
  int vm_num;
  int task_num;
  formula_t budget_formula;
} BenchInfo;

BenchInfo get_benchmark_info(int vm_num, int task_num);
void set_benchmark_formula(int vm_num, int task_num, formula_t formula);

#endif
