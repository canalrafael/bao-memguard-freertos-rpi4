#include "../inc/bench_functions.h"
#include <stdint.h>

#include "../inc/data.h"

#define array_size 128
volatile int a = 12345;
volatile int b = 6789;
volatile int c = 42;
volatile int d = 17;
volatile int array[array_size] = {
    53,  8,   27,  74,  19,  91,  42,  66,  3,   58,  95,  12,  81,  33,  60,
    4,   77,  23,  88,  49,  1,   63,  38,  99,  15,  70,  29,  84,  45,  9,
    51,  22,  78,  36,  93,  17,  68,  25,  80,  40,  7,   56,  31,  89,  14,
    73,  28,  85,  47,  6,   54,  21,  79,  34,  96,  11,  64,  26,  82,  43,
    5,   59,  32,  90,  16,  75,  24,  87,  48,  2,   52,  20,  76,  37,  94,
    18,  69,  30,  86,  46,  10,  62,  35,  97,  13,  71,  41,  67,  39,  92,
    57,  44,  100, 61,  83,  50,  72,  65,  55,  98,  110, 121, 105, 118, 102,
    127, 108, 115, 103, 123, 113, 107, 119, 101, 126, 104, 112, 122, 106, 117,
    111, 125, 109, 116, 114, 124, 120, 128};

// helper for some functions
void copy_array(const volatile int origin[], int *destiny, int size) {
  for (int i = 0; i < size; ++i) {
    destiny[i] = origin[i];
  }
}
////////////

volatile int sum_array(void) {
  int total = 0;
  for (int i = 0; i < array_size; ++i) {
    total += array[i];
  }
  return total;
}

volatile int subtract_array(void) {
  int total = array[0];
  for (int i = 1; i < array_size; ++i) {
    total -= array[i];
  }
  return total;
}

volatile int multiply_array(void) {
  int total = 1;
  for (int i = 0; i < array_size; ++i) {
    total *= array[i];
  }
  return total;
}

volatile int divide_array(void) {
  int total = array[0] ? array[0] : 1;
  for (int i = 1; i < array_size; ++i) {
    int v = array[i] ? array[i] : 1;
    total /= v;
  }
  return total;
}

volatile int mod_array(void) {
  int total = array[0];
  for (int i = 1; i < array_size; ++i) {
    int v = array[i] ? array[i] : 1;
    total %= v;
  }
  return total;
}

volatile int and_array(void) {
  int total = array[0];
  for (int i = 1; i < array_size; ++i) {
    total &= array[i];
  }
  return total;
}

volatile int or_array(void) {
  int total = array[0];
  for (int i = 1; i < array_size; ++i) {
    total |= array[i];
  }
  return total;
}

volatile int xor_array(void) {
  int total = array[0];
  for (int i = 1; i < array_size; ++i) {
    total ^= array[i];
  }
  return total;
}

volatile int sum_not_array(void) {
  int total = 0;
  for (int i = 0; i < array_size; ++i) {
    total += ~array[i];
  }
  return total;
}

volatile int sum_shl2_array(void) {
  int total = 0;
  for (int i = 0; i < array_size; ++i) {
    total += (array[i] << 2);
  }
  return total;
}

volatile int sum_shr1_array(void) {
  int total = 0;
  for (int i = 0; i < array_size; ++i) {
    total += (array[i] >> 1);
  }
  return total;
}

volatile int count_eq_next(void) {
  int cnt = 0;
  for (int i = 0; i < array_size - 1; ++i) {
    if (array[i] == array[i + 1])
      ++cnt;
  }
  return cnt;
}

volatile int count_gt_prev(void) {
  int cnt = 0;
  for (int i = 1; i < array_size; ++i) {
    if (array[i] > array[i - 1])
      ++cnt;
  }
  return cnt;
}

volatile int sum_of_squares_array(void) {
  int total = 0;
  for (int i = 0; i < array_size; ++i) {
    total += array[i] * array[i];
  }
  return total;
}

volatile int count_primes_array(void) {
  int total = 0;
  for (int i = 0; i < array_size; ++i) {
    int v = array[i];
    int is_prime = (v > 1);
    for (int d = 2; d * d <= v && is_prime; ++d) {
      if (v % d == 0)
        is_prime = 0;
    }
    total += is_prime;
  }
  return total;
}

////////////

void empty_wrapper() {
  while (1) {
    printf("\n\n\n\nthis should never happen\n\n\n\n");
  }
}

volatile int max_sum(void) {
  int m = array[0];
  for (int i = 1; i < array_size; ++i) {
    m = (m > array[i]) ? m : array[i];
  }
  return m;
}

volatile int min_sum(void) {
  int m = array[0];
  for (int i = 1; i < array_size; ++i) {
    m = (m < array[i]) ? m : array[i];
  }
  return m;
}

volatile int logical_and(void) {
  int r = 1;
  for (int i = 0; i < array_size; ++i) {
    r = r && (array[i] != 0);
  }
  return r;
}

volatile int logical_or(void) {
  int r = 0;
  for (int i = 0; i < array_size; ++i) {
    r = r || (array[i] != 0);
  }
  return r;
}

volatile int logical_not_equal(void) {
  int cnt = 0;
  for (int i = 0; i < array_size - 1; ++i) {
    cnt += (array[i] != array[i + 1]);
  }
  return cnt;
}

volatile int increment_a_1000_times() {
  int x = a;
  for (int i = 0; i < 1000; i++)
    x++;
  return x;
}
volatile int decrement_b_1000_times() {
  int x = b;
  for (int i = 0; i < 1000; i++)
    x--;
  return x;
}
volatile int factorial_15() {
  int x = 1;
  for (int i = 1; i <= 15; i++)
    x *= i;
  return x;
}
volatile int sum_1_to_1000() {
  int x = 0;
  for (int i = 1; i <= 1000; i++)
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

volatile int sum_of_squares_0_to_999() {
  int x = 0;
  for (int i = 0; i < 1000; i++)
    x += i * i;
  return x;
}
volatile int repeated_square_mod() {
  int x = a;
  for (int i = 0; i < 10; i++)
    x = x * x % 997;
  return x;
}
volatile int count_primes_below_1000() {
  int count = 0;
  for (int i = 2; i < 1000; i++) {
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
volatile int sum_of_cubes_1_to_100() {
  int x = 0;
  for (int i = 1; i <= 100; i++)
    x += i * i * i;
  return x;
}
volatile int xor_all_globals() {
  int sum = 0;
  for (int i = 0; i < array_size; ++i) {
    sum += a ^ b ^ c ^ d;
  }
  return sum;
}
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
volatile int my_q_sort(int v[], int left, int right) {
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

  return 0;
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

///////////////////
int *g_mem_ptr = NULL; // bandwidth
I2D *It = NULL;

void init_data() {
  // bandwitdh
  g_mem_ptr = (int *)malloc(BW_DEFAULT_ALLOC_SIZE);
  memset((char *)g_mem_ptr, 1, BW_DEFAULT_ALLOC_SIZE);
  for (uint64_t i = 0; i < BW_DEFAULT_ALLOC_SIZE / sizeof(int); i++) {
    g_mem_ptr[i] = i;
  }

  // mser
  int i, j, k;
  I2D *I;
  // I2D *It;
  int rows = 196, cols = 98;

  I = (I2D *)mserb;
  rows = I->height;
  cols = I->width;

  It = (I2D *)mserb1;

  k = 0;
  for (i = 0; i < cols; i++) {
    for (j = 0; j < rows; j++) {
      asubsref(It, k++) = subsref(I, j, i);
    }
  }
}

void free_data() {
  // bandwitdh
  free(g_mem_ptr);
}

void bandwidth_wrapper() {
  static volatile uint64_t sum = 0;
  sum += bench_read(g_mem_ptr);
  sum += bench_write(g_mem_ptr);

  return;
}

void dijkstra_wrapper() {
  int i, j, k;

  for (i = 0, j = NUM_NODES / 2; i < 100; i++, j++) {
    j = j % NUM_NODES;
    dijkstra(i, j);
  }

  return;
}

// -
void disparity_wrapper() {
  I2D *imleft = (I2D *)img1;
  I2D *imright = (I2D *)img2;
  int WIN_SZ = 8, SHIFT = 64;
  char signature[2] = {66, 77};
  short int bits_per_pixel = 24;

  // Check if images are valid BMP images.
  if (imleft->height <= 0 || imleft->width <= 0 || signature[0] != 'B' ||
      signature[1] != 'M' || (bits_per_pixel != 24 && bits_per_pixel != 8)) {
    return;
  }

  if (imright->height <= 0 || imright->width <= 0 || signature[0] != 'B' ||
      signature[1] != 'M' || (bits_per_pixel != 24 && bits_per_pixel != 8)) {
    return;
  }

  I2D *retDisparity = getDisparity(imleft, imright, WIN_SZ, SHIFT);
  return;
}

// -
void fft_wrapper() {
  static uint32_t A_re[NUM_POINTS];
  static uint32_t A_im[NUM_POINTS];
  static uint32_t W_re[NUM_POINTS / 2];
  static uint32_t W_im[NUM_POINTS / 2];

  int n = NUM_POINTS;
  init_array(n, A_re, A_im);
  compute_W(n, W_re, W_im);
  fft(n, A_re, A_im, W_re, W_im);
  permute_bitrev(n, A_re, A_im);

  return;
}

// -
void mser_wrapper() {
  //
  mser(It, 2);
  return;
}

// -
void qsort_wrapper() {
  struct my3DVertexStruct array[QSORT_MAXARRAY];

  int numRows = sizeof(qsort_input_data) / sizeof(qsort_input_data[0]);
  int count = 0;

  // Process the array instead of reading from a file
  for (int i = 0; i < numRows && count < QSORT_MAXARRAY; i++) {
    array[count].x = qsort_input_data[i][0];
    array[count].y = qsort_input_data[i][1];
    array[count].z = qsort_input_data[i][2];
    array[count].distance = (array[count].x * array[count].x) +
                            (array[count].y * array[count].y) +
                            (array[count].z * array[count].z);
    count++;
  }

  qsort(array, count, sizeof(struct my3DVertexStruct), qsort_compare);
  return;
}

void sha_wrapper() {
  SHA_INFO sha_info;
  sha_stream(&sha_info);
  return;
}

void sorting_wrapper() {
  int orig[MAX_SORTING], copy[MAX_SORTING], i;

  void *function[NUM_SORT] = {&selection_sort, &quick_sort,     &shell_sort,
                              &stdlib_qsort,   &insertion_sort, &bubble_sort};
  char *sort_name[NUM_SORT] = {"Selection sort", "Quicksort",
                               "Shellsort",      "Qsort",
                               "Insertion sort", "Bubble sort"};

  fill_array(orig, MAX_SORTING);

  for (i = 0; i < NUM_SORT; i++) {
    execute_sort(orig, copy, MAX_SORTING, sort_name[i], function[i]);
  }

  return;
}
