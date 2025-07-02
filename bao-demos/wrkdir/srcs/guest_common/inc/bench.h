#ifndef BENCH_H
#define BENCH_H

typedef struct {
  volatile int (*function)(void);
  const char *name;
  int vm_num;
  int task_num;
  const char *budget_function;
} BenchInfo;

BenchInfo get_benchmark_info(int vm_num, int task_num);

#endif
