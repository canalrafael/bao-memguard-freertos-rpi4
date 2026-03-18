#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "benchmarks/wrappers/inc/benchmarks_wrappers.h"

extern int g_bw_memory[];

int main(int argc, char *argv[]) {
    if (argc < 2) {
        return 1;
    }

    char *bench_name = argv[1];
    
    bandwidth_context_fann_t bw_ctx;
    bw_ctx.mem_ptr = g_bw_memory; 
    bw_ctx.sum = 0;

    while (1) {
        if (strcmp(bench_name, "bandwidth") == 0) {
            bandwidth_wrapper_fann(&bw_ctx);
        } else if (strcmp(bench_name, "dijkstra") == 0) {
            dijkstra_wrapper_fann();
        } else if (strcmp(bench_name, "disparity") == 0) {
            disparity_wrapper_fann(); 
        } else if (strcmp(bench_name, "fft") == 0) {
            fft_wrapper_fann();
        } else if (strcmp(bench_name, "qsort") == 0) {
            qsort_wrapper_fann();
        } else if (strcmp(bench_name, "sha") == 0) {
            sha_wrapper_fann();
        } else if (strcmp(bench_name, "sorting") == 0) {
            sorting_wrapper_fann();
        } else {
            printf("Benchmark '%s' desconhecido!\n", bench_name);
            return 1;
        }

        usleep(10000); 
    }

    return 0;
}