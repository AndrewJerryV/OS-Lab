#include <stdio.h>
#include <stdbool.h>

void displayFrames(int frame[], int frames) {
    for (int i = 0; i < frames; i++) {
        if (frame[i] == -1)
            printf(" - ");
        else
            printf(" %d ", frame[i]);
    }
    printf("\n");
}

void fifo(int pages[], int n, int frames) {
    int frame[frames], front = 0, page_faults = 0;
    for (int i = 0; i < frames; i++) frame[i] = -1;
    
    printf("\nFIFO Page Replacement:\n");
    for (int i = 0; i < n; i++) {
        bool found = false;
        for (int j = 0; j < frames; j++) {
            if (frame[j] == pages[i]) {
                found = true;
                break;
            }
        }
        if (!found) {
            frame[front] = pages[i];
            front = (front + 1) % frames;
            page_faults++;
        }
        displayFrames(frame, frames);
    }
    printf("Total FIFO Page Faults: %d\n", page_faults);
}

void lru(int pages[], int n, int frames) {
    int frame[frames], recent[frames], page_faults = 0;
    for (int i = 0; i < frames; i++) frame[i] = -1;
    
    printf("\nLRU Page Replacement:\n");
    for (int i = 0; i < n; i++) {
        bool found = false;
        for (int j = 0; j < frames; j++) {
            if (frame[j] == pages[i]) {
                found = true;
                recent[j] = i;
                break;
            }
        }
        if (!found) {
            int lru_index = 0, min = 1e9;
            for (int j = 0; j < frames; j++) {
                if (frame[j] == -1) {
                    lru_index = j;
                    break;
                }
                if (recent[j] < min) {
                    min = recent[j];
                    lru_index = j;
                }
            }
            frame[lru_index] = pages[i];
            recent[lru_index] = i;
            page_faults++;
        }
        displayFrames(frame, frames);
    }
    printf("Total LRU Page Faults: %d\n", page_faults);
}

void lfu(int pages[], int n, int frames) {
    int frame[frames], freq[frames], recent[frames], page_faults = 0;
    for (int i = 0; i < frames; i++) {
        frame[i] = -1;
        recent[i] = -1;
        freq[i] = 0;
    }
    
    printf("\nLFU Page Replacement:\n");
    for (int i = 0; i < n; i++) {
        bool found = false;
        for (int j = 0; j < frames; j++) {
            if (frame[j] == pages[i]) {
                found = true;
                freq[j]++;
                recent[j] = i;
                break;
            }
        }
        if (!found) {
            int lfu_index = -1, min_freq = 1e9, oldest = 1e9;
            for (int j = 0; j < frames; j++) {
                if (frame[j] == -1) {
                    lfu_index = j;
                    break;
                }
                if (freq[j] < min_freq || (freq[j] == min_freq && recent[j] < oldest)) {
                    min_freq = freq[j];
                    oldest = recent[j];
                    lfu_index = j;
                }
            }
            frame[lfu_index] = pages[i];
            freq[lfu_index] = 1;
            recent[lfu_index] = i;
            page_faults++;
        }
        displayFrames(frame, frames);
    }
    printf("Total LFU Page Faults: %d\n", page_faults);
}

void mfu(int pages[], int n, int frames) {
    int frame[frames], freq[frames], fifo[frames], page_faults = 0;
    
    for (int i = 0; i < frames; i++) {
        frame[i] = -1;
        freq[i] = 0;
        fifo[i] = -1;
    }
    
    printf("\nMFU Page Replacement:\n");
    for (int i = 0; i < n; i++) {
        bool found = false;
        for (int j = 0; j < frames; j++) {
            if (frame[j] == pages[i]) {
                found = true;
                freq[j]++; 
                break;
            }
        }
        if (!found) {
            int mfu_index = -1;
            for (int j = 0; j < frames; j++) {
                if (frame[j] == -1) {
                    mfu_index = j;
                    break;
                }
            }
            if (mfu_index == -1) {
                int max_freq = -1, oldest = 99999;
                for (int j = 0; j < frames; j++) {
                    if (freq[j] > max_freq) {
                        max_freq = freq[j];
                        mfu_index = j;
                    }
                }
            }
            frame[mfu_index] = pages[i];
            freq[mfu_index] = 1;     
            page_faults++;
        }
        displayframes(frame, frames);
    }
    printf("Total MFU Page Faults: %d\n", page_faults);
}

void optimal(int pages[], int n, int frames) {
    int frame[frames], page_faults = 0;
    for (int i = 0; i < frames; i++) frame[i] = -1;
    printf("\nOptimal Page Replacement:\n");
    for (int i = 0; i < n; i++) {
        bool found = false;
        for (int j = 0; j < frames; j++) {
            if (frame[j] == pages[i]) {
                found = true;
                break;
            }
        }
        if (!found) {
            int pos = -1;
            for (int j = 0; j < frames; j++) {
                if (frame[j] == -1) {
                    pos = j;
                    break;
                }
            }
            if (pos == -1) {
                int farthest = -1;
                for (int j = 0; j < frames; j++) {
                    int k;
                    for (k = i + 1; k < n; k++) {
                        if (frame[j] == pages[k])
                            break;
                    }
                    if (k == n) {
                        pos = j;
                        break;
                    }
                    if (k > farthest) {
                        farthest = k;
                        pos = j;
                    }
                }
            }
            frame[pos] = pages[i];
            page_faults++;
        }
        displayframes(frame, frames);
    }
    printf("Total Optimal Page Faults: %d\n", page_faults);
}

int main() {
    int n, frames;
    printf("Enter number of pages: ");
    scanf("%d", &n);
    int pages[n];
    printf("Enter the page reference string: ");
    for (int i = 0; i < n; i++) scanf("%d", &pages[i]);
    printf("Enter number of frames: ");
    scanf("%d", &frames);
    
    fifo(pages, n, frames);
    lru(pages, n, frames);
    lfu(pages, n, frames);
    mfu(pages, n, frames);
    optimal(pages, n, frames);
    
    return 0;
}
