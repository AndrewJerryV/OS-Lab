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
    char message[128];

    // Create shared memory object
    fd = shm_open(NAME, O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("shm_open failed");
        exit(EXIT_FAILURE);
    }

    // Configure shared memory size
    ftruncate(fd, sizeof(SHARED_AREA));

    // Map shared memory
    sharedBuffer = mmap(0, sizeof(SHARED_AREA), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (sharedBuffer == MAP_FAILED) {
        perror("mmap failed");
        exit(EXIT_FAILURE);
    }

    // Initialize shared data
    sharedBuffer->dataStatus = 0;
    sharedBuffer->exitRequest = 0;

    while(1) {
        printf("\nEnter message (STOP to exit): ");
        fgets(message, sizeof(message), stdin);
        message[strcspn(message, "\n")] = '\0'; // Remove newline

        // Write message to shared memory
        strcpy(sharedBuffer->message, message);
        sharedBuffer->dataStatus = 1;

        if (strcmp(message, "STOP") == 0) {
            sharedBuffer->exitRequest = 1;
            break;
        }

        // Wait for reader to process
        while(sharedBuffer->dataStatus == 1) {
            sleep(1);
        }
    }

    // Cleanup
    munmap(sharedBuffer, sizeof(SHARED_AREA));
    close(fd);
    shm_unlink(NAME);
    return 0;
}
