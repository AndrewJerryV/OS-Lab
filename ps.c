#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int pid;             
    int arrival_time;   
    int burst_time;      
    int priority;        
    int completion_time; 
    int turnaround_time; 
    int waiting_time;    
    int response_time;   
} Process;

int compare(const void *a, const void *b) {
    Process *p = (Process*)a, *q = (Process*)b;
    return (p->arrival_time != q->arrival_time) ? 
           p->arrival_time - q->arrival_time : 
           p->priority - q->priority; 
}

void calculateTimes(Process p[], int n) {
    int current = 0, completed = 0;
    while(completed < n) {
        int next = -1, min_priority = 1e9;
        
        for(int i = 0; i < n; i++) {
            if(p[i].arrival_time <= current && p[i].waiting_time == -1 && 
               p[i].priority < min_priority) {
                min_priority = p[i].priority;
                next = i;
            }
        }
        
        if(next == -1) {
            int next_arrival = 1e9;
            for(int i = 0; i < n; i++) {
                if(p[i].waiting_time == -1 && p[i].arrival_time < next_arrival) 
                    next_arrival = p[i].arrival_time;
            }
            current = next_arrival;
        } 
        else {
            p[next].completion_time = current + p[next].burst_time;
            p[next].turnaround_time = p[next].completion_time - p[next].arrival_time;
            p[next].waiting_time = current - p[next].arrival_time;
            p[next].response_time = p[next].waiting_time; 
            current = p[next].completion_time;
            completed++;
        }
    }
}

int main() {
    int n;
    printf("Enter number of processes: ");
    scanf("%d", &n);
    Process p[n];
    
    for(int i = 0; i < n; i++) {
        printf("Process %d (PID, Arrival, Burst, Priority): ", i+1);
        scanf("%d%d%d%d", &p[i].pid, &p[i].arrival_time, 
              &p[i].burst_time, &p[i].priority);
        p[i].waiting_time = -1; 
    }

    qsort(p, n, sizeof(Process), compare);
    calculateTimes(p, n);

    float avg_turnaround = 0, avg_waiting = 0, avg_response = 0, avg_completion=0;
    printf("\n%-10s %-12s %-10s %-8s %-12s %-12s %-12s %-12s\n", "PID", "Arrival", "Burst", "Priority", "Response", "Completion", "Turnaround", "Waiting");
    for(int i = 0; i < n; i++) {
        printf("%-10d %-12d %-10d %-8d %-12d %-12d %-12d %-12d\n", 
              p[i].pid, 
              p[i].arrival_time,
              p[i].burst_time,
              p[i].priority,
              p[i].response_time,
              p[i].completion_time,
              p[i].turnaround_time,
              p[i].waiting_time
            );
        avg_turnaround += p[i].turnaround_time;
        avg_waiting += p[i].waiting_time;
        avg_response += p[i].response_time;
        avg_completion += p[i].completion_time;
    }

    printf("\nAverage:");
    printf("\nResponse Time:   %.2f", avg_response/n);
    printf("\nCompletion Time: %.2f", avg_completion/n);
    printf("\nTurnaround Time: %.2f", avg_turnaround/n);
    printf("\nWaiting Time:    %.2f\n", avg_waiting/n);
    return 0;
}
