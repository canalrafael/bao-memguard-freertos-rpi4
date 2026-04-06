#!/bin/sh

trap 'kill -9 $BENCH_PID 2>/dev/null; exit 0' INT
b_idx=0
ciclo=1

get_bench() {
    case $1 in
        0) echo "bandwidth" ;; 1) echo "dijkstra" ;; 2) echo "disparity" ;;
        3) echo "fft" ;; 4) echo "qsort" ;; 5) echo "sha" ;; 6) echo "sorting" ;;
    esac
}

while true; do
    CURR_BENCH=$(get_bench $b_idx)
    
    /root/benchmark $CURR_BENCH &
    BENCH_PID=$!

    # Aguarda 15 segundos para o nosso teste rápido
    sleep 15

    # Mata o benchmark para deixar a placa silenciosa
    kill -9 $BENCH_PID 2>/dev/null
    sleep 2
    
    # Executa o gatilho que acabamos de compilar!
    /root/trigger_print
    
    b_idx=$(( (b_idx + 1) % 7 ))
    ciclo=$(( ciclo + 1 ))
    sleep 5
done