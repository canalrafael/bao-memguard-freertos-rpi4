#ifndef BUDGET_FUNCTIONS_H
#define BUDGET_FUNCTIONS_H

volatile int empty();
volatile int sum_ab();
volatile int subtract_ab();
volatile int multiply_ab();
volatile int divide_ab();
volatile int mod_ab();

volatile int and_ab();
volatile int or_ab();
volatile int xor_ab();
volatile int not_a();
volatile int shift_a_left_2();

volatile int shift_b_right_1();
volatile int equals_ab();
volatile int not_equals_ab();
volatile int greater_than_ab();
volatile int less_than_ab();

volatile int max_ab();
volatile int min_ab();
volatile int logical_and();
volatile int logical_or();
volatile int logical_not_equal();

volatile int increment_a_100_times();
volatile int decrement_b_100_times();
volatile int factorial_10();
volatile int sum_1_to_100();
volatile int gcd_ab();

volatile int popcount_a();
volatile int sum_mod7_0_to_999();
volatile int lcg_sequence();
volatile int xorshift_a();
volatile int sum_of_squares_ab();

volatile int sum_of_squares_0_to_99();
volatile int repeated_square_mod();
volatile int count_primes_below_100();
volatile int fibonacci_20();
volatile int reverse_digits_a();

volatile int russian_peasant_mul();
volatile int count_bits_a();
volatile int sum_of_cubes_1_to_10();
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
