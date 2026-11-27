/*
 * Experiment 5(a): Non-Preemptive Priority CPU Scheduling
 * What it does: Schedules n processes (all arriving at t = 0) in ascending
 *               priority-number order and computes per-process and average
 *               waiting and turnaround times.
 * How it works: Reads burst time and integer priority for each process, sorts
 *               the process table via qsort() using comparator by_priority()
 *               (breaking ties by PID to preserve FCFS arrival order), and
 *               accumulates elapsed CPU time to compute wait and turnaround.
 * Related OS Concepts:
 *   - Non-Preemptive CPU Scheduling and Priority Assignment
 *   - Scheduling Metrics: Waiting Time (WT) and Turnaround Time (TAT)
 *   - Starvation (Indefinite Blocking) and Aging as a Mitigation Strategy
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_PROCESSES 10

/* Process Control Block (PCB) representation for scheduling simulation. */
struct process {
    int pid;
    int priority;
    int burst;
    int wait;
    int turnaround;
};

/* qsort comparator: orders by priority ascending, then PID ascending. */
static int by_priority(const void *a, const void *b)
{
    const struct process *x = a;
    const struct process *y = b;

    if (x->priority != y->priority)
        return x->priority - y->priority;   /* smaller number = higher priority */
    return x->pid - y->pid;                 /* keep input order for ties */
}

int main(void)
{
    struct process p[MAX_PROCESSES];
    int n, i, elapsed = 0;
    int total_wait = 0;
    int total_turnaround = 0;

    printf("Priority scheduling\nEnter number of processes: ");
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX_PROCESSES) {
        fprintf(stderr, "Enter a number between 1 and %d.\n", MAX_PROCESSES);
        return 1;
    }

    for (i = 0; i < n; i++) {
        p[i].pid = i + 1;
        printf("Process %d (burst time priority): ", p[i].pid);
        if (scanf("%d %d", &p[i].burst, &p[i].priority) != 2 ||
            p[i].burst < 1) {
            fprintf(stderr, "Invalid input.\n");
            return 1;
        }
    }

    /* Order processes from highest priority (lowest integer) to lowest. */
    qsort(p, n, sizeof p[0], by_priority);

    /* Non-preemptive dispatch: each process runs to completion upon selection. */
    for (i = 0; i < n; i++) {
        p[i].wait = elapsed;                  /* WT = start time (arrival = 0) */
        p[i].turnaround = elapsed + p[i].burst; /* TAT = WT + Burst Time */
        elapsed += p[i].burst;
        total_wait += p[i].wait;
        total_turnaround += p[i].turnaround;
    }

    printf("\nPID  Priority  Burst  Wait  Turnaround\n");
    for (i = 0; i < n; i++)
        printf("%3d  %8d  %5d  %4d  %10d\n",
               p[i].pid, p[i].priority, p[i].burst,
               p[i].wait, p[i].turnaround);

    printf("\nTotal waiting time    : %d\n", total_wait);
    printf("Average waiting time  : %.2f\n", (double)total_wait / n);
    printf("Total turnaround time : %d\n", total_turnaround);
    printf("Average turnaround    : %.2f\n", (double)total_turnaround / n);

    return 0;
}
