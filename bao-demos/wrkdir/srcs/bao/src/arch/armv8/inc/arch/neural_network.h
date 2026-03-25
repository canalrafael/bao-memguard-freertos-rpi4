#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H

#include <bao.h>
#include <cpu.h>
#include <arch/generic_timer.h>

void bao_run_interference_detection(cpuid_t cpu_id, volatile pmu_data_t *pmu);

#endif