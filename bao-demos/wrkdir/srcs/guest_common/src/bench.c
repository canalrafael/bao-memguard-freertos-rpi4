#include "../inc/bench.h"
#include "../inc/budget_function.h"

// Global variables for all benchmark functions
#define NUM_BENCHMARKS 56
#define MAX_TASKS 8
#define BENCH_ARRAY_INDEX 5

volatile int a = 12345;
volatile int b = 6789;
volatile int c = 42;
volatile int d = 17;
const volatile int array[] = {
    53,  8,   27,  74,  19,  91,  42,  66,  3,   58,  95,  12,  81,  33,  60,
    4,   77,  23,  88,  49,  1,   63,  38,  99,  15,  70,  29,  84,  45,  9,
    51,  22,  78,  36,  93,  17,  68,  25,  80,  40,  7,   56,  31,  89,  14,
    73,  28,  85,  47,  6,   54,  21,  79,  34,  96,  11,  64,  26,  82,  43,
    5,   59,  32,  90,  16,  75,  24,  87,  48,  2,   52,  20,  76,  37,  94,
    18,  69,  30,  86,  46,  10,  62,  35,  97,  13,  71,  41,  67,  39,  92,
    57,  44,  100, 61,  83,  50,  72,  65,  55,  98,  110, 121, 105, 118, 102,
    127, 108, 115, 103, 123, 113, 107, 119, 101, 126, 104, 112, 122, 106, 117,
    111, 125, 109, 116, 114, 124, 120, 128};
#define array_size 128

void copy_array(const volatile int origin[], int *destiny, int size) {
  for (int i = 0; i < size; ++i) {
    destiny[i] = origin[i];
  }
}

// Function declarations
volatile int empty() { return array[0]; }
volatile int sum_ab() { return a + b; }
volatile int subtract_ab() { return a - b; }
volatile int multiply_ab() { return a * b; }
volatile int divide_ab() { return a / (b ? b : 1); }
volatile int mod_ab() { return a % (b ? b : 1); }

volatile int and_ab() { return a & b; }
volatile int or_ab() { return a | b; }
volatile int xor_ab() { return a ^ b; }
volatile int not_a() { return ~a; }
volatile int shift_a_left_2() { return a << 2; }

volatile int shift_b_right_1() { return b >> 1; }
volatile int equals_ab() { return (a == b); }
volatile int not_equals_ab() { return (a != b); }
volatile int greater_than_ab() { return (a > b); }
volatile int less_than_ab() { return (a < b); }

volatile int max_ab() { return (a > b) ? a : b; }
volatile int min_ab() { return (a < b) ? a : b; }
volatile int logical_and() { return (a > b && b > c); }
volatile int logical_or() { return (a < b || c < d); }
volatile int logical_not_equal() { return !(a == b); }

volatile int increment_a_100_times() {
  int x = a;
  for (int i = 0; i < 100; i++)
    x++;
  return x;
}
volatile int decrement_b_100_times() {
  int x = b;
  for (int i = 0; i < 100; i++)
    x--;
  return x;
}
volatile int factorial_10() {
  int x = 1;
  for (int i = 1; i <= 10; i++)
    x *= i;
  return x;
}
volatile int sum_1_to_100() {
  int x = 0;
  for (int i = 1; i <= 100; i++)
    x += i;
  return x;
}
volatile int gcd_ab() {
  int x = a, y = b;
  while (y) {
    int t = y;
    y = x % y;
    x = t;
  }
  return x;
}

volatile int popcount_a() {
  int x = a, cnt = 0;
  while (x) {
    cnt += x & 1;
    x >>= 1;
  }
  return cnt;
}
volatile int sum_mod7_0_to_999() {
  int x = 0;
  for (int i = 0; i < 1000; i++)
    x += i % 7;
  return x;
}
volatile int lcg_sequence() {
  int x = 1;
  for (int i = 0; i < 20; i++)
    x = (x * 3 + 1) % 1000;
  return x;
}
volatile int xorshift_a() {
  int x = a;
  x ^= x << 13;
  x ^= x >> 17;
  x ^= x << 5;
  return x;
}
volatile int sum_of_squares_ab() { return a * a + b * b; }

volatile int sum_of_squares_0_to_99() {
  int x = 0;
  for (int i = 0; i < 100; i++)
    x += i * i;
  return x;
}
volatile int repeated_square_mod() {
  int x = a;
  for (int i = 0; i < 10; i++)
    x = x * x % 997;
  return x;
}
volatile int count_primes_below_100() {
  int count = 0;
  for (int i = 2; i < 100; i++) {
    int p = 1;
    for (int j = 2; j * j <= i; j++)
      if (i % j == 0)
        p = 0;
    count += p;
  }
  return count;
}
volatile int fibonacci_20() {
  int f1 = 1, f2 = 1, t;
  for (int i = 3; i <= 20; i++) {
    t = f1 + f2;
    f1 = f2;
    f2 = t;
  }
  return f2;
}
volatile int reverse_digits_a() {
  int x = a, r = 0;
  while (x) {
    r = r * 10 + x % 10;
    x /= 10;
  }
  return r;
}

volatile int russian_peasant_mul() {
  int x = a, y = b, p = 0;
  while (y) {
    if (y & 1)
      p += x;
    x <<= 1;
    y >>= 1;
  }
  return p;
}
volatile int count_bits_a() {
  int x = a, count = 0;
  while (x) {
    count++;
    x >>= 1;
  }
  return count;
}
volatile int sum_of_cubes_1_to_10() {
  int x = 0;
  for (int i = 1; i <= 10; i++)
    x += i * i * i;
  return x;
}
volatile int xor_all_globals() { return a ^ b ^ c ^ d; }
volatile int complex_bitwise() { return (a & b) | (c ^ d); }

volatile int mixed_arithmetic_mod() { return ((a + b) * (c + d)) % 10000; }
volatile int mixed_arithmetic_masked() { return ((a - b) * (c - d)) & 0xFFFF; }
volatile int swap_ab_and_sum() {
  int x = a, y = b, tmp = x;
  x = y;
  y = tmp;
  return x + y;
}
volatile int abs_a_bitwise() {
  int x = a;
  int s = x >> (sizeof(int) * 8 - 1);
  return (x ^ s) - s;
}
volatile int fizzbuzz_sum() {
  int x = 0;
  for (int i = 1; i <= 100; i++)
    if (i % 3 == 0 || i % 5 == 0)
      x += i;
  return x;
}

volatile int count_bits_in_b() {
  int y = b, cnt = 0;
  while (y) {
    y &= (y - 1);
    cnt++;
  }
  return cnt;
}
volatile int parity_a() {
  int x = a, p = 0;
  while (x) {
    p ^= x & 1;
    x >>= 1;
  }
  return p;
}
volatile int shift_mix_ab() { return (a << 1) + (b >> 1); }
volatile int simple_parity_a() {
  int x = a, sum = 0;
  for (int i = 0; i < 16; i++)
    sum += (x >> i) & 1;
  return sum;
}
volatile int count_ones_in_a() {
  int x = a, cnt = 0;
  while (x) {
    cnt += x & 1;
    x >>= 1;
  }
  return cnt;
}

// void bubble_sort(int numbers[], int array_size);
// void selection_sort(int numbers[], int array_size);
// void shell_sort(int numbers[], int array_size);
// void insertion_sort(int numbers[], int array_size);
// void q_sort(int numbers[], int left, int right);
// void quick_sort(int numbers[], int array_size);

/* /\*----------------------------  function
 * ------------------------------------ */
/*  Name: bubble_sort */
/*  Purpose: Sort given array using bubble sort algorithm */
/*  Pre: Array a must be of atleast array_size */
/*  Post: Sorts the array a  */
/*  Arguments: a is the array that needs to be sorted and array_size is the size
 */
/*             of the array */
/*  Returns: void */
/*  Calls: None */
/*  Source: http://www.cs.princeton.edu/~ah/alg_anim/gawain-4.0/BubbleSort.html
 */
/*  ----------------------------------------------------------------------------*\/
 */
volatile int my_bubble_sort() {
  int i, j, tmp;
  int numbers[array_size];
  copy_array(array, numbers, array_size);

  for (i = 0; i < array_size; i++) {

    for (j = 0; j < (array_size - 1 - i); j++) {

      if (numbers[j + 1] < numbers[j]) {

        tmp = numbers[j];
        numbers[j] = numbers[j + 1];
        numbers[j + 1] = tmp;

      } // end if

    } // end for j

  } // end for i

  return 0;
}

/* /\*----------------------------  function
 * ------------------------------------ */
/*  Name: selection_sort */
/*  Purpose: Sort given array using selection sort algorithm  */
/*  Pre: Array must be of atleast array_size */
/*  Post: Sorts the array upto array_size */
/*  Arguments: numbers is the array that needs to be sorted and array_size is
 * the */
/*             index upto which the elements will be sorted */
/*  Returns: void */
/*  Calls: None */
/*  Source: http://linux.wku.edu/~lamonml/algor/sort/selection.html */
/*  ----------------------------------------------------------------------------*\/
 */
volatile int my_selection_sort() {
  int i, j;
  int min, temp;
  int numbers[array_size];
  copy_array(array, numbers, array_size);

  for (i = 0; i < array_size - 1; i++) {
    min = i;
    for (j = i + 1; j < array_size; j++) {
      if (numbers[j] < numbers[min])
        min = j;
    }
    temp = numbers[i];
    numbers[i] = numbers[min];
    numbers[min] = temp;
  }
  return 0;
}

/* /\*----------------------------  function
 * ------------------------------------ */
/*  Name: shell_sort */
/*  Purpose: Sort given array using shell sort algorithm */
/*  Pre: numbers must be of atleast array_size */
/*  Post: Sorts the array  */
/*  Arguments: number is the array that needs to be sorted and array_size is the
 */
/*             size of the array */
/*  Returns: void */
/*  Calls: None */
/*  Source: http://linux.wku.edu/~lamonml/algor/sort/shell.html */
/*  ----------------------------------------------------------------------------*\/
 */
volatile int my_shell_sort() {
  int i, j, increment, temp;
  int numbers[array_size];
  copy_array(array, numbers, array_size);

  increment = 3;
  while (increment > 0) {
    for (i = 0; i < array_size; i++) {
      j = i;
      temp = numbers[i];
      while ((j >= increment) && (numbers[j - increment] > temp)) {
        numbers[j] = numbers[j - increment];
        j = j - increment;
      }
      numbers[j] = temp;
    }
    if (increment / 2 != 0)
      increment = increment / 2;
    else if (increment == 1)
      increment = 0;
    else
      increment = 1;
  }
  return 0;
}

/* /\*----------------------------  function
 * ------------------------------------ */
/*  Name: insertion_sort */
/*  Purpose: Sort given array using Insertion sort algorithm */
/*  Pre: numbers must be atleast of array_size */
/*  Post: Sorts the array */
/*  Arguments: numbers is the array that needs to be sorted and array_size is
 * the */
/*             size of the array */
/*  Returns: void */
/*  Calls: None */
/*  Source: http://linux.wku.edu/~lamonml/algor/sort/insertion.html */
/*  ----------------------------------------------------------------------------*\/
 */
volatile int my_insertion_sort() {
  int i, j, index;
  int numbers[array_size];
  copy_array(array, numbers, array_size);

  for (i = 1; i < array_size; i++) {
    index = numbers[i];
    j = i;
    while ((j > 0) && (numbers[j - 1] > index)) {
      numbers[j] = numbers[j - 1];
      j = j - 1;
    }
    numbers[j] = index;
  }
  return 0;
}

/* /\*----------------------------  function
 * ------------------------------------ */
/*  Name: q_sort */
/*  Purpose: Sort array using quick sort algorithm */
/*  Pre: left must be less than right */
/*  Post: Breaks the array into half and calls sort function again recursively
 */
/*  Arguments: number is the array and left and right are the index up to which
 */
/*             array needs to be sorted */
/*  Returns: void */
/*  Calls: q_sort */
/*  Source: http://linux.wku.edu/~lamonml/algor/sort/quick.html */
/*  ----------------------------------------------------------------------------*\/
 */
void my_q_sort(int v[], int left, int right) {
  int pivot, l_hold, r_hold;

  l_hold = left;
  r_hold = right;
  pivot = v[left];
  while (left < right) {
    while ((v[right] >= pivot) && (left < right))
      right--;
    if (left != right) {
      v[left] = v[right];
      left++;
    }
    while ((v[left] <= pivot) && (left < right))
      left++;
    if (left != right) {
      v[right] = v[left];
      right--;
    }
  }
  v[left] = pivot;
  pivot = left;
  left = l_hold;
  right = r_hold;
  if (left < pivot)
    my_q_sort(v, left, pivot - 1);
  if (right > pivot)
    my_q_sort(v, pivot + 1, right);
}

/* /\*----------------------------  function
 * ------------------------------------ */
/*  Name: quick_sort */
/*  Purpose: To call the recursive q_sort method */
/*  Pre: numbers must be of atleast array_size */
/*  Post: Calls the q_sort method to sort the array */
/*  Arguments: number is the array that needs to be sorted and array_size is the
 */
/*             array size */
/*  Returns: void */
/*  Calls: q_sort */
/*  ----------------------------------------------------------------------------*\/
 */
volatile int my_quick_sort() {
  int numbers[array_size];
  copy_array(array, numbers, array_size);
  my_q_sort(numbers, 0, array_size - 1);
  return 0;
}

// Function pointer table and function names
const char *function_names[NUM_BENCHMARKS] = {"empty",
                                              "sum_ab",
                                              "subtract_ab",
                                              "multiply_ab",
                                              "divide_ab",
                                              "mod_ab",
                                              "and_ab",
                                              "or_ab",
                                              "xor_ab",
                                              "not_a",
                                              "shift_a_left_2",
                                              "shift_b_right_1",
                                              "equals_ab",
                                              "not_equals_ab",
                                              "greater_than_ab",
                                              "less_than_ab",
                                              "max_ab",
                                              "min_ab",
                                              "logical_and",
                                              "logical_or",
                                              "logical_not_equal",
                                              "increment_a_100_times",
                                              "decrement_b_100_times",
                                              "factorial_10",
                                              "sum_1_to_100",
                                              "gcd_ab",
                                              "popcount_a",
                                              "sum_mod7_0_to_999",
                                              "lcg_sequence",
                                              "xorshift_a",
                                              "sum_of_squares_ab",
                                              "sum_of_squares_0_to_99",
                                              "repeated_square_mod",
                                              "count_primes_below_100",
                                              "fibonacci_20",
                                              "reverse_digits_a",
                                              "russian_peasant_mul",
                                              "count_bits_a",
                                              "sum_of_cubes_1_to_10",
                                              "xor_all_globals",
                                              "complex_bitwise",
                                              "mixed_arithmetic_mod",
                                              "mixed_arithmetic_masked",
                                              "swap_ab_and_sum",
                                              "abs_a_bitwise",
                                              "fizzbuzz_sum",
                                              "count_bits_in_b",
                                              "parity_a",
                                              "shift_mix_ab",
                                              "simple_parity_a",
                                              "count_ones_in_a",
                                              "bubble_sort",
                                              "selection_sort",
                                              "shell_sort",
                                              "insertion_sort",
                                              "quick_sort"};

typedef volatile int (*benchmark_func)(void);

benchmark_func function_table[NUM_BENCHMARKS] = {empty,
                                                 sum_ab,
                                                 subtract_ab,
                                                 multiply_ab,
                                                 divide_ab,
                                                 mod_ab,
                                                 and_ab,
                                                 or_ab,
                                                 xor_ab,
                                                 not_a,
                                                 shift_a_left_2,
                                                 shift_b_right_1,
                                                 equals_ab,
                                                 not_equals_ab,
                                                 greater_than_ab,
                                                 less_than_ab,
                                                 max_ab,
                                                 min_ab,
                                                 logical_and,
                                                 logical_or,
                                                 logical_not_equal,
                                                 increment_a_100_times,
                                                 decrement_b_100_times,
                                                 factorial_10,
                                                 sum_1_to_100,
                                                 gcd_ab,
                                                 popcount_a,
                                                 sum_mod7_0_to_999,
                                                 lcg_sequence,
                                                 xorshift_a,
                                                 sum_of_squares_ab,
                                                 sum_of_squares_0_to_99,
                                                 repeated_square_mod,
                                                 count_primes_below_100,
                                                 fibonacci_20,
                                                 reverse_digits_a,
                                                 russian_peasant_mul,
                                                 count_bits_a,
                                                 sum_of_cubes_1_to_10,
                                                 xor_all_globals,
                                                 complex_bitwise,
                                                 mixed_arithmetic_mod,
                                                 mixed_arithmetic_masked,
                                                 swap_ab_and_sum,
                                                 abs_a_bitwise,
                                                 fizzbuzz_sum,
                                                 count_bits_in_b,
                                                 parity_a,
                                                 shift_mix_ab,
                                                 simple_parity_a,
                                                 count_ones_in_a,
                                                 my_bubble_sort,
                                                 my_selection_sort,
                                                 my_shell_sort,
                                                 my_insertion_sort,
                                                 my_quick_sort};

////////

int get_benchmark_index(int vm_num, int task_num) {
  int index = (vm_num * 2 + task_num) + (MAX_TASKS * BENCH_ARRAY_INDEX);
  if (index < 1 || index >= NUM_BENCHMARKS) {
    return 0;
  }
  return index;
}

BenchInfo get_benchmark_info(int vm_num, int task_num) {
  BenchInfo info;
  int index = get_benchmark_index(vm_num, task_num);
  info.function = function_table[index];
  info.name = function_names[index];
  info.vm_num = vm_num;
  info.task_num = task_num;
  info.budget_function = get_budget_function();
  // printf("BenchInfo{%d, %s, VM%d, T%d}\n", *(int *)info.function, info.name,
  // info.vm_num, info.task_num);
  return info;
}
