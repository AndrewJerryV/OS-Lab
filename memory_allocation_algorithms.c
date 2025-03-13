#include <stdio.h>

void firstFit(int blockSize[], int m, int processSize[], int n) {
    int allocation[n], internalFragmentation = 0, externalFragmentation = 0;
    int blockAllocated[m];
    for (int j = 0; j < m; j++) 
        blockAllocated[j] = 0;
    
    for (int i = 0; i < n; i++)
        allocation[i] = -1;
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (blockSize[j] >= processSize[i]) {
                allocation[i] = j;
                blockAllocated[j] = 1;  
                blockSize[j] -= processSize[i];
                break;
            }
        }
    }

    for(int i = 0; i < m; i++) {
        if (blockAllocated[i]) 
            internalFragmentation += blockSize[i];
        else
            externalFragmentation += blockSize[i];
    }
    printf("\nFirst Fit Allocation:\n");
    printf("Process No\tProcess Size\tBlock No\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t\t%d\t\t", i + 1, processSize[i]);
        if (allocation[i] != -1)
            printf("%d", allocation[i] + 1);
        else
            printf("Not Allocated");
        printf("\n");
    }
    printf("Internal Fragmentation: %d\n", internalFragmentation);
    printf("External Fragmentation: %d\n", externalFragmentation);
}

void bestFit(int blockSize[], int m, int processSize[], int n) {
    int allocation[n], internalFragmentation = 0, externalFragmentation = 0;
    int blockAllocated[m];
    for (int j = 0; j < m; j++) 
        blockAllocated[j] = 0;

    for (int i = 0; i < n; i++)
        allocation[i] = -1;

    for (int i = 0; i < n; i++) {
        int bestIdx = -1;
        for (int j = 0; j < m; j++) {
            if (blockSize[j] >= processSize[i]) {
                if (bestIdx == -1 || blockSize[j] < blockSize[bestIdx])
                    bestIdx = j;
            }
        }
        if (bestIdx != -1) {
            allocation[i] = bestIdx;
            blockAllocated[bestIdx] = 1;  
            blockSize[bestIdx] -= processSize[i];
        }
    }
    for(int i = 0; i < m; i++) {
        if (blockAllocated[i]) 
            internalFragmentation += blockSize[i];
        else
            externalFragmentation += blockSize[i];
    }
    printf("\nBest Fit Allocation:\n");
    printf("Process No\tProcess Size\tBlock No\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t\t%d\t\t", i + 1, processSize[i]);
        if (allocation[i] != -1)
            printf("%d", allocation[i] + 1);
        else
            printf("Not Allocated");
        printf("\n");
    }
    printf("Internal Fragmentation: %d\n", internalFragmentation);
    printf("External Fragmentation: %d\n", externalFragmentation);
}

void worstFit(int blockSize[], int m, int processSize[], int n) {
    int allocation[n], internalFragmentation = 0, externalFragmentation = 0;
    int blockAllocated[m];
    for (int j = 0; j < m; j++) 
        blockAllocated[j] = 0;

    for (int i = 0; i < n; i++)
        allocation[i] = -1;

    for (int i = 0; i < n; i++) {
        int worstIdx = -1;
        for (int j = 0; j < m; j++) {
            if (blockSize[j] >= processSize[i]) {
                if (worstIdx == -1 || blockSize[j] > blockSize[worstIdx])
                    worstIdx = j;
            }
        }
        if (worstIdx != -1) {
            allocation[i] = worstIdx;
            blockAllocated[worstIdx] = 1;  
            blockSize[worstIdx] -= processSize[i];
        }
    }
    for(int i = 0; i < m; i++) {
        if (blockAllocated[i]) 
            internalFragmentation += blockSize[i];
        else
            externalFragmentation += blockSize[i];
    }
    printf("\nWorst Fit Allocation:\n");
    printf("Process No\tProcess Size\tBlock No\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t\t%d\t\t", i + 1, processSize[i]);
        if (allocation[i] != -1)
            printf("%d", allocation[i] + 1);
        else
            printf("Not Allocated");
        printf("\n");
    }
    printf("Internal Fragmentation: %d\n", internalFragmentation);
    printf("External Fragmentation: %d\n", externalFragmentation);
}

int main() {
    int m, n;

    printf("Enter number of memory blocks: ");
    scanf("%d", &m);
    int blockSize[m], blockSizeCopy[m];

    printf("Enter the size of each memory block: ");
    for (int i = 0; i < m; i++) {
        scanf("%d", &blockSize[i]);
        blockSizeCopy[i] = blockSize[i]; 
    }

    printf("Enter number of processes: ");
    scanf("%d", &n);
    int processSize[n];

    printf("Enter the size of each process: ");
    for (int i = 0; i < n; i++) 
        scanf("%d", &processSize[i]);

    firstFit(blockSize, m, processSize, n);

    for (int i = 0; i < m; i++) 
        blockSize[i] = blockSizeCopy[i]; 
    bestFit(blockSize, m, processSize, n);

    for (int i = 0; i < m; i++) 
        blockSize[i] = blockSizeCopy[i]; 
    worstFit(blockSize, m, processSize, n);

    return 0;
}
