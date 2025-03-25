#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#define N 4

int A[N][N] = {
    {1, 2, 3},
    {5, 6, 7},
    {9, 10, 11},
};

int B[N][N] = {
    {1, 0, 0},
    {0, 1, 0},
    {0, 0, 1},
};

int C[N][N];

typedef struct {
    int row;
} ThreadData;

void* multiply_row(void* arg) {
    ThreadData* data = (ThreadData*) arg;
    int row = data->row;
    for (int j = 0; j < N; j++) {
        C[row][j] = 0;
        for (int k = 0; k < N; k++) 
            C[row][j] += A[row][k] * B[k][j];
    }
    free(data);
    return NULL;
}

int main() {
    pthread_t threads[N];
    for (int i = 0; i < N; i++) {
        ThreadData* data = (ThreadData*) malloc(sizeof(ThreadData));
        data->row = i;
        pthread_create(&threads[i], NULL, multiply_row, data);
    }

    for (int i = 0; i < N; i++) 
        pthread_join(threads[i], NULL);

    printf("Result matrix C:\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) 
            printf("%d ", C[i][j]);
        printf("\n");
    }
    return 0;
}
