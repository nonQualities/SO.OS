/*
 * Experiment 5(c): First-Come, First-Served (FCFS) CPU Scheduling
 * What it does: Schedules n processes strictly in order of arrival (assumed
 *               t = 0 in input order) and calculates waiting and turnaround
 *               times along with their averages.
 * How it works: Reads each process's burst time in arrival order; assigns
 *               wait[0] = 0 and wait[i] = wait[i-1] + burst[i-1], and sets
 *               turnaround[i] = wait[i] + burst[i].
 * Related OS Concepts:
 *   - Non-Preemptive FIFO Ready-Queue Dispatching
 *   - Convoy Effect (Short Processes Delayed Behind a Long CPU-Bound Job)
 *   - Waiting Time and Turnaround Time in Batch Scheduling
 */

#include <stdio.h>

#define MAX_PROCESSES 10

struct process {
    int pid;
    int burst;
    int wait;
    int turnaround;
};

int main(void)
{
    struct process p[MAX_PROCESSES];
    int n, i;
    int total_wait = 0;
    int total_turnaround = 0;

    printf("FCFS scheduling\nEnter number of processes: ");
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX_PROCESSES) {
        fprintf(stderr, "Enter a number between 1 and %d.\n", MAX_PROCESSES);
        return 1;
    }

    /* Compute WT and TAT inline in arrival order as a prefix sum of bursts. */
    for (i = 0; i < n; i++) {
        p[i].pid = i + 1;
        printf("Burst time of process %d: ", p[i].pid);
        if (scanf("%d", &p[i].burst) != 1 || p[i].burst < 0) {
            fprintf(stderr, "Invalid burst time.\n");
            return 1;
        }

        /* First process starts immediately; subsequent wait for prior finish. */
        p[i].wait = (i == 0) ? 0 : p[i - 1].wait + p[i - 1].burst;
        p[i].turnaround = p[i].wait + p[i].burst;
        total_wait += p[i].wait;
        total_turnaround += p[i].turnaround;
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
