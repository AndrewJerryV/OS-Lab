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

void calculateTimes(Process processes[], int n) {
    int current_time = 0;
    for (int i = 0; i < n; i++) {
        if (current_time < processes[i].arrival_time) 
            current_time = processes[i].arrival_time;
        processes[i].response_time = current_time - processes[i].arrival_time;
        processes[i].completion_time = current_time + processes[i].burst_time;
        processes[i].turnaround_time = processes[i].completion_time - processes[i].arrival_time;
        processes[i].waiting_time = processes[i].turnaround_time - processes[i].burst_time;
        current_time = processes[i].completion_time;
    }
}

void printTable(Process processes[], int n) {
    printf("\n%-4s%-8s%-6s%-11s%-11s%-8s%-9s\n", "PID", "Arrival", "Burst", "Completion", "Turnaround", "Waiting", "Response");
    for (int i = 0; i < n; i++) {
        printf("%-4d%-8d%-6d%-11d%-11d%-8d%-9d\n",
            processes[i].pid, 
            processes[i].arrival_time, 
            processes[i].burst_time, 
            processes[i].completion_time, 
            processes[i].turnaround_time, 
            processes[i].waiting_time, 
            processes[i].response_time);
    }
}

int main() {
    int n;
    printf("Enter the number of processes: ");
    scanf("%d", &n);
    Process processes[n];

    printf("Enter process details (PID, Arrival Time, Burst Time):\n");
    for (int i = 0; i < n; i++) {
        printf("Process %d: ", i + 1);
        scanf("%d %d %d", &processes[i].pid, &processes[i].arrival_time, &processes[i].burst_time);
    }

    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (processes[j].arrival_time > processes[j + 1].arrival_time) {
                Process temp = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp;
            }
        }
    }
    calculateTimes(processes, n);
    printTable(processes, n);
    return 0;
}
