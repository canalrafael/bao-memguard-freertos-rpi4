#include "../inc/bench.h"
#include "../inc/bench_functions.h"

#include <stdio.h>

// Global variables for all benchmark functions
#define NUM_BENCHMARKS 56
#define MAX_TASKS 8

Function benchmark_functions[NUM_BENCHMARKS] = {
    {empty, "empty"},
    {sum_array, "sum_array"},
    {subtract_array, "subtract_array"},
    {multiply_array, "multiply_array"},
    {divide_array, "divide_array"},
    {mod_array, "mod_array"},
    {and_array, "and_array"},
    {or_array, "or_array"},
    {xor_array, "xor_array"},
    {sum_not_array, "sum_not_array"},
    {sum_shl2_array, "sum_shl2_array"},
    {sum_shr1_array, "sum_shr1_array"},
    {count_eq_next, "count_eq_next"},
    {count_gt_prev, "count_gt_prev"},
    {sum_of_squares_array, "sum_of_squares_array"},
    {count_primes_array, "count_primes_array"},
    {max_sum, "max_sum"},
    {min_sum, "min_sum"},
    {logical_and, "logical_and"},
    {logical_or, "logical_or"},
    {logical_not_equal, "logical_not_equal"},
    {increment_a_1000_times, "increment_a_1000_times"},
    {decrement_b_1000_times, "decrement_b_1000_times"},
    {factorial_15, "factorial_15"},
    {sum_1_to_1000, "sum_1_to_1000"},
    {gcd_ab, "gcd_ab"},
    {popcount_a, "popcount_a"},
    {sum_mod7_0_to_999, "sum_mod7_0_to_999"},
    {lcg_sequence, "lcg_sequence"},
    {xorshift_a, "xorshift_a"},
    {sum_of_squares_ab, "sum_of_squares_ab"},
    {sum_of_squares_0_to_999, "sum_of_squares_0_to_999"},
    {repeated_square_mod, "repeated_square_mod"},
    {count_primes_below_1000, "count_primes_below_1000"},
    {fibonacci_20, "fibonacci_20"},
    {reverse_digits_a, "reverse_digits_a"},
    {russian_peasant_mul, "russian_peasant_mul"},
    {count_bits_a, "count_bits_a"},
    {sum_of_cubes_1_to_100, "sum_of_cubes_1_to_100"},
    {xor_all_globals, "xor_all_globals"},
    {complex_bitwise, "complex_bitwise"},
    {mixed_arithmetic_mod, "mixed_arithmetic_mod"},
    {mixed_arithmetic_masked, "mixed_arithmetic_masked"},
    {swap_ab_and_sum, "swap_ab_and_sum"},
    {abs_a_bitwise, "abs_a_bitwise"},
    {fizzbuzz_sum, "fizzbuzz_sum"},
    {count_bits_in_b, "count_bits_in_b"},
    {parity_a, "parity_a"},
    {shift_mix_ab, "shift_mix_ab"},
    {simple_parity_a, "simple_parity_a"},
    {count_ones_in_a, "count_ones_in_a"},
    {my_bubble_sort, "my_bubble_sort"},
    {my_selection_sort, "my_selection_sort"},
    {my_shell_sort, "my_shell_sort"},
    {my_insertion_sort, "my_insertion_sort"},
    {my_quick_sort, "my_quick_sort"},
};

formula_t benchmark_formulas[NUM_BENCHMARKS] = {};

////////

int get_benchmark_index(int vm_num, int task_num) {
  int index = (vm_num * 2 + task_num) + (MAX_TASKS * BENCH_ARRAY_INDEX);
  if (index < 1 || index >= NUM_BENCHMARKS) {
    return 0;
  }
  return index;
}

void set_benchmark_formula(int vm_num, int task_num, formula_t formula) {
  int index = get_benchmark_index(vm_num, task_num);
  benchmark_formulas[index] = formula;
}

BenchInfo get_benchmark_info(int vm_num, int task_num) {
  BenchInfo info;
  int index = get_benchmark_index(vm_num, task_num);
  info.function = benchmark_functions[index];
  info.vm_num = vm_num;
  info.task_num = task_num;
  info.budget_formula = benchmark_formulas[index];
  info.function_index = index;
  // printf("BenchInfo{%d, %s, VM%d, T%d}\n", *(int *)info.function, info.name,
  // info.vm_num, info.task_num);
  return info;
}
