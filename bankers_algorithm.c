#include <stdio.h>
#include <stdbool.h>

#define NUMBER_OF_PROCESSES 5
#define NUMBER_OF_RESOURCES 3

int main() {

    int available[NUMBER_OF_RESOURCES] = {3, 3, 2};

    int maximum[NUMBER_OF_PROCESSES][NUMBER_OF_RESOURCES] = {
        {7, 5, 3},
        {3, 2, 2},
        {9, 0, 2},
        {2, 2, 2},
        {4, 3, 3}
    };

    int allocation[NUMBER_OF_PROCESSES][NUMBER_OF_RESOURCES] = {
        {0, 1, 0},
        {2, 0, 0},
        {3, 0, 2},
        {2, 1, 1},
        {0, 0, 2}
    };

    int need[NUMBER_OF_PROCESSES][NUMBER_OF_RESOURCES];
    for (int i = 0; i < NUMBER_OF_PROCESSES; i++) {
        for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
            need[i][j] = maximum[i][j] - allocation[i][j];
        }
    }

    printf("Need Matrix:\n");
    for (int i = 0; i < NUMBER_OF_PROCESSES; i++) {
        for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
            printf("%d ", need[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    bool finish[NUMBER_OF_PROCESSES] = {false};
    int safeSequence[NUMBER_OF_PROCESSES];
    int work[NUMBER_OF_RESOURCES];

    for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
        work[j] = available[j];
    }

    int count = 0;
    while (count < NUMBER_OF_PROCESSES) {
        bool found = false;
        for (int i = 0; i < NUMBER_OF_PROCESSES; i++) {
            if (!finish[i]) {
                bool canAllocate = true;
                for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
                    if (need[i][j] > work[j]) {
                        canAllocate = false;
                        break;
                    }
                }

                if (canAllocate) {
                    for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
                        work[j] += allocation[i][j];
                    }
                    safeSequence[count++] = i;
                    finish[i] = true;
                    printf("Process P%d has finished. New work: ", i);
                    for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
                        printf("%d ", work[j]);
                    }
                    printf("\n");
                    found = true;
                }
            }
        }

        if (!found) {
            break;
        }
    }

    if (count == NUMBER_OF_PROCESSES) {
        printf("\nSystem is in a SAFE state.\nSafe sequence: ");
        for (int i = 0; i < NUMBER_OF_PROCESSES; i++) {
            printf("P%d ", safeSequence[i]);
        }
        printf("\n");
    } else {
        printf("\nSystem is in an UNSAFE state! No safe sequence exists.\n");
    }
    return 0;
}
