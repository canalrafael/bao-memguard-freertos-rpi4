#include "../inc/budget_function.h"

const char *get_budget_function() {
  switch (USED_BUDGET_FORMULA) {

  case EWMA_FORMULA:
    return "EWMA";
    break;
  case SW_FORMULA:
    return "SW";
    break;
  case AMBP_FORMULA:
    return "AMBP";
    break;
  case AFC_FORMULA:
    return "AFC";
    break;
  case LR_FORMULA:
    return "LR";
    break;
  case PIC_FORMULA:
    return "PIC";
    break;
  }
}
