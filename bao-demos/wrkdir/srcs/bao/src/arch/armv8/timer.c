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

    //desabilita o PMU globalmente (PMCR.E = 0)
    MRS(val, pmcr_el0);
    val &= ~(1 << 0);
    MSR(pmcr_el0, val);

    //desabilita explicitamente os contadores individuais (Crucial para configurar Tipos)
    val = PMU_ENABLE_ALL;
    MSR(pmcntenclr_el0, val); 

    //configura o filtro de ciclos (0 = conta em todos os niveis de excecao permitidos)
    asm volatile("msr pmccfiltr_el0, %0" :: "r" (0));

    //configura contador de branch misses (Contador 0)
    asm volatile("msr pmselr_el0, %0" :: "r" (0));
    ISB();
    asm volatile("msr pmxevtyper_el0, %0" :: "r" (PMU_EVT_BR_MIS_PRED));

    //configura contador de cache misses (Contador 1)
    asm volatile("msr pmselr_el0, %0" :: "r" (1));
    ISB();
    asm volatile("msr pmxevtyper_el0, %0" :: "r" (PMU_EVT_L1D_CACHE_REFILL));

    //configura contador de instrucoes (Contador 3)
    //usando o contador 3 porque o 2 esta sendo usado para os ciclos
    asm volatile("msr pmselr_el0, %0" :: "r" (3));
    ISB();
    asm volatile("msr pmxevtyper_el0, %0" :: "r" (PMU_EVT_INST_RETIRED));

    //habilita os contadores
    val = PMU_ENABLE_ALL;
    MSR(pmcntenset_el0, val);

    //reset e enable global
    MRS(val, pmcr_el0);
    val |= (1 << 0) | (1 << 1) | (1 << 2); // Enable, Reset Counters, Reset Cycle
    MSR(pmcr_el0, val);
}

//leitura dos dados da pmu
static inline void pmu_collect_data(void) {
    uint64_t val;

    cpuid_t id = cpu()->id;
    if (id >= PMU_MAX_CPUS) return;

    //cpu cycles
    MRS(val, pmccntr_el0); // sysreg_pmccntr_el0_read
    g_pmu_data[id].cpu_cycles = val;

    //branch misses
    asm volatile("msr pmselr_el0, %0" :: "r" (0));
    ISB();
    MRS(val, pmxevcntr_el0);
    g_pmu_data[id].branch_misses = val;

    //cache misses
    asm volatile("msr pmselr_el0, %0" :: "r" (1));
    ISB();
    MRS(val, pmxevcntr_el0);
    g_pmu_data[id].cache_misses = val;

    //instrucoes
    asm volatile("msr pmselr_el0, %0" :: "r" (3));
    ISB();
    MRS(val, pmxevcntr_el0);
    g_pmu_data[id].instuctions = val;

    //timestamp
    MRS(val, cntpct_el0); 
    g_pmu_data[id].timestamp = val;

    //reseta os contadores para a proxima coleta
    uint64_t pmcr;
    MRS(pmcr, pmcr_el0);

    //mantem o bit 0 (enable) e seta os bits de reset (Eventos e Ciclos)
    pmcr |= (1 << 2) | (1 << 1); 
    
    MSR(pmcr_el0, pmcr);
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