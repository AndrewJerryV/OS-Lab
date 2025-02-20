#include <stdio.h>

typedef struct {
    int id;     
    int size;   
} MemoryBlock;

typedef struct {
    int id;              
    int size;            
    int allocatedBlock;  
} Process;

void firstFit(MemoryBlock blocks[], int m, Process processes[], int n) {

    for (int i = 0; i < n; i++) 
        processes[i].allocatedBlock = -1;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (blocks[j].size >= processes[i].size) {
                processes[i].allocatedBlock = blocks[j].id;
                blocks[j].size -= processes[i].size;
                break; 
            }
        }
    }

    printf("\nProcess No\tProcess Size\tBlock\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t\t%d\t\t", processes[i].id, processes[i].size);
        if (processes[i].allocatedBlock != -1)
            printf("%d", processes[i].allocatedBlock);
        else
            printf("Not Allocated");
        printf("\n");
    }
}

int main() {

    MemoryBlock blocks[] = { {1, 100}, {2, 500}, {3, 200}, {4, 300}, {5, 600} };
    int m = sizeof(blocks) / sizeof(blocks[0]);

    Process processes[] = { {1, 212, -1}, {2, 417, -1}, {3, 112, -1}, {4, 426, -1} };
    int n = sizeof(processes) / sizeof(processes[0]);

    firstFit(blocks, m, processes, n);

    return 0;
}
