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
typedef enum {
  EWMA_FORMULA = 0,
  SW_FORMULA = 1,
  AMBP_FORMULA = 2,
  AFC_FORMULA = 3,
  LR_FORMULA = 4,
  PIC_FORMULA = 5,
} formula_t;

const char *get_formula_name(formula_t formula);

// const char *get_current_formula_name();
// void set_current_formula(formula_t formula);

#endif
