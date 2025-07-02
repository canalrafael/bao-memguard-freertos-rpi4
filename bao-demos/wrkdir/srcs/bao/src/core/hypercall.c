/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#include <hypercall.h>
#include <cpu.h>
#include <vm.h>
#include <ipc.h>
#include <pmu.h>
#include <stdio.h>
#include <regulator.h>


long int hypercall(unsigned long id)
{
    long int ret = -HC_E_INVAL_ID;

    uint32_t arg0 = vcpu_readreg(cpu()->vcpu, HYPCALL_ARG_REG(0));
    uint32_t arg1 = vcpu_readreg(cpu()->vcpu, HYPCALL_ARG_REG(1));
    uint32_t arg2 = vcpu_readreg(cpu()->vcpu, HYPCALL_ARG_REG(2));
	uint32_t arg3 = vcpu_readreg(cpu()->vcpu, HYPCALL_ARG_REG(3));
	uint32_t arg4 = vcpu_readreg(cpu()->vcpu, HYPCALL_ARG_REG(4));

    switch(id)
	{		
	case HC_PMU_CONFIG_COUNTER:
		PMU_config_counter(arg0, arg1, arg2, arg3, arg4);
		break;
		
	case HC_PMU_START_COUNTER:
		PMU_start_counter(arg0);
		break;
		
	case HC_PMU_STOP_COUNTER:
		PMU_stop_counter(arg0);
		break;
		
	case HC_PMU_GET_COUNTER_VALUE:
		ret = PMU_get_counter_value(arg0);
		break;

	case HC_PMU_RESET_COUNTER:
		PMU_reset_counter(arg0);
		break;
		
	case HC_REGULATOR_BUDGET_DEPLETED:
		regulator_budget_depleted(arg0);
		break;

	case HC_REGULATOR_GET_NEW_BUDGET:
		ret = regulator_get_new_budget(arg0, arg1);
		break;

	
		
	/* case HC_REGULATOR_GET_TOTAL_CALCULATED_NEW_BUDGET: */
	/* 	ret = regulator_get_total_calculated_new_budget(arg0, arg1); */
	/* 	break; */

	case HC_REGULATOR_GET_TOTAL_USED_BUDGET:
		ret = regulator_get_total_used_budget(arg0, arg1);
		break;

	case HC_REGULATOR_GET_CURRENT_USED_BUDGET:
		ret = regulator_get_current_used_budget(arg0, arg1);
		break;
		
	case HC_IPC:
		ret = ipc_hypercall(arg0, arg1, arg2);
        break;

	default:
		WARNING("Unknown hypercall id %d", id);
    }

    return ret;
}
