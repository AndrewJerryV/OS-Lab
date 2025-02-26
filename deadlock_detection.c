#include <stdio.h>
#define MAX_PROCESSES 10
#define MAX_RESOURCES 10

int allocation[MAX_PROCESSES][MAX_RESOURCES];
int request[MAX_PROCESSES][MAX_RESOURCES];
int available[MAX_RESOURCES];
int resources[MAX_RESOURCES];
int work[MAX_RESOURCES];
int marked[MAX_PROCESSES];

int main() {
    int num_processes, num_resources;

    printf("Enter the number of processes: ");
    scanf("%d", &num_processes);

    printf("Enter the number of resources: ");
    scanf("%d", &num_resources);

    for (int i = 0; i < num_resources; i++) {
        printf("Enter the total amount of Resource R%d: ", i + 1);
        scanf("%d", &resources[i]);
    }

    printf("Enter the request matrix:\n");
    for (int i = 0; i < num_processes; i++) {
        for (int j = 0; j < num_resources; j++) 
            scanf("%d", &request[i][j]);
    }

    printf("Enter the allocation matrix:\n");
    for (int i = 0; i < num_processes; i++) {
        for (int j = 0; j < num_resources; j++) 
            scanf("%d", &allocation[i][j]);
    }

    for (int j = 0; j < num_resources; j++) {
        available[j] = resources[j];
        for (int i = 0; i < num_processes; i++) 
            available[j] -= allocation[i][j];
    }

    for (int i = 0; i < num_processes; i++) {
        int count = 0;
        for (int j = 0; j < num_resources; j++) {
            if (allocation[i][j] == 0) 
                count++;
            else 
                break;
        }
        if (count == num_resources) 
            marked[i] = 1;
    }

    for (int j = 0; j < num_resources; j++) 
        work[j] = available[j];

    for (int i = 0; i < num_processes; i++) {
        int can_be_processed = 1;
        if (marked[i] != 1) {
            for (int j = 0; j < num_resources; j++) {
                if (request[i][j] > work[j]) {
                    can_be_processed = 0;
                    break;
                }
            }
            if (can_be_processed) {
                marked[i] = 1;
                for (int j = 0; j < num_resources; j++) 
                    work[j] += allocation[i][j];
            }
        }
    }

    int deadlock = 0;
    for (int i = 0; i < num_processes; i++) {
        if (marked[i] != 1) {
            deadlock = 1;
            break;
        }
    }

    if (deadlock) 
        printf("Deadlock detected\n");
    else 
        printf("No deadlock possible\n");

    return 0;
}
