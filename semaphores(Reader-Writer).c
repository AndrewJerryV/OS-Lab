#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

int shared_data = 0;
int readCount = 0;

sem_t rw_mutex; 
sem_t queue;    
pthread_mutex_t mutex; 

void *reader(void *arg) {
    while(1) {

        sem_wait(&queue);

        pthread_mutex_lock(&mutex);
        readCount++;
        if (readCount == 1) 
            sem_wait(&rw_mutex);
    
        pthread_mutex_unlock(&mutex);

        sem_post(&queue);

        printf("Reader read: %d\n", shared_data);
        sleep(1); 

        pthread_mutex_lock(&mutex);
        readCount--;
        if (readCount == 0) 
            sem_post(&rw_mutex);
        
        pthread_mutex_unlock(&mutex);

        sleep(2); 
    }
}

void *writer(void *arg) {
    for (int i = 0;; i++) {
        int item = i + 1;

        sem_wait(&queue);
        sem_wait(&rw_mutex); 

        sem_post(&queue);

        shared_data = item;
        printf("Writer wrote: %d\n", item);
        sem_post(&rw_mutex); 
        sleep(2); 
    }
}

int main() {
    pthread_t readers[3], writers[2];

    sem_init(&rw_mutex, 0, 1);
    sem_init(&queue, 0, 1);
    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < 3; i++)
        pthread_create(&readers[i], NULL, reader, NULL);

    for (int i = 0; i < 2; i++)
        pthread_create(&writers[i], NULL, writer, NULL);

    for (int i = 0; i < 3; i++)
        pthread_join(readers[i], NULL);
    for (int i = 0; i < 2; i++)
        pthread_join(writers[i], NULL);

    sem_destroy(&rw_mutex);
    sem_destroy(&queue);
    pthread_mutex_destroy(&mutex);

    return 0;
}
