#ifndef BENCH_FUNCTIONS_H
#define BENCH_FUNCTIONS_H

#include "../../benchmarks/bandwidth/inc/bandwidth.h"
#include "../../benchmarks/dijkstra/inc/dijkstra.h"
#include "../../benchmarks/disparity/inc/disparity.h"
#include "../../benchmarks/fft/inc/fft.h"
#include "../../benchmarks/mser/inc/mser.h"
#include "../../benchmarks/qsort/inc/qsort.h"
#include "../../benchmarks/sha/inc/sha.h"
#include "../../benchmarks/sorting/inc/sorting.h"

volatile int empty();

volatile int sum_array();
volatile int subtract_array();
volatile int multiply_array();
volatile int divide_array();
volatile int mod_array();

volatile int and_array();
volatile int or_array();
volatile int xor_array();
volatile int sum_not_array();
volatile int sum_shl2_array();

volatile int sum_shr1_array();
volatile int count_eq_next();
volatile int count_gt_prev();
volatile int sum_of_squares_array();
volatile int count_primes_array();

/////////////

volatile int max_sum();
volatile int min_sum();
volatile int logical_and();
volatile int logical_or();
volatile int logical_not_equal();

volatile int increment_a_1000_times();
volatile int decrement_b_1000_times();
volatile int factorial_15();
volatile int sum_1_to_1000();
volatile int gcd_ab();

volatile int popcount_a();
volatile int sum_mod7_0_to_999();
volatile int lcg_sequence();
volatile int xorshift_a();
volatile int sum_of_squares_ab();

volatile int sum_of_squares_0_to_999();
volatile int repeated_square_mod();
volatile int count_primes_below_1000();
volatile int fibonacci_20();
volatile int reverse_digits_a();

volatile int russian_peasant_mul();
volatile int count_bits_a();
volatile int sum_of_cubes_1_to_100();
volatile int xor_all_globals();
volatile int complex_bitwise();

volatile int mixed_arithmetic_mod();
volatile int mixed_arithmetic_masked();
volatile int swap_ab_and_sum();
volatile int abs_a_bitwise();
volatile int fizzbuzz_sum();

volatile int count_bits_in_b();
volatile int parity_a();
volatile int shift_mix_ab();
volatile int simple_parity_a();
volatile int count_ones_in_a();

volatile int my_bubble_sort();
volatile int my_selection_sort();
volatile int my_shell_sort();
volatile int my_insertion_sort();
volatile int my_q_sort(int v[], int left, int right);
volatile int my_quick_sort();
#endif

///////

volatile int bandwidth_wrapper();
volatile int dijkstra_wrapper();
volatile int disparity_wrapper();
volatile int fft_wrapper();
volatile int mser_wrapper();
volatile int qsort_wrapper();
volatile int sha_wrapper();
volatile int sorting_wrapper();

void init_data();
void free_data();
