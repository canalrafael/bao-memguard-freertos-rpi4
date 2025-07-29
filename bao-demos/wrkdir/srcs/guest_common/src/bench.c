#include "../inc/bench.h"
#include "../inc/bench_functions.h"
#include "../inc/regulation.h"

#include <stdbool.h>

// Global variables for all benchmark functions
#define NUM_BENCHMARKS 8
#define MAX_TASKS 4

bool init = false;

Function benchmark_functions[NUM_BENCHMARKS] = {
    {0, fft_wrapper, "fft_wrapper"},             // -
    {1, bandwidth_wrapper, "bandwidth_wrapper"}, // OK
    {2, bandwidth_wrapper, "bandwidth_wrapper"}, // OK
    {3, bandwidth_wrapper, "bandwidth_wrapper"}, // OK

    // {1, fft_wrapper, "fft_wrapper"},             // -
    // {2, sorting_wrapper, "sorting_wrapper"},     // OK
    // {3, mser_wrapper, "mser_wrapper"},           // -
    // {4, qsort_wrapper, "qsort_wrapper"},         // -
    // {5, dijkstra_wrapper, "dijkstra_wrapper"},   // OK
    // {6, sha_wrapper, "sha_wrapper"},             // OK
    // {7, disparity_wrapper, "disparity_wrapper"}, // -

    // {empty, "empty"},
    // {sum_array, "sum_array"},
    // {subtract_array, "subtract_array"},
    // {multiply_array, "multiply_array"},
    // {divide_array, "divide_array"},
    // {mod_array, "mod_array"},
    // {and_array, "and_array"},
    // {or_array, "or_array"},
    // {xor_array, "xor_array"},
    // {sum_not_array, "sum_not_array"},
    // {sum_shl2_array, "sum_shl2_array"},
    // {sum_shr1_array, "sum_shr1_array"},
    // {count_eq_next, "count_eq_next"},
    // {count_gt_prev, "count_gt_prev"},
    // {sum_of_squares_array, "sum_of_squares_array"},
    // {count_primes_array, "count_primes_array"},
    // {max_sum, "max_sum"},
    // {min_sum, "min_sum"},
    // {logical_and, "logical_and"},
    // {logical_or, "logical_or"},
    // {logical_not_equal, "logical_not_equal"},
    // {increment_a_1000_times, "increment_a_1000_times"},
    // {decrement_b_1000_times, "decrement_b_1000_times"},
    // {factorial_15, "factorial_15"},
    // {sum_1_to_1000, "sum_1_to_1000"},
    // {gcd_ab, "gcd_ab"},
    // {popcount_a, "popcount_a"},
    // {sum_mod7_0_to_999, "sum_mod7_0_to_999"},
    // {lcg_sequence, "lcg_sequence"},
    // {xorshift_a, "xorshift_a"},
    // {sum_of_squares_ab, "sum_of_squares_ab"},
    // {sum_of_squares_0_to_999, "sum_of_squares_0_to_999"},
    // {repeated_square_mod, "repeated_square_mod"},
    // {count_primes_below_1000, "count_primes_below_1000"},
    // {fibonacci_20, "fibonacci_20"},
    // {reverse_digits_a, "reverse_digits_a"},
    // {russian_peasant_mul, "russian_peasant_mul"},
    // {count_bits_a, "count_bits_a"},
    // {sum_of_cubes_1_to_100, "sum_of_cubes_1_to_100"},
    // {xor_all_globals, "xor_all_globals"},
    // {complex_bitwise, "complex_bitwise"},
    // {mixed_arithmetic_mod, "mixed_arithmetic_mod"},
    // {mixed_arithmetic_masked, "mixed_arithmetic_masked"},
    // {swap_ab_and_sum, "swap_ab_and_sum"},
    // {abs_a_bitwise, "abs_a_bitwise"},
    // {fizzbuzz_sum, "fizzbuzz_sum"},
    // {count_bits_in_b, "count_bits_in_b"},
    // {parity_a, "parity_a"},
    // {shift_mix_ab, "shift_mix_ab"},
    // {simple_parity_a, "simple_parity_a"},
    // {count_ones_in_a, "count_ones_in_a"},
    // {my_bubble_sort, "my_bubble_sort"},
    // {my_selection_sort, "my_selection_sort"},
    // {my_shell_sort, "my_shell_sort"},
    // {my_insertion_sort, "my_insertion_sort"},
    // {my_quick_sort, "my_quick_sort"},
};

BenchInfo benchmark_info[NUM_BENCHMARKS];
formula_t budget_formula = EWMA_FORMULA;

////////

int get_benchmark_index(int vm_num, int task_num) {
  int index =
      (vm_num * TASK_QUANTITY + task_num) + (MAX_TASKS * BENCH_ARRAY_INDEX);
  if (index < 0 || index >= NUM_BENCHMARKS) {
    printf("Invalid get_benchmark_index call %d\n", index);
    return 0;
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

  for (int vm_num = 0; vm_num < VM_QNT; ++vm_num) {
    for (int task_num = 0; task_num < TASK_QUANTITY; ++task_num) {
      int index = get_benchmark_index(vm_num, task_num);

      benchmark_info[index].function.index = -1;
      benchmark_info[index].function.name = "UNDEFINED";
      benchmark_info[index].function.pointer = (void *)MAX_INT;
      benchmark_info[index].task_num = -1;
      // benchmark_info[index].task_handle = (void *)MAX_INT;
      // benchmark_info[index].budget_formula = -1;
      benchmark_info[index].periodicity = 0;
      benchmark_info[index].task_overruns = -1;
      benchmark_info[index].task_underruns = -1;
    }
  }

  init = true;
}

void destroy_bench() {
  if (init) {
    free_data();
  }
}
