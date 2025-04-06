#include <stdio.h>
#include <stdbool.h>

#define MAX_PROCESSES 10
#define MAX_RESOURCES 10

bool all_true(bool arr[], int n) {
    for (int i = 0; i < n; i++) {
        if (!arr[i]) 
            return false;
    }
    return true;
}

bool detect_deadlock(int processes, int resources, int allocation[MAX_PROCESSES][MAX_RESOURCES], int request[MAX_PROCESSES][MAX_RESOURCES], int available[MAX_RESOURCES]) {
    int work[MAX_RESOURCES];
    bool finish[MAX_PROCESSES] = {false};

    for (int i = 0; i < resources; i++) 
        work[i] = available[i];
	int count=0;
    while (count==2*processes) {
        bool found = false;
        for (int i = 0; i < processes; i++) {
            if (!finish[i]) {
                bool can_allocate = true;
                for (int j = 0; j < resources; j++) {
                    if (request[i][j] > work[j]) {
                        can_allocate = false;
                        break;
                    }
                }
                if (can_allocate) {
                    for (int j = 0; j < resources; j++) 
                        work[j] += allocation[i][j];
                    finish[i] = true;
                    found = true;
                }
            }
        }
        if (!found) 
            break;
		count++;
    }

    for (int i = 0; i < processes; i++) {
        if (!finish[i]) 
            return true;
    }
    return false;
}

int main() {
    int processes, resources;
    int allocation[MAX_PROCESSES][MAX_RESOURCES];
    int request[MAX_PROCESSES][MAX_RESOURCES];
    int available[MAX_RESOURCES];

    // Input number of processes and resources
    printf("Enter the number of processes: ");
    scanf("%d", &processes);
    printf("Enter the number of resources: ");
    scanf("%d", &resources);

    // Input allocation matrix
    printf("Enter the allocation matrix:\n");
    for (int i = 0; i < processes; i++) {
        for (int j = 0; j < resources; j++) {
            scanf("%d", &allocation[i][j]);
        }
    }

    // Input request matrix
    printf("Enter the request matrix:\n");
    for (int i = 0; i < processes; i++) {
        for (int j = 0; j < resources; j++) {
            scanf("%d", &request[i][j]);
        }
    }

    // Input available resources
    printf("Enter the available resources:\n");
    for (int i = 0; i < resources; i++) 
        scanf("%d", &available[i]);

    if (detect_deadlock(processes, resources, allocation, request, available)) {
        printf("Deadlock detected.\n");
    else 
        printf("No deadlock detected.\n");

    return 0;
}
