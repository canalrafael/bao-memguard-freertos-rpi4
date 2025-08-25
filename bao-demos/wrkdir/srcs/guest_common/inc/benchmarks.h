#ifndef BENCHMARKS_H
#define BENCHMARKS_H

#include <stdint.h>
#include <stdlib.h>

// #-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#//

#include "../../benchmarks/bandwidth/inc/bandwidth.h"
#include "../../benchmarks/dijkstra/inc/dijkstra.h"
#include "../../benchmarks/disparity/inc/disparity.h"
#include "../../benchmarks/fft/inc/fft.h"
#include "../../benchmarks/mser/inc/mser.h"
#include "../../benchmarks/qsort/inc/qsort.h"
#include "../../benchmarks/sha/inc/sha.h"
#include "../../benchmarks/sorting/inc/sorting.h"

void empty_wrapper();
void bandwidth_wrapper();
void dijkstra_wrapper();
void disparity_wrapper();
void fft_wrapper();
void mser_wrapper();
void qsort_wrapper();
void sha_wrapper();
void sorting_wrapper();

// #-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#//

#define BENCH_ARRAY_INDEX 159
#define FORMULA_COUNT 11

typedef enum {
  // LR_FORMULA, // <not implemented>
  EWMA_V2_FORMULA,
  SW_V2_FORMULA,
  AFC_V2_FORMULA,
  LR_V2_FORMULA,
  AMBP_V2_FORMULA,
  PIC_V2_FORMULA,
  //
  EWMA_FORMULA,
  SW_FORMULA,
  AFC_FORMULA,
  AMBP_FORMULA,
  PIC_FORMULA,
  //
  // FORMULA_COUNT,
} formula_t;

typedef struct {
  formula_t formula;
} Benchmark;

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

Benchmark *benchmark_create();
void benchmark_destroy(Benchmark *b);

const char *benchmark_get_formula_name(Benchmark *b);
formula_t benchmark_get_budget_formula(Benchmark *b);

void benchmark_set_budget_formula(formula_t formula);
BenchInfo *benchmark_add_info(int vm_num, int task_num, int periodicity);
BenchInfo *benchmark_get_info(int vm_num, int task_num);
void set_array_index(int index);

#endif
