#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>

typedef struct {
    int value;
} semaphore;

void semaphore_wait(semaphore *s) {
    while (s->value <= 0);
    s->value--;
}

void semaphore_signal(semaphore *s) {
    s->value++;
}

int main() {
    semaphore *sem = mmap(NULL, sizeof(semaphore), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (sem == MAP_FAILED) {
        exit(1);
    }
    
    sem->value = 1; 
    
    pid_t pid = fork();
    if (pid < 0) {
        exit(1);
    }
    
    if (pid == 0) {
        semaphore_wait(sem);
        printf("Child process entered critical section.\n");
        sleep(2);
        printf("Child process leaving critical section.\n");
        semaphore_signal(sem);
        exit(0);
    } 
    else {
        semaphore_wait(sem);
        printf("Parent process entered critical section.\n");
        sleep(2);
        printf("Parent process leaving critical section.\n");
        semaphore_signal(sem);
        wait(NULL);
    }
    
    munmap(sem, sizeof(semaphore));
    return 0;
}
