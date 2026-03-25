#!/bin/sh

trap 'kill -9 $BENCH_PID 2>/dev/null; exit 0' INT

get_bench() {
    case $1 in
        0) echo "bandwidth" ;; 1) echo "dijkstra" ;; 2) echo "disparity" ;;
        3) echo "fft" ;; 4) echo "qsort" ;; 5) echo "sha" ;; 6) echo "sorting" ;;
    esac
}

for b_idx in 0 1 2 3 4 5 6; do
    CURR_BENCH=$(get_bench $b_idx)
    echo "=================================================="
    echo "[VM2] Iniciando 40 MINUTOS de: $CURR_BENCH"
    
    for ciclo in $(seq 1 120); do
        /root/benchmark $CURR_BENCH > /dev/null 2>&1 &
        BENCH_PID=$!

        sleep 15

        kill -9 $BENCH_PID 2>/dev/null
        
        sleep 1
        
        /root/trigger_print
        
        sleep 2
    done
done