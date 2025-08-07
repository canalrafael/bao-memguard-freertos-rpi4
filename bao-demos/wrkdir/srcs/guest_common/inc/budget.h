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
#define FORMULA_COUNT 2
typedef enum {
  // LR_FORMULA, // not fully implemented in bao
  EWMA_V2_FORMULA,
  EWMA_FORMULA,
  // SW_FORMULA,
  // AFC_FORMULA,
  // AMBP_FORMULA,
  // PIC_FORMULA, // breaks the current logic, no idea how to fix
  // FORMULA_COUNT,
} formula_t;

const char *get_formula_name(formula_t formula);

// const char *get_current_formula_name();
// void set_current_formula(formula_t formula);

#endif
