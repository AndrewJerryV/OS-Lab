#include <stdio.h>

typedef struct {
    int pid;             
    int arrival_time;   
    int burst_time;      
    int completion_time; 
    int turnaround_time; 
    int waiting_time;    
    int response_time;   
} Process;

int main() {
    int n;
    printf("Enter number of processes: ");
    scanf("%d", &n);
    Process p[n];
    
    for(int i=0; i<n; i++) {
        printf("Process %d (PID, Arrival, Burst): ", i+1);
        scanf("%d%d%d", &p[i].pid, &p[i].arrival_time, &p[i].burst_time);
    }

    for(int i=0; i<n-1; i++) {
        for(int j=0; j<n-i-1; j++) {
            if(p[j].arrival_time > p[j+1].arrival_time) {
                Process temp = p[j];
                p[j] = p[j+1];
                p[j+1] = temp;
            }
        }
    }

    int current_time = 0;
    printf("\nGantt Chart:\n");
    
    for(int i=0; i<n; i++) {
        if(current_time < p[i].arrival_time) {
            printf("[IDLE %d-%d] ", current_time, p[i].arrival_time);
            current_time = p[i].arrival_time;
        }
        p[i].response_time = current_time - p[i].arrival_time;
        printf("P%d [%d-%d] ", p[i].pid, current_time, current_time + p[i].burst_time);
        p[i].completion_time = current_time + p[i].burst_time;
        p[i].turnaround_time = p[i].completion_time - p[i].arrival_time;
        p[i].waiting_time = p[i].turnaround_time - p[i].burst_time;
        current_time = p[i].completion_time;
    }

    float avg_turnaround = 0, avg_waiting = 0, avg_response = 0, avg_completion=0;
    printf("\n\n%-5s%-8s%-6s%-10s%-12s%-12s%-8s", "PID", "Arrival", "Burst", "Response", "Completion", "Turnaround", "Waiting");
    printf("\n----------------------------------------------------------------\n");

    for(int i=0; i<n; i++) {
        printf(" %-5d %-8d%-6d %-10d%-12d%-12d%-8d\n",
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
    printf("\nResponse Time:   %f", avg_response/n);
    printf("\nCompletion Time: %f", avg_completion/n);
    printf("\nTurnaround Time: %f", avg_turnaround/n);
    printf("\nWaiting Time:    %f\n", avg_waiting/n);

    return 0;
}
