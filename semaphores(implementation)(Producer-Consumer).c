#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>

#define BUFFER_SIZE 5
#define wait(s) while (s <= 0); s--
#define signal(s) s++

typedef struct {
    int buffer[BUFFER_SIZE];
    int in, out;
    int empty, full, mutex;
} Shared;

int main() {
    Shared *s = mmap(NULL, sizeof(Shared), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    if (s == MAP_FAILED) exit(1);

    s->empty = BUFFER_SIZE;
    s->full = 0;
    s->mutex = 1;
    s->in = s->out = 0;

    if (fork() == 0) {  // Producer
        for (int i = 0;; i++) {
            wait(s->empty);
            wait(s->mutex);
            s->buffer[s->in] = i;
            printf("Produced: %d\n", i);
            s->in = (s->in + 1) % BUFFER_SIZE;
            signal(s->mutex);
            signal(s->full);
            sleep(1);
        }
    } else {            // Consumer
        while (1) {
            wait(s->full);
            wait(s->mutex);
            int item = s->buffer[s->out];
            printf("Consumed: %d\n", item);
            s->out = (s->out + 1) % BUFFER_SIZE;
            signal(s->mutex);
            signal(s->empty);
            sleep(2);
        }
    }
}
