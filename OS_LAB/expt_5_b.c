/*
 * Experiment 5(b): Round Robin (RR) Preemptive CPU Scheduling
 * What it does: Simulates time-shared Round Robin scheduling for n processes
 *               (arriving at t = 0) given a fixed time quantum, outputting a
 *               Gantt chart trace and waiting/turnaround statistics.
 * How it works: Repeatedly cycles through the process list; each unfinished
 *               process executes for min(remaining, quantum) units. When
 *               remaining reaches zero, turnaround = completion time and
 *               wait = turnaround - burst.
 * Related OS Concepts:
 *   - Preemptive Time-Sharing Scheduling and Time Quantum (Time Slice)
 *   - Context Switching Overhead vs. Responsiveness Trade-off
 *   - Gantt Chart Execution Timeline and Fairness (Starvation-Free)
 */

#include <stdio.h>

#define MAX_PROCESSES 10

/* Process descriptor tracking total burst and remaining CPU time. */
struct process {
    int pid;
    int burst;
    int remaining;
    int wait;
    int turnaround;
};

int main(void)
{
    struct process p[MAX_PROCESSES];
    int n, quantum, i;
    int time = 0, completed = 0;
    int total_wait = 0;
    int total_turnaround = 0;

    printf("Round Robin scheduling\nEnter number of processes: ");
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX_PROCESSES) {
        fprintf(stderr, "Enter a number between 1 and %d.\n", MAX_PROCESSES);
        return 1;
    }

    printf("Enter time quantum: ");
    if (scanf("%d", &quantum) != 1 || quantum < 1) {
        fprintf(stderr, "Time quantum must be a positive number.\n");
        return 1;
    }

    for (i = 0; i < n; i++) {
        p[i].pid = i + 1;
        printf("Burst time of process %d: ", p[i].pid);
        if (scanf("%d", &p[i].burst) != 1 || p[i].burst < 1) {
            fprintf(stderr, "Burst time must be a positive number.\n");
            return 1;
        }
        p[i].remaining = p[i].burst;
        p[i].wait = 0;
        p[i].turnaround = 0;
    }

    /* Circular ready-queue simulation until all n processes finish. */
    printf("\nGantt chart:\n");
    while (completed < n) {
        for (i = 0; i < n; i++) {
            int slice;

            if (p[i].remaining == 0)
                continue;

            /* Grant at most one quantum (or remaining burst if smaller). */
            slice = (p[i].remaining < quantum) ? p[i].remaining : quantum;
            printf("P%d [%d - %d]\n", p[i].pid, time, time + slice);

            time += slice;
            p[i].remaining -= slice;

            /* Record completion metrics once the process finishes its burst. */
            if (p[i].remaining == 0) {
                p[i].turnaround = time;                 /* arrival = 0 */
                p[i].wait = time - p[i].burst;          /* WT = TAT - BT */
                total_wait += p[i].wait;
                total_turnaround += p[i].turnaround;
                completed++;
            }
        }
    }

    printf("\nPID  Burst  Wait  Turnaround\n");
    for (i = 0; i < n; i++)
        printf("%3d  %5d  %4d  %10d\n",
               p[i].pid, p[i].burst, p[i].wait, p[i].turnaround);

    printf("\nTotal waiting time    : %d\n", total_wait);
    printf("Average waiting time  : %.2f\n", (double)total_wait / n);
    printf("Total turnaround time : %d\n", total_turnaround);
    printf("Average turnaround    : %.2f\n", (double)total_turnaround / n);

    return 0;
}
