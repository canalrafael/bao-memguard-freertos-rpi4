#include "../inc/budget.h"
#include <stdio.h>

// formula_t used_budget_formula = EWMA_FORMULA;

const char *get_formula_name(formula_t formula) {
  switch (formula) {
  case EWMA_FORMULA:
    return "EWMA";
    break;
  case SW_FORMULA:
    return "SW";
    break;
  case AFC_FORMULA:
    return "AFC";
    break;
    // case AMBP_FORMULA:
    //   return "AMBP";
    //   break;
    // case LR_FORMULA:
    //   return "LR";
    //   break;
    // case PIC_FORMULA:
    //   return "PIC";
    //   break;
    // default:
    //   printf("invalid formula name %d, returning UNDEFINED\n", formula);
    //   return "UNDEFINED";
    //   break;
  }
}

// formula_t get_current_formula() { return used_budget_formula; }
// void set_current_formula(formula_t formula) { used_budget_formula = formula;
// }
