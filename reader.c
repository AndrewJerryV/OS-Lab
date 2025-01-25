#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

#define NAME "MYAREA"

typedef struct {
    int dataStatus;    /* 1=Data written, 0=Data read */
    int exitRequest;   /* 1=Exit Request */
    char message[128]; /* Message */
} SHARED_AREA;

int main() {
    int fd;
    SHARED_AREA *sharedBuffer;

    // Open existing shared memory
    fd = shm_open(NAME, O_RDWR, 0666);
    if (fd == -1) {
        perror("shm_open failed");
        exit(EXIT_FAILURE);
    }

    // Map shared memory
    sharedBuffer = mmap(0, sizeof(SHARED_AREA), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (sharedBuffer == MAP_FAILED) {
        perror("mmap failed");
        exit(EXIT_FAILURE);
    }

    while(1) {
        if (sharedBuffer->dataStatus == 1) {
            printf("\nReceived: %s\n", sharedBuffer->message);
            sharedBuffer->dataStatus = 0;
        }

        if (sharedBuffer->exitRequest == 1) {
            printf("Exit request received\n");
            break;
        }

        sleep(1); // Prevent busy waiting
    }

    // Cleanup
    munmap(sharedBuffer, sizeof(SHARED_AREA));
    close(fd);
    return 0;
}
