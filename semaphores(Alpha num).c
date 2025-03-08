#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t sem_num, sem_alpha;

void* put_numbers(void* arg) {
    int num=1;
    while(num) {
        sem_wait(&sem_num);
        printf("Added number: %d\n", num++);
        sem_post(&sem_alpha);
        sleep(1);
    }
}

void* put_alphabets(void* arg) {
    for (char ch = 'A'; ch <= 'Z'; ch++) {
        sem_wait(&sem_alpha);
        printf("Added alphabet: %c\n", ch);
        sem_post(&sem_num);
        sleep(1);
    }
}

int main() {
    pthread_t t1, t2, t3;
    sem_init(&sem_num, 0, 1);
    sem_init(&sem_alpha, 0, 0);

    pthread_create(&t1, NULL, put_numbers, NULL);
    pthread_create(&t2, NULL, put_alphabets, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    sem_destroy(&sem_num);
    sem_destroy(&sem_alpha);
    return 0;
}
