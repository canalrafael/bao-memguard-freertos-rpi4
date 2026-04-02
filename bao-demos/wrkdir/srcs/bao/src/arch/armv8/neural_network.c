#include <bao.h>
#include <cpu.h>
#include <math.h>
#include <arch/pesos_treinados.h>
#include <arch/generic_timer.h>

#define NORM_BRANCH      100000000.0f
#define NORM_CACHE       50000000.0f 
#define NORM_INSTR       200000000.0f
#define NORM_CYCLES      400000000.0f
#define NORM_TIMESTAMP   1000000.0f

#define WINDOW_SIZE 5
#define METRICS 5
#define TOTAL_INPUTS (WINDOW_SIZE * METRICS) // 25
#define NUM_HIDDEN1 16
#define NUM_HIDDEN2 8
#define NUM_OUTPUT 1

static float sliding_window_buffer[PMU_MAX_CPUS][WINDOW_SIZE][METRICS];
static int window_index[PMU_MAX_CPUS] = {0};
static int window_filled[PMU_MAX_CPUS] = {0};
static uint64_t last_timestamp[PMU_MAX_CPUS] = {0};

static float baremetal_expf(float x) {
    //limites de seguranca para evitar underflow/overflow na FPU
    if (x <= -15.0f) return 0.0f;
    if (x >= 15.0f) return 3269017.0f;
    
    //calcula e^x usando a aproximacao de (1 + x/n)^n com n=1024
    float d = 1.0f + (x / 1024.0f);
    d *= d; d *= d; d *= d; d *= d; // ^16
    d *= d; d *= d; d *= d; d *= d; // ^256
    d *= d; d *= d;                 // ^1024
    return d;
}

//funcoes de ativacao identicas a da fann usando o baremetal_expf()
static float fann_sigmoid_symmetric(float x) {
    return -1.0f + (2.0f / (1.0f + baremetal_expf(-x)));
}

static float fann_sigmoid(float x) {
    return 1.0f / (1.0f + baremetal_expf(-x));
}

static float run_mlp_inference(float *inputs) {
    float hidden1[NUM_HIDDEN1];
    float hidden2[NUM_HIDDEN2];
    float out;
    int p = 0;

    //hidden layer 1
    for (int i = 0; i < NUM_HIDDEN1; i++) {
        hidden1[i] = pesos_iniciais[p++]; 
        for (int j = 0; j < TOTAL_INPUTS; j++) {
            hidden1[i] += pesos_iniciais[p++] * inputs[j];
        }
        hidden1[i] = fann_sigmoid_symmetric(hidden1[i]);
    }

    //hidden layer 2
    for (int i = 0; i < NUM_HIDDEN2; i++) {
        hidden2[i] = pesos_iniciais[p++]; 
        for (int j = 0; j < NUM_HIDDEN1; j++) {
            hidden2[i] += pesos_iniciais[p++] * hidden1[j];
        }
        hidden2[i] = fann_sigmoid_symmetric(hidden2[i]);
    }

    //chamada de saida
    out = pesos_iniciais[p++]; 
    for (int j = 0; j < NUM_HIDDEN2; j++) {
        out += pesos_iniciais[p++] * hidden2[j];
    }
    return fann_sigmoid(out);
}



void bao_run_interference_detection(cpuid_t cpu_id, volatile pmu_data_t *pmu) {
    
    //calcula o tempo que passou desde a ultima coleta
    uint64_t current_ts = pmu->timestamp;
    uint64_t delta_cycles;

    if (last_timestamp[cpu_id] == 0) delta_cycles = 0;
    else delta_cycles = current_ts - last_timestamp[cpu_id];

    last_timestamp[cpu_id] = current_ts;

    //frequencia do timer = sysreg_cntfrq_el0
    uint64_t freq;
    __asm__ volatile("mrs %0, cntfrq_el0" : "=r" (freq));
    float delta_ms = (delta_cycles * 1000.0f) / (float)freq;

    //coloca os dados na janela
    int idx = window_index[cpu_id];
    
    sliding_window_buffer[cpu_id][idx][0] = (float)pmu->branch_misses / NORM_BRANCH;
    sliding_window_buffer[cpu_id][idx][1] = (float)pmu->cache_misses / NORM_CACHE;
    sliding_window_buffer[cpu_id][idx][2] = (float)pmu->instructions / NORM_INSTR; 
    sliding_window_buffer[cpu_id][idx][3] = (float)pmu->cpu_cycles / NORM_CYCLES;
    sliding_window_buffer[cpu_id][idx][4] = delta_ms / NORM_TIMESTAMP;

    //atualiza o indice circular
    window_index[cpu_id] = (idx + 1) % WINDOW_SIZE;
    if (window_index[cpu_id] == 0) {
        window_filled[cpu_id] = 1;
    }

    //roda se a janela estiver cheia
    if (window_filled[cpu_id]) {
        float flattened_input[TOTAL_INPUTS];
        int flat_idx = 0;

        //achata a matrix 5x5 em um vetor linear com a ordem temporal correta
        for (int i = 0; i < WINDOW_SIZE; i++) {
            int circular_idx = (window_index[cpu_id] + i) % WINDOW_SIZE;
            for (int m = 0; m < METRICS; m++) {
                flattened_input[flat_idx++] = sliding_window_buffer[cpu_id][circular_idx][m];
            }
        }

        //executa a predicao
        float is_attack = run_mlp_inference(flattened_input);

        int frac_part = (int)(is_attack * 10000.0f); 
        
        printk("VM %d | NN: %d\n", cpu_id, frac_part);
        
        if (is_attack > 0.85f) { //ataque detectado
        }
    }
}