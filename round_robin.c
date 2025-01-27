#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int pid;             
    int arrival_time;   
    int burst_time;      
    int remaining_time;  
    int completion_time; 
    int turnaround_time; 
    int waiting_time;    
    int response_time;   
    int is_started;      
} Process;

int compare(const void *a, const void *b) {
    Process *p = (Process*)a, *q = (Process*)b;
    return p->arrival_time - q->arrival_time;
}

void calculateTimes(Process p[], int n, int time_quantum) {
    int current_time = 0;
    int completed = 0;
    int queue[n * n]; 
    int front = 0, rear = 0;

    for (int i = 0; i < n; i++) {
        p[i].remaining_time = p[i].burst_time;
        p[i].is_started = 0;
    }

    for (int i = 0; i < n; i++) {
        if (p[i].arrival_time <= current_time) {
            queue[rear++] = i;
        }
    }

    while (completed < n) {
        if (front >= rear) {
            int next_arrival = 1e9;
            for (int i = 0; i < n; i++) {
                if (p[i].remaining_time > 0 && p[i].arrival_time < next_arrival) {
                    next_arrival = p[i].arrival_time;
                }
            }
            current_time = next_arrival;
            for (int i = 0; i < n; i++) {
                if (p[i].arrival_time == current_time && p[i].remaining_time > 0) {
                    queue[rear++] = i;
                }
            }
            continue;
        }

        int idx = queue[front++];
        Process *proc = &p[idx];

        if (!proc->is_started) {
            proc->response_time = current_time - proc->arrival_time;
            proc->is_started = 1;
        }

        int exec_time = (proc->remaining_time < time_quantum) ? proc->remaining_time : time_quantum;
        proc->remaining_time -= exec_time;
        int prev_time = current_time;
        current_time += exec_time;

        for (int i = 0; i < n; i++) {
            if (p[i].arrival_time > prev_time && p[i].arrival_time <= current_time && p[i].remaining_time > 0) {
                queue[rear++] = i;
            }
        }

        if (proc->remaining_time == 0) {
            proc->completion_time = current_time;
            proc->turnaround_time = proc->completion_time - proc->arrival_time;
            proc->waiting_time = proc->turnaround_time - proc->burst_time;
            completed++;
        } else {
            queue[rear++] = idx;
        }
    }
}

int main() {
    int n, time_quantum;
    printf("Enter number of processes: ");
    scanf("%d", &n);
    printf("Enter time quantum: ");
    scanf("%d", &time_quantum);
    Process p[n];
    
    for(int i = 0; i < n; i++) {
        printf("Process %d (PID, Arrival, Burst): ", i+1);
        scanf("%d%d%d", &p[i].pid, &p[i].arrival_time, &p[i].burst_time);
        p[i].remaining_time = p[i].burst_time;
        p[i].is_started = 0;
    }

    qsort(p, n, sizeof(Process), compare);
    calculateTimes(p, n, time_quantum);

    float avg_turnaround = 0, avg_waiting = 0, avg_response = 0, avg_completion=0;
    printf("\n%-10s %-12s %-10s %-12s %-12s %-12s %-12s\n", "PID", "Arrival", "Burst", "Response", "Completion", "Turnaround", "Waiting");
    for(int i = 0; i < n; i++) {
        printf("%-10d %-12d %-10d %-12d %-12d %-12d %-12d\n", 
              p[i].pid, 
              p[i].arrival_time,
              p[i].burst_time,
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
