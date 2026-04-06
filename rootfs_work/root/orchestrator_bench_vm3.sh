#!/bin/sh

# Começa do índice 5 (sorting)
b_idx=5
ciclo=1

get_bench() {
    case $1 in
        0) echo "bandwidth" ;;
        1) echo "dijkstra" ;;
        2) echo "disparity" ;;
        3) echo "fft" ;;
        4) echo "qsort" ;;
        5) echo "sha" ;;
        6) echo "sorting" ;;
    esac
}

echo "[VM3 - BENCHMARK] Iniciando coleta (Ordem Decrescente)..."

while true; do
    CURR_BENCH=$(get_bench $b_idx)
    
    echo "=========================================================="
    echo "[VM3] Ciclo $ciclo: Rodando $CURR_BENCH"
    echo "=========================================================="
    
    /root/benchmark $CURR_BENCH &
    BENCH_PID=$!

    sleep 15

    kill -9 $BENCH_PID
    
    b_idx=$(( (b_idx + 6) % 7 ))
    ciclo=$(( ciclo + 1 ))
    
    sleep 5
done