#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdint.h>
#include <unistd.h>

#define IPC_BASE_ADDR 0x70000000
#define IPC_SIZE 0x00400000

typedef struct {
    volatile uint32_t dump_request;
} IPC_Payload;

int main() {
    int fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (fd == -1) {
        printf("[Linux] Erro ao abrir /dev/mem.\n");
        return -1;
    }

    void *map_base = mmap(0, IPC_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, IPC_BASE_ADDR);
    if (map_base == (void *) -1) {
        printf("[Linux] Erro no mapeamento de memoria.\n");
        close(fd);
        return -1;
    }

    IPC_Payload *payload = (IPC_Payload *) map_base;

    printf("\n[Linux] Avisando o FreeRTOS para imprimir os dados...\n");
    payload->dump_request = 1;

    while(payload->dump_request == 1) {
        usleep(100000);
    }

    printf("[Linux] O FreeRTOS confirmou que terminou de imprimir!\n");

    munmap(map_base, IPC_SIZE);
    close(fd);
    return 0;
}