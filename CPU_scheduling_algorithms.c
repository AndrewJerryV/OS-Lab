#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int pid;
    int arrival_time;
    int burst_time;
    int priority;
    int remaining_time;
    int completion_time;
    int turnaround_time;
    int waiting_time;
    int response_time;
    int is_started;
} Process;

typedef struct {
    int start;
    int end;
    int pid;
} GanttEntry;

// Comparison functions
int compareSJF(const void *a, const void *b) {
    Process *p = (Process *)a, *q = (Process *)b;
    return (p->arrival_time != q->arrival_time) ?  p->arrival_time - q->arrival_time : p->burst_time - q->burst_time;
}

int compareRR(const void *a, const void *b) {
    Process *p = (Process *)a, *q = (Process *)b;
    return p->arrival_time - q->arrival_time;
}

int comparePS(const void *a, const void *b) {
    Process *p = (Process *)a, *q = (Process *)b;
    return (p->arrival_time != q->arrival_time) ? p->arrival_time - q->arrival_time : p->priority - q->priority;
}

int compareFCFS(const void *a, const void *b) {
    Process *p = (Process *)a, *q = (Process *)b;
    return p->arrival_time - q->arrival_time;
}

void printGanttChart(GanttEntry gantt[], int ganttCount) {
    printf("\nGantt Chart:\n");
    for (int i = 0; i < ganttCount; i++) {
        if (gantt[i].pid == -1) 
            printf("[IDLE %d-%d] ", gantt[i].start, gantt[i].end);
        else 
            printf("P%d [%d-%d] ", gantt[i].pid, gantt[i].start, gantt[i].end);
    }
    printf("\n");
}

// SJF scheduling
void calculateSJFTimes(Process p[], int n) {
    qsort(p, n, sizeof(Process), compareSJF);
    int current_time = 0, completed = 0;
    GanttEntry gantt[1000];
    int ganttCount = 0;

    while (completed < n) {
        int next = -1, min_burst = 1e9;

        for(int i = 0; i < n; i++) {
            if(p[i].arrival_time <= current_time && 
               p[i].waiting_time == -1 && 
               p[i].burst_time < min_burst) {
                min_burst = p[i].burst_time;
                next = i;
            }
        }

        if (next == -1) 
            current_time++;
        else {
            gantt[ganttCount].start = current_time;
            gantt[ganttCount].pid = p[next].pid;

            p[next].completion_time = current_time + p[next].burst_time;
            p[next].turnaround_time = p[next].completion_time - p[next].arrival_time;
            p[next].waiting_time = p[next].turnaround_time - p[next].burst_time;
            p[next].response_time = current_time - p[next].arrival_time;

            gantt[ganttCount].end = p[next].completion_time;
            (ganttCount)++;

            current_time = p[next].completion_time;
            completed++;
        }
    }
    printGanttChart(gantt, ganttCount);
}

// Round Robin scheduling
void calculateRRTimes(Process p[], int n, int time_quantum) {
    qsort(p, n, sizeof(Process), compareRR);
    int current_time = 0, completed = 0;
    int queue[100], front = 0, rear = 0;
    GanttEntry gantt[1000];
    int ganttCount = 0;

    for (int i = 0; i < n; i++) {
        p[i].remaining_time = p[i].burst_time;
        p[i].is_started = 0;
    }

    for (int i = 0; i < n; i++) {
        if (p[i].arrival_time <= current_time) 
            queue[rear++] = i;
    }

    while (completed < n) {
        if (front == rear) {
            current_time++;
            for (int i = 0; i < n; i++) {
                if (p[i].arrival_time == current_time && p[i].remaining_time > 0) 
                    queue[rear++] = i;
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
        gantt[ganttCount].start = current_time;
        gantt[ganttCount].pid = proc->pid;

        proc->remaining_time -= exec_time;
        current_time += exec_time;

        gantt[ganttCount].end = current_time;
        (ganttCount)++;

        for (int i = 0; i < n; i++) {
            if (p[i].arrival_time > gantt[ganttCount - 1].start && p[i].arrival_time <= current_time && p[i].remaining_time > 0) 
                queue[rear++] = i;
        }

        if (proc->remaining_time == 0) {
            proc->completion_time = current_time;
            proc->turnaround_time = proc->completion_time - proc->arrival_time;
            proc->waiting_time = proc->turnaround_time - proc->burst_time;
            completed++;
        } else 
            queue[rear++] = idx;
    }
    printGanttChart(gantt, ganttCount);
}

// Priority Scheduling
void calculatePSTimes(Process p[], int n) {
    qsort(p, n, sizeof(Process), comparePS);
    int current_time = 0, completed = 0;
    GanttEntry gantt[1000];
    int ganttCount = 0;

    while (completed < n) {
        int next = -1, min_priority = 1e9;

        for (int i = 0; i < n; i++) {
            if (p[i].arrival_time <= current_time && p[i].waiting_time == -1 && p[i].priority < min_priority) {
                min_priority = p[i].priority;
                next = i;
            }
        }

        if (next == -1) 
            current_time++;
        else {
            gantt[ganttCount].start = current_time;
            gantt[ganttCount].pid = p[next].pid;

            p[next].completion_time = current_time + p[next].burst_time;
            p[next].turnaround_time = p[next].completion_time - p[next].arrival_time;
            p[next].waiting_time = p[next].turnaround_time - p[next].burst_time;
            p[next].response_time = current_time - p[next].arrival_time;

            gantt[ganttCount].end = p[next].completion_time;
            (ganttCount)++;

            current_time = p[next].completion_time;
            completed++;
        }
    }
    printGanttChart(gantt, ganttCount);
}

// FCFS Scheduling
void calculateFCFSTimes(Process p[], int n) {
    qsort(p, n, sizeof(Process), compareFCFS);
    GanttEntry gantt[1000];
    int ganttCount = 0;
    int current_time = 0;

    for (int i = 0; i < n; i++) {
        if (current_time < p[i].arrival_time) {
            gantt[ganttCount].start = current_time;
            gantt[ganttCount].end = p[i].arrival_time;
            gantt[ganttCount].pid = -1; 
            (ganttCount)++;
            current_time = p[i].arrival_time;
        }
        gantt[ganttCount].start = current_time;
        gantt[ganttCount].pid = p[i].pid;
        p[i].response_time = current_time - p[i].arrival_time;
        p[i].completion_time = current_time + p[i].burst_time;
        p[i].turnaround_time = p[i].completion_time - p[i].arrival_time;
        p[i].waiting_time = p[i].turnaround_time - p[i].burst_time;
        gantt[ganttCount].end = p[i].completion_time;
        (ganttCount)++;
        current_time = p[i].completion_time;
    }
    printGanttChart(gantt, ganttCount);
}

void printResults(Process p[], int n) {
    float avg_turnaround = 0, avg_waiting = 0, avg_response = 0, avg_completion = 0;
    printf("\n%-10s %-12s %-10s %-8s %-12s %-12s %-12s %-12s\n", "PID", "Arrival", "Burst", "Priority", "Response", "Completion", "Turnaround", "Waiting");
    for (int i = 0; i < n; i++) {
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
    printf("\nResponse Time:   %.2f", avg_response / n);
    printf("\nCompletion Time: %.2f", avg_completion / n);
    printf("\nTurnaround Time: %.2f", avg_turnaround / n);
    printf("\nWaiting Time:    %.2f\n", avg_waiting / n);
}

int main() {
    int n, time_quantum;
    printf("Enter number of processes: ");
    scanf("%d", &n);
    Process processes[n];

    for(int i = 0; i < n; i++) {
        printf("Process %d (PID, Arrival, Burst, Priority): ", i+1);
        scanf("%d%d%d%d", &processes[i].pid, &processes[i].arrival_time, &processes[i].burst_time, &processes[i].priority);
        processes[i].waiting_time = 0;
        processes[i].is_started = 0;
        processes[i].remaining_time = 0;
    }

    printf("Enter time quantum for Round Robin: ");
    scanf("%d", &time_quantum);

    printf("\nFCFS Scheduling\n---------------------------\n");
    calculateFCFSTimes(processes, n);
    printResults(processes, n);

    printf("\nSJF Scheduling\n---------------------------\n");
    for(int i = 0; i < n; i++) processes[i].waiting_time = -1;
    calculateSJFTimes(processes, n);
    printResults(processes, n);

    printf("\nRound Robin Scheduling\n---------------------------\n");
    calculateRRTimes(processes, n, time_quantum);
    printResults(processes, n);

    printf("\nPriority Scheduling\n---------------------------\n");
    for(int i = 0; i < n; i++) processes[i].waiting_time = -1;
    calculatePSTimes(processes, n);
    printResults(processes, n);
    return 0;
}
