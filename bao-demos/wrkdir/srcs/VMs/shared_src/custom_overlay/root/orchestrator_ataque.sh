#!/bin/sh

ATAQUES="spectre armageddon"

while true; do
    for ATAQUE in $ATAQUES; do
        echo "=================================================="
        echo "[VM1] Iniciando 40 MINUTOS de Ataque: $ATAQUE"
        
        for ciclo in $(seq 1 120); do
            /root/$ATAQUE > /dev/null 2>&1 &
            PID_ATAQUE=$!

            sleep 15

            kill -9 $PID_ATAQUE 2>/dev/null
            
            sleep 5
        done
    done
done