#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/select.h>

#define NAME "MYAREA"
#define CHECK_INTERVAL 100000  // 100ms

typedef struct {
    int dataStatus1;    
    int dataStatus2;    
    int exitRequest;
    char message1[128]; 
    char message2[128];
} SHARED_AREA;

int main() {
    int fd;
    SHARED_AREA *sharedBuffer;
    char message[128];
    int isProcess1 = 0;

    fd = shm_open(NAME, O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("shm_open failed");
        exit(EXIT_FAILURE);
    }
    ftruncate(fd, sizeof(SHARED_AREA));

    sharedBuffer = mmap(0, sizeof(SHARED_AREA), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (sharedBuffer == MAP_FAILED) {
        perror("mmap failed");
        exit(EXIT_FAILURE);
    }

    if (sharedBuffer->dataStatus1 == 0 && sharedBuffer->dataStatus2 == 0 && sharedBuffer->exitRequest == 0) {
        isProcess1 = 1;
        printf("Running as Process 1 (Type messages, 'STOP' to exit)\n");
    } else {
        isProcess1 = 0;
        printf("Running as Process 2 (Type messages, 'STOP' to exit)\n");
    }
    
    fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL) | O_NONBLOCK);

    while(1) {
        fd_set readfds;
        struct timeval tv;
        int retval;

        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);
        tv.tv_sec = 0;
        tv.tv_usec = CHECK_INTERVAL;

        retval = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &tv);

        if (retval == -1) {
            perror("select error");
        } else if (retval) {
            if (fgets(message, sizeof(message), stdin) != NULL) {
                message[strcspn(message, "\n")] = '\0';

                if (isProcess1) {
                    strcpy(sharedBuffer->message1, message);
                    sharedBuffer->dataStatus1 = 1;
                } else {
                    strcpy(sharedBuffer->message2, message);
                    sharedBuffer->dataStatus2 = 1;
                }

                if (strcmp(message, "STOP") == 0) {
                    sharedBuffer->exitRequest = 1;
                    break;
                }
            }
        }

        if (sharedBuffer->exitRequest) {
            printf("Exit requested. Exiting...\n");
            break;
        }

        if (isProcess1) {
            if (sharedBuffer->dataStatus2) {
                printf("\nReceived: %s\n", sharedBuffer->message2);
                sharedBuffer->dataStatus2 = 0;
            }
        } else {
            if (sharedBuffer->dataStatus1) {
                printf("\nReceived: %s\n", sharedBuffer->message1);
                sharedBuffer->dataStatus1 = 0;
            }
        }
    }

    munmap(sharedBuffer, sizeof(SHARED_AREA));
    close(fd);
    shm_unlink(NAME);
    return 0;
}
