#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    int frame_number;
    int valid;
} PageTableEntry;

void shuffle(int *array, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

int main() {
    srand(time(0));
    int page_size, num_pages, num_frames;
    printf("Enter page size: ");
    scanf("%d", &page_size);
    printf("Enter number of pages: ");
    scanf("%d", &num_pages);
    printf("Enter number of frames: ");
    scanf("%d", &num_frames);

    PageTableEntry *page_table = (PageTableEntry *)malloc(num_pages * sizeof(PageTableEntry));
    for (int i = 0; i < num_pages; i++) {
        page_table[i].valid = 0;
        page_table[i].frame_number = -1;
    }

    if (num_pages <= num_frames) {
        int *frames = (int *)malloc(num_frames * sizeof(int));
        for (int i = 0; i < num_frames; i++) {
            frames[i] = i;
        }
        shuffle(frames, num_frames);
        for (int i = 0; i < num_pages; i++) {
            page_table[i].valid = 1;
            page_table[i].frame_number = frames[i];
        }
        free(frames);
    } 
	else {
        int *pages = (int *)malloc(num_pages * sizeof(int));
        for (int i = 0; i < num_pages; i++) 
            pages[i] = i;
        shuffle(pages, num_pages);
        int *frames = (int *)malloc(num_frames * sizeof(int));
        for (int i = 0; i < num_frames; i++) 
            frames[i] = i;
        shuffle(frames, num_frames);
        for (int i = 0; i < num_frames; i++) {
            int page = pages[i];
            page_table[page].valid = 1;
            page_table[page].frame_number = frames[i];
        }
        free(pages);
        free(frames);
    }

    int num_addresses;
    printf("Enter the number of logical addresses to generate: ");
    scanf("%d", &num_addresses);

    for (int i = 0; i < num_addresses; i++) {
        int logical_address = rand() % (num_pages * page_size);
        int page_number = logical_address / page_size;
        int offset = logical_address % page_size;

        printf("Logical Address: %d\n", logical_address);
        printf("Page Number: %d, Offset: %d\n", page_number, offset);

        if (page_number < 0 || page_number >= num_pages) 
            printf("Invalid page number.\n");
        else {
            PageTableEntry entry = page_table[page_number];
            if (entry.valid) {
                int physical_address = entry.frame_number * page_size + offset;
                printf("Physical Address: %d (Frame %d)\n", physical_address, entry.frame_number);
            } 
            else 
                printf("Page Fault: Page %d is not present in memory.\n", page_number);
        }
        printf("-------------------\n");
    }

    free(page_table);
    return 0;
}
