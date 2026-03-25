#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdint.h>
#include <unistd.h>

#define IPC_BASE_ADDR 0x70000000
#define IPC_SIZE 0x00010000
typedef struct {
    volatile uint32_t dump_request;
} IPC_Payload;

int main() {
    int fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (fd == -1) return -1;

    void *map_base = mmap(0, IPC_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, IPC_BASE_ADDR);
    IPC_Payload *payload = (IPC_Payload *) map_base;

    payload->dump_request = 1;

    while(payload->dump_request == 1) {
        usleep(500000); // Checa a cada meio segundo
    }

    munmap(map_base, IPC_SIZE);
    close(fd);
    return 0;
}