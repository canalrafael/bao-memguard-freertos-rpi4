#include <bao.h>
#include <arch/sysregs.h>
#include <arch/generic_timer.h>
#include <interrupts.h>
#include <cpu.h>

#define MRS(v, r) __asm__ volatile("mrs %0, " #r : "=r" (v))
#define MSR(r, v) __asm__ volatile("msr " #r ", %0" : : "r" (v))
#define ISB()     __asm__ volatile("isb")

#define TIMMER_PERIOD 2000 // 2ms 
#define CNTHP_IRQ 26 // ID do timer fisico EL2

//eventos pmu
#define PMU_EVT_INST_RETIRED     0x08
#define PMU_EVT_L1D_CACHE_REFILL 0x03
#define PMU_EVT_BR_MIS_PRED      0x10

//mascara para habilitar o contador de ciclos e os contadores de eventos
#define PMU_ENABLE_ALL  ((1UL << 31) | (1 << 0) | (1 << 1) | (1 << 2))

// typedef struct {
//     uint64_t cpu_cycles;
//     uint64_t instuctions;
//     uint64_t branch_misses;
//     uint64_t cache_misses;
// } pmu_data_t;

volatile pmu_data_t g_pmu_data[PMU_MAX_CPUS];

// funcao para transformar microsegundos em ticks do timer
static uint64_t us_to_ticks(uint64_t us) {
    uint64_t freq;
    MRS(freq, cntfrq_el0); // sysreg_cntfrq_el0_read
    return (us * freq) / 1000000;
}

static void pmu_init_registers(void) {
    uint64_t val;

    // 1. Configurar o Tipo de Evento para cada Contador
    // Counter 0 -> Instructions
    MSR(pmevtyper0_el0, PMU_EVT_INST_RETIRED);
    // Counter 1 -> Cache Misses
    MSR(pmevtyper1_el0, PMU_EVT_L1D_CACHE_REFILL);
    // Counter 2 -> Branch Misses
    MSR(pmevtyper2_el0, PMU_EVT_BR_MIS_PRED);

    // 2. Habilitar os Contadores (Incluindo o de Ciclos - Bit 31)
    val = PMU_ENABLE_ALL;
    MSR(pmcntenset_el0, val);

    // 3. Resetar e Habilitar o PMU Globalmente (PMCR_EL0)
    // Bit 0 (E): Enable
    // Bit 1 (P): Reset Event Counters
    // Bit 2 (C): Reset Cycle Counter
    MRS(val, pmcr_el0);
    val |= (1 << 0) | (1 << 1) | (1 << 2); 
    MSR(pmcr_el0, val);
}

//leitura dos dados da pmu
static inline void pmu_collect_data(void) {
    uint64_t val;

    cpuid_t id = cpu()->id;
    //cpu cycles
    MRS(val, pmccntr_el0); // sysreg_pmccntr_el0_read
    g_pmu_data[id].cpu_cycles = val;

    //instrucoes
    asm volatile("msr pmselr_el0, %0" :: "r" (0));
    ISB();
    MRS(val, pmxevcntr_el0);
    g_pmu_data[id].instuctions = val;

    //cache misses
    asm volatile("msr pmselr_el0, %0" :: "r" (1));
    ISB();
    MRS(val, pmxevcntr_el0);
    g_pmu_data[id].cache_misses = val;

    //branch misses
    asm volatile("msr pmselr_el0, %0" :: "r" (2));
    ISB();
    MRS(val, pmxevcntr_el0);
    g_pmu_data[id].branch_misses = val;
}

void timer_handler(irqid_t irq_id) {
    // desabilita o timer temporariamente
    uint32_t ctl;
    MRS(ctl, cnthp_ctl_el2);
    ctl |= GENERIC_TIMER_IMASK; 
    MSR(cnthp_ctl_el2, ctl);

    // coleta os dados da PMU
    pmu_collect_data();

    // reprograma o proximo disparo
    uint64_t current_cnt;
    MRS(current_cnt, cntpct_el0);
    MSR(cnthp_cval_el2, current_cnt + us_to_ticks(TIMMER_PERIOD));

    // reabilita o timer
    ctl &= ~GENERIC_TIMER_IMASK;
    MSR(cnthp_ctl_el2, ctl);

    // limpa a gic
    interrupts_arch_clear(irq_id);
}

void timer_arch_init(void) {
    // inicializa os registradores da PMU
    pmu_init_registers();

    // habilita o timer fisico EL2
    uint32_t ctl = GENERIC_TIMER_ENABLE;
    MSR(cnthp_ctl_el2, ctl);

    // define o primeiro disparo
    uint64_t current_cnt;
    MRS(current_cnt, cntpct_el0);
    MSR(cnthp_cval_el2, current_cnt + us_to_ticks(TIMMER_PERIOD));

    interrupts_reserve(CNTHP_IRQ, timer_handler);
    interrupts_arch_enable(CNTHP_IRQ, true);
}

