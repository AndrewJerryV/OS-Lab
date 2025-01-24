#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

struct Process {
    int id;
    int arrival_time;
    int burst_time;
    int priority;
    int waiting_time;
    int turnaround_time;
    int completed; 
};

int main() {
    int n, current_time = 0, total_completed = 0;
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    struct Process processes[n];

    for (int i = 0; i < n; i++) {
        processes[i].id = i + 1;
        printf("Process %d:\n", i + 1);
        printf("  Arrival time: ");
        scanf("%d", &processes[i].arrival_time);
        printf("  Burst time: ");
        scanf("%d", &processes[i].burst_time);
        printf("  Priority (lower = higher priority): ");
        scanf("%d", &processes[i].priority);
        processes[i].completed = 0;
    }

    while (total_completed < n) {
        int highest_priority = INT_MAX;
        int selected_index = -1;

        for (int i = 0; i < n; i++) {
            if (!processes[i].completed && 
                processes[i].arrival_time <= current_time && 
                processes[i].priority < highest_priority) {
                highest_priority = processes[i].priority;
                selected_index = i;
            }
        }

        if (selected_index != -1) {
            processes[selected_index].waiting_time = current_time - processes[selected_index].arrival_time;
            processes[selected_index].turnaround_time = processes[selected_index].waiting_time + processes[selected_index].burst_time;
            
            current_time += processes[selected_index].burst_time;
            processes[selected_index].completed = 1;
            total_completed++;
        } else {
            int next_arrival = INT_MAX;
            for (int i = 0; i < n; i++) {
                if (!processes[i].completed && processes[i].arrival_time < next_arrival) {
                    next_arrival = processes[i].arrival_time;
                }
            }
            current_time = next_arrival;
        }
    }

    printf("\nProcess ID\tArrival\tBurst\tPriority\tWaiting\tTurnaround\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t\t%d\t%d\t%d\t\t%d\t%d\n",
               processes[i].id,
               processes[i].arrival_time,
               processes[i].burst_time,
               processes[i].priority,
               processes[i].waiting_time,
               processes[i].turnaround_time);
    }

    float avg_wait = 0, avg_turnaround = 0;
    for (int i = 0; i < n; i++) {
        avg_wait += processes[i].waiting_time;
        avg_turnaround += processes[i].turnaround_time;
    }
    avg_wait /= n;
    avg_turnaround /= n;
    printf("\nAverage Waiting Time: %.2f\n", avg_wait);
    printf("Average Turnaround Time: %.2f\n", avg_turnaround);

    return 0;
}
