#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid;
    pid = fork();
    if (pid < 0) 
        printf("Fork failed");
    else if (pid == 0) 
        printf("Child process: PID = %d, Parent PID = %d\n", getpid(), getppid());
    else {
        int status;
        wait(&status);
        printf("Parent process: PID = %d, Child PID = %d\n", getpid(), pid);
    }
    return 0;
}
