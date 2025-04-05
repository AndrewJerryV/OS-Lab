#include <stdio.h>
#include <stdlib.h>

#define MAX_BLOCKS 50

int blocks[MAX_BLOCKS] = {0}; // 0 = free, 1 = allocated

void resetBlocks() {
    for (int i = 0; i < MAX_BLOCKS; i++)
        blocks[i] = 0;
}

void sequentialAllocation() {
    int start, length;
    printf("\n[Sequential Allocation]\nEnter starting block and length of the file: ");
    scanf("%d %d", &start, &length);

    if (start + length > MAX_BLOCKS) {
        printf("Allocation exceeds disk size.\n");
        return;
    }

    for (int i = start; i < start + length; i++) {
        if (blocks[i] == 1) {
            printf("Block %d already allocated. Allocation failed.\n", i);
            return;
        }
    }

    for (int i = start; i < start + length; i++) 
        blocks[i] = 1;

    printf("File allocated from block %d to %d\n", start, start + length - 1);
}

void indexedAllocation() {
    int indexBlock, n;
    printf("\n[Indexed Allocation]\nEnter index block: ");
    scanf("%d", &indexBlock);

    if (blocks[indexBlock] == 1) {
        printf("Index block already allocated.\n");
        return;
    }

    printf("Enter number of data blocks: ");
    scanf("%d", &n);

    int dataBlocks[n];
    printf("Enter the block numbers:\n");
    for (int i = 0; i < n; i++) {
        scanf("%d", &dataBlocks[i]);
        if (blocks[dataBlocks[i]] == 1) {
            printf("Block %d already allocated. Allocation failed.\n", dataBlocks[i]);
            return;
        }
    }

    blocks[indexBlock] = 1;
    for (int i = 0; i < n; i++) 
        blocks[dataBlocks[i]] = 1;

    printf("File allocated with index block %d pointing to: ", indexBlock);
    for (int i = 0; i < n; i++) 
        printf("%d ", dataBlocks[i]);
    printf("\n");
}

void linkedAllocation() {
    int start, length;
    printf("\n[Linked Allocation]\nEnter starting block and number of blocks: ");
    scanf("%d %d", &start, &length);

    if (blocks[start] == 1) {
        printf("Starting block already allocated. Allocation failed.\n");
        return;
    }

    int count = 0, i = 0, blockList[MAX_BLOCKS];
    while (count < length && i < MAX_BLOCKS) {
        if (blocks[i] == 0) {
            blockList[count++] = i;
        }
        i++;
    }

    if (count < length) {
        printf("Not enough free blocks. Allocation failed.\n");
        return;
    }

    for (i = 0; i < length; i++) 
        blocks[blockList[i]] = 1;

    printf("File blocks linked as: ");
    for (i = 0; i < length; i++) 
        printf("%d -> ", blockList[i]);
    printf("NULL\n");
}

int main() {
    int choice;

    while (1) {
        printf("\nFile Allocation Strategies Simulation\n");
        printf("1. Sequential Allocation\n");
        printf("2. Indexed Allocation\n");
        printf("3. Linked Allocation\n");
        printf("4. Reset Disk\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: sequentialAllocation(); break;
            case 2: indexedAllocation(); break;
            case 3: linkedAllocation(); break;
            case 4: resetBlocks(); printf("Disk reset.\n"); break;
            case 5: exit(0);
            default: printf("Invalid choice!\n");
        }
    }
    return 0;
}
