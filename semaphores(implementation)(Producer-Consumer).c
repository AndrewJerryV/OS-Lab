#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>

#define BUFFER_SIZE 5

typedef struct {
    int value;
} semaphore;

typedef struct {
    int buffer[BUFFER_SIZE];
    int in;
    int out;
} shared_data;

void semaphore_wait(semaphore *s) {
    while (s->value <= 0);
    s->value--;
}

void semaphore_signal(semaphore *s) {
    s->value++;
}

int main() {
    semaphore *empty = mmap(NULL, sizeof(semaphore), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    semaphore *full = mmap(NULL, sizeof(semaphore), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    semaphore *mutex = mmap(NULL, sizeof(semaphore), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    shared_data *data = mmap(NULL, sizeof(shared_data), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    
    if (empty == MAP_FAILED || full == MAP_FAILED || mutex == MAP_FAILED || data == MAP_FAILED) {
        perror("mmap failed");
        exit(1);
    }
    
    empty->value = BUFFER_SIZE;
    full->value = 0;
    mutex->value = 1;
    data->in = 0;
    data->out = 0;
    
    pid_t pid = fork();
    if (pid < 0) {
        exit(1);
    }
    
    if (pid == 0) {
        int i = 0; 
        while(1) {
            semaphore_wait(empty);
            semaphore_wait(mutex);
            
            data->buffer[data->in] = i++;
            printf("Produced: %d\n", i);
            data->in = (data->in + 1) % BUFFER_SIZE;
            
            semaphore_signal(mutex);
            semaphore_signal(full);
            
            sleep(1);
        }
        exit(0);
    } 
    else { 
        while(1) {
            semaphore_wait(full);
            semaphore_wait(mutex);
            
            int item = data->buffer[data->out];
            printf("Consumed: %d\n", item);
            data->out = (data->out + 1) % BUFFER_SIZE;
            
            semaphore_signal(mutex);
            semaphore_signal(empty);
            
            sleep(2);
        }
        wait(NULL);
    }
    
    munmap(empty, sizeof(semaphore));
    munmap(full, sizeof(semaphore));
    munmap(mutex, sizeof(semaphore));
    munmap(data, sizeof(shared_data));
    
    return 0;
}
