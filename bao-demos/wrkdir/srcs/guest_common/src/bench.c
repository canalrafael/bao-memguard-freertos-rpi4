#include "../inc/bench.h"
#include "../inc/bench_functions.h"
#include "../inc/regulation.h"

#include <stdbool.h>

// Global variables for all benchmark functions
#define NUM_BENCHMARKS 96
#define MAX_TASKS 8

bool init = false;

Function benchmark_functions[NUM_BENCHMARKS] = {
    // Scenario 1: High-load baseline
    {0, bandwidth_wrapper, "bandwidth_wrapper"},
    {1, disparity_wrapper, "disparity_wrapper"},
    {2, empty_wrapper, "empty"}, // Idle VM1
    {3, empty_wrapper, "empty"}, // Idle VM1
    {4, empty_wrapper, "empty"}, // Idle VM2
    {5, empty_wrapper, "empty"}, // Idle VM2
    {6, empty_wrapper, "empty"}, // Idle VM3
    {7, empty_wrapper, "empty"}, // Idle VM3

    // Scenario 2: Interference from one noisy neighbor
    {8, bandwidth_wrapper, "bandwidth_wrapper"},
    {9, disparity_wrapper, "disparity_wrapper"},
    {10, fft_wrapper, "fft_wrapper"},
    {11, mser_wrapper, "mser_wrapper"},
    {12, empty_wrapper, "empty"}, // Idle VM2
    {13, empty_wrapper, "empty"}, // Idle VM2
    {14, empty_wrapper, "empty"}, // Idle VM3
    {15, empty_wrapper, "empty"}, // Idle VM3

    // Scenario 3: One high-load and one low-load neighbor
    {16, bandwidth_wrapper, "bandwidth_wrapper"},
    {17, disparity_wrapper, "disparity_wrapper"},
    {18, fft_wrapper, "fft_wrapper"},
    {19, mser_wrapper, "mser_wrapper"},
    {20, qsort_wrapper, "qsort_wrapper"},
    {21, sorting_wrapper, "sorting_wrapper"},
    {22, empty_wrapper, "empty"}, // Idle VM3
    {23, empty_wrapper, "empty"}, // Idle VM3

    // Scenario 4: One high-load and two low-load neighbors
    {24, bandwidth_wrapper, "bandwidth_wrapper"},
    {25, disparity_wrapper, "disparity_wrapper"},
    {26, fft_wrapper, "fft_wrapper"},
    {27, mser_wrapper, "mser_wrapper"},
    {28, sha_wrapper, "sha_wrapper"},
    {29, dijkstra_wrapper, "dijkstra_wrapper"},
    {30, qsort_wrapper, "qsort_wrapper"},
    {31, sorting_wrapper, "sorting_wrapper"},

    // Scenario 5: Tipping-point test with three high-load VMs
    {32, bandwidth_wrapper, "bandwidth_wrapper"},
    {33, disparity_wrapper, "disparity_wrapper"},
    {34, fft_wrapper, "fft_wrapper"},
    {35, mser_wrapper, "mser_wrapper"},
    {36, qsort_wrapper, "qsort_wrapper"},
    {37, sha_wrapper, "sha_wrapper"},
    {38, empty_wrapper, "empty"}, // Idle VM3
    {39, empty_wrapper, "empty"}, // Idle VM3

    // Scenario 6: Full system saturation with mixed high-load tasks
    {40, bandwidth_wrapper, "bandwidth_wrapper"},
    {41, disparity_wrapper, "disparity_wrapper"},
    {42, fft_wrapper, "fft_wrapper"},
    {43, mser_wrapper, "mser_wrapper"},
    {44, qsort_wrapper, "qsort_wrapper"},
    {45, sha_wrapper, "sha_wrapper"},
    {46, dijkstra_wrapper, "dijkstra_wrapper"},
    {47, sorting_wrapper, "sorting_wrapper"},

    // Scenario 7: Inversion Test (low-load vm0 vs saturated system)
    {48, dijkstra_wrapper, "dijkstra_wrapper"},
    {49, sha_wrapper, "sha_wrapper"},
    {50, fft_wrapper, "fft_wrapper"},
    {51, mser_wrapper, "mser_wrapper"},
    {52, bandwidth_wrapper, "bandwidth_wrapper"},
    {53, disparity_wrapper, "disparity_wrapper"},
    {54, qsort_wrapper, "qsort_wrapper"},
    {55, sorting_wrapper, "sorting_wrapper"},

    // Scenario 8: Realistic Mix (chaotic, mixed workloads)
    {56, bandwidth_wrapper, "bandwidth_wrapper"},
    {57, sha_wrapper, "sha_wrapper"},
    {58, fft_wrapper, "fft_wrapper"},
    {59, qsort_wrapper, "qsort_wrapper"},
    {60, mser_wrapper, "mser_wrapper"},
    {61, dijkstra_wrapper, "dijkstra_wrapper"},
    {62, disparity_wrapper, "disparity_wrapper"},
    {63, sorting_wrapper, "sorting_wrapper"},

    // Scenario 9: Monoculture Saturation (bandwidth)
    {64, bandwidth_wrapper, "bandwidth_wrapper"},
    {65, bandwidth_wrapper, "bandwidth_wrapper"},
    {66, bandwidth_wrapper, "bandwidth_wrapper"},
    {67, bandwidth_wrapper, "bandwidth_wrapper"},
    {68, bandwidth_wrapper, "bandwidth_wrapper"},
    {69, bandwidth_wrapper, "bandwidth_wrapper"},
    {70, bandwidth_wrapper, "bandwidth_wrapper"},
    {71, bandwidth_wrapper, "bandwidth_wrapper"},

    // Scenario 10: Monoculture Saturation (fft)
    {72, fft_wrapper, "fft_wrapper"},
    {73, fft_wrapper, "fft_wrapper"},
    {74, fft_wrapper, "fft_wrapper"},
    {75, fft_wrapper, "fft_wrapper"},
    {76, fft_wrapper, "fft_wrapper"},
    {77, fft_wrapper, "fft_wrapper"},
    {78, fft_wrapper, "fft_wrapper"},
    {79, fft_wrapper, "fft_wrapper"},

    // Scenario 11: Monoculture Saturation (mser)
    {80, mser_wrapper, "mser_wrapper"},
    {81, mser_wrapper, "mser_wrapper"},
    {82, mser_wrapper, "mser_wrapper"},
    {83, mser_wrapper, "mser_wrapper"},
    {84, mser_wrapper, "mser_wrapper"},
    {85, mser_wrapper, "mser_wrapper"},
    {86, mser_wrapper, "mser_wrapper"},
    {87, mser_wrapper, "mser_wrapper"},

    // Scenario 12: Monoculture Saturation (disparity)
    {88, disparity_wrapper, "disparity_wrapper"},
    {89, disparity_wrapper, "disparity_wrapper"},
    {90, disparity_wrapper, "disparity_wrapper"},
    {91, disparity_wrapper, "disparity_wrapper"},
    {92, disparity_wrapper, "disparity_wrapper"},
    {93, disparity_wrapper, "disparity_wrapper"},
    {94, disparity_wrapper, "disparity_wrapper"},
    {95, disparity_wrapper, "disparity_wrapper"},
};

BenchInfo benchmark_info[NUM_BENCHMARKS];
formula_t budget_formula = 0;

////////

int get_benchmark_index(int vm_num, int task_num) {
  int index =
      (vm_num * TASK_QUANTITY + task_num) + (MAX_TASKS * BENCH_ARRAY_INDEX);
  if (index < 0 || index >= NUM_BENCHMARKS) {
    printf("Invalid get_benchmark_index call %d\n", index);
    return -1;
  }
  return index;
}

void set_budget_formula(formula_t formula) {
  if (formula < 0 || formula >= FORMULA_COUNT) {
    printf("Invalid set_benchmark_formula call %d\n", formula);
    return;
  }
  budget_formula = formula;
}

formula_t get_budget_formula() { return budget_formula; }

BenchInfo *get_benchmark_info(int vm_num, int task_num) {
  if (!init) {
    printf("not initialized\n");
    return NULL;
  }
  //
  int index = get_benchmark_index(vm_num, task_num);
  BenchInfo *benchInfo = &benchmark_info[index];
  if (benchInfo == NULL) {
    printf("NULL benchInfo\n");
  }
  return benchInfo;
}

BenchInfo *add_benchmark_info(int vm_num, int task_num, int periodicity) {
  BenchInfo info;
  int index = get_benchmark_index(vm_num, task_num);
  info.function = benchmark_functions[index];
  info.task_num = task_num;
  // info.task_handle = handler;
  // info.budget_formula = AFC_FORMULA;
  info.periodicity = periodicity;
  info.task_overruns = 0;
  info.task_underruns = 0;

  benchmark_info[index] = info;
  return &benchmark_info[index];
}

void init_bench() {
  init_data();

  for (int index = 0; index < NUM_BENCHMARKS; ++index) {
    benchmark_info[index].function.index = -1;
    benchmark_info[index].function.name = "UNDEFINED";
    benchmark_info[index].function.pointer = empty_wrapper;
    benchmark_info[index].task_num = -1;
    benchmark_info[index].periodicity = 0;
    benchmark_info[index].task_overruns = -1;
    benchmark_info[index].task_underruns = -1;
  }

  init = true;
}

void destroy_bench() {
  if (init) {
    free_data();
  }
}
