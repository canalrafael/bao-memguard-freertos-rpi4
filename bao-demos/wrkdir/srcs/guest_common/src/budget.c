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
  case AMBP_FORMULA:
    return "AMBP";
    break;
  // case LR_FORMULA:
  //   return "LR";
  //   break;
  case PIC_FORMULA:
    return "PIC";
    break;
  case EWMA_V2_FORMULA:
    return "EWMA_v2";
    break;
  case SW_V2_FORMULA:
    return "SW_v2";
    break;
  case AFC_V2_FORMULA:
    return "AFC_v2";
    break;
  case AMBP_V2_FORMULA:
    return "AMBP_v2";
    break;
  case LR_V2_FORMULA:
    return "LR_v2";
    break;
  case PIC_V2_FORMULA:
    return "PIC_v2";
    break;
  default:
    printf("invalid formula name %d, returning UNDEFINED\n", formula);
    return "UNDEFINED";
    break;
  }
}

// formula_t get_current_formula() { return used_budget_formula; }
// void set_current_formula(formula_t formula) { used_budget_formula = formula;
// }
