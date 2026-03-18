#!/bin/sh

# Lista dos ataques que compilamos
ATAQUES="spectre armageddon"


while true; do
    for ATAQUE in $ATAQUES; do
        
        
        /root/$ATAQUE &
        PID_ATAQUE=$!

        # Aguarda 40 minutos (40 min * 60 seg = 2400 segundos)
        sleep 15

        kill -9 $PID_ATAQUE
        
        # Pausa de 5 segundos para o FreeRTOS transferir os dados via IPC com folga
        sleep 5
        
    done
done