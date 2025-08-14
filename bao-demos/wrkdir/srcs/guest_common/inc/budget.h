#ifndef BUDGET_FUNCTION_H
#define BUDGET_FUNCTION_H
//
// #define EWMA_FORMULA 0
// #define SW_FORMULA 1
// #define AMBP_FORMULA 2
// #define AFC_FORMULA 3
// #define LR_FORMULA 4
// #define PIC_FORMULA 5
// #define USED_BUDGET_FORMULA 5
//
#define FORMULA_COUNT 12
typedef enum {
  EWMA_FORMULA,
  SW_FORMULA,
  AFC_FORMULA,
  AMBP_FORMULA,
  PIC_FORMULA,
  LR_FORMULA, // <not implemented>
  //
  EWMA_V2_FORMULA,
  SW_V2_FORMULA,
  AFC_V2_FORMULA,
  LR_V2_FORMULA,
  AMBP_V2_FORMULA,
  PIC_V2_FORMULA,
  // FORMULA_COUNT,
} formula_t;

const char *get_formula_name(formula_t formula);

// const char *get_current_formula_name();
// void set_current_formula(formula_t formula);

#endif
