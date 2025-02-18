#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 5  

pthread_mutex_t forks[N]; 
sem_t room;               

void *philosopher(void *arg) {
    int id = *((int *)arg);
    while (1) {

        printf("Philosopher %d is thinking.\n", id);
        sleep(2); 

        sem_wait(&room);

        pthread_mutex_lock(&forks[id]);                   
        pthread_mutex_lock(&forks[(id + 1) % N]);           

        printf("Philosopher %d is eating.\n", id);
        sleep(1); 

        pthread_mutex_unlock(&forks[id]);
        pthread_mutex_unlock(&forks[(id + 1) % N]);

        sem_post(&room);
    }
}

int main() {
    pthread_t threads[N];
    int philosopher_ids[N];

    sem_init(&room, 0, N - 1);

    for (int i = 0; i < N; i++) {
        pthread_mutex_init(&forks[i], NULL);
    }

    for (int i = 0; i < N; i++) {
        philosopher_ids[i] = i;
        pthread_create(&threads[i], NULL, philosopher, &philosopher_ids[i]);
    }

    for (int i = 0; i < N; i++) {
        pthread_join(threads[i], NULL);
    }

    for (int i = 0; i < N; i++) {
        pthread_mutex_destroy(&forks[i]);
    }
    sem_destroy(&room);

    return 0;
}
