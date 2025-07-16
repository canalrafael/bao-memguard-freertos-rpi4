#include "../inc/bench.h"
#include "../inc/budget_functions.h"

// Global variables for all benchmark functions
#define NUM_BENCHMARKS 56
#define MAX_TASKS 8
#define BENCH_ARRAY_INDEX 3

Function benchmark_functions[NUM_BENCHMARKS] = {
    {empty, "empty"},
    {sum_ab, "sum_ab"},
    {subtract_ab, "subtract_ab"},
    {multiply_ab, "multiply_ab"},
    {divide_ab, "divide_ab"},
    {mod_ab, "mod_ab"},
    {and_ab, "and_ab"},
    {or_ab, "or_ab"},
    {xor_ab, "xor_ab"},
    {not_a, "not_a"},
    {shift_a_left_2, "shift_a_left_2"},
    {shift_b_right_1, "shift_b_right_1"},
    {equals_ab, "equals_ab"},
    {not_equals_ab, "not_equals_ab"},
    {greater_than_ab, "greater_than_ab"},
    {less_than_ab, "less_than_ab"},
    {max_ab, "max_ab"},
    {min_ab, "min_ab"},
    {logical_and, "logical_and"},
    {logical_or, "logical_or"},
    {logical_not_equal, "logical_not_equal"},
    {increment_a_100_times, "increment_a_100_times"},
    {decrement_b_100_times, "decrement_b_100_times"},
    {factorial_10, "factorial_10"},
    {sum_1_to_100, "sum_1_to_100"},
    {gcd_ab, "gcd_ab"},
    {popcount_a, "popcount_a"},
    {sum_mod7_0_to_999, "sum_mod7_0_to_999"},
    {lcg_sequence, "lcg_sequence"},
    {xorshift_a, "xorshift_a"},
    {sum_of_squares_ab, "sum_of_squares_ab"},
    {sum_of_squares_0_to_99, "sum_of_squares_0_to_99"},
    {repeated_square_mod, "repeated_square_mod"},
    {count_primes_below_100, "count_primes_below_100"},
    {fibonacci_20, "fibonacci_20"},
    {reverse_digits_a, "reverse_digits_a"},
    {russian_peasant_mul, "russian_peasant_mul"},
    {count_bits_a, "count_bits_a"},
    {sum_of_cubes_1_to_10, "sum_of_cubes_1_to_10"},
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
    {my_bubble_sort, "bubble_sort"},
    {my_selection_sort, "selection_sort"},
    {my_shell_sort, "shell_sort"},
    {my_insertion_sort, "insertion_sort"},
    {my_quick_sort, "quick_sort"},
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
  info.budget_formula = EWMA_FORMULA;
  info.function_index = index;
  // printf("BenchInfo{%d, %s, VM%d, T%d}\n", *(int *)info.function, info.name,
  // info.vm_num, info.task_num);
  return info;
}
