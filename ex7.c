#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid;
    pid = fork();
    if (pid < 0) {
        perror("Fork failed");
        exit(-1);
    } 
    else if (pid == 0) {
        printf("Child process: PID = %d, Parent PID = %d\n", getpid(), getppid());
        execl("./ADD", "ADD", "10", "20", NULL);
        perror("exec failed");
        exit(-1);
    } 
    else {
        int status;
        waitpid(pid, &status, 0);
        printf("Child process completed");
    }
    return 0;
}
