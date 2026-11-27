/*
 * Experiment 8: Deadlock Avoidance Using Banker's Algorithm
 * What it does: Determines whether a system of n processes and r resource types
 *               is in a safe state and, if so, outputs a valid safe execution
 *               sequence along with Allocation, Max, Need, and Available tables.
 * How it works: Reads Max, Allocation, and Available vectors; computes
 *               Need[i][j] = Max[i][j] - Allocation[i][j]; then iteratively
 *               selects an unfinished process whose Need <= Work, simulates its
 *               completion by adding Allocation to Work, and checks if all n
 *               processes finish.
 * Related OS Concepts:
 *   - Deadlock Avoidance vs. Prevention and Detection
 *   - Safe State, Unsafe State, and Safe Sequence Invariant
 *   - Resource Allocation Matrices (Max, Allocation, Need, Available)
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX 100

int n, r;
int max_need[MAX][MAX];
int alloc[MAX][MAX];
int need[MAX][MAX];
int avail[MAX];

/* Read a validated integer from stdin, terminating on malformed input. */
static int read_int(const char *prompt)
{
    int value;

    printf("%s", prompt);
    if (scanf("%d", &value) != 1) {
        fprintf(stderr, "Invalid input.\n");
        exit(EXIT_FAILURE);
    }
    return value;
}

/* Populate an n x r resource matrix (Max or Allocation) from stdin. */
static void read_matrix(int matrix[MAX][MAX], const char *name)
{
    int i, j;

    printf("Enter the %s matrix (%d x %d):\n", name, n, r);
    for (i = 0; i < n; i++) {
        for (j = 0; j < r; j++) {
            if (scanf("%d", &matrix[i][j]) != 1) {
                fprintf(stderr, "Invalid input.\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

/* Gather system dimensions, Max, Allocation, and Available vectors. */
static void input(void)
{
    int j;

    n = read_int("Enter number of processes: ");
    r = read_int("Enter number of resource types: ");
    if (n < 1 || n > MAX || r < 1 || r > MAX) {
        fprintf(stderr, "Values must be between 1 and %d.\n", MAX);
        exit(EXIT_FAILURE);
    }

    read_matrix(max_need, "Max");
    read_matrix(alloc, "Allocation");

    printf("Enter Available resources (%d values):\n", r);
    for (j = 0; j < r; j++) {
        if (scanf("%d", &avail[j]) != 1) {
            fprintf(stderr, "Invalid input.\n");
            exit(EXIT_FAILURE);
        }
    }
}

/* Display formatted Allocation, Max, Need, and Available resource tables. */
static void show(void)
{
    int i, j;

    printf("\n%-8s %-12s %-12s %-12s %s\n",
           "Process", "Allocation", "Max", "Need", "Available");

    for (i = 0; i < n; i++) {
        printf("P%-7d", i);

        for (j = 0; j < r; j++)
            printf("%3d", alloc[i][j]);
        printf("   ");

        for (j = 0; j < r; j++)
            printf("%3d", max_need[i][j]);
        printf("   ");

        for (j = 0; j < r; j++)
            printf("%3d", need[i][j]);
        printf("   ");

        if (i == 0) {
            for (j = 0; j < r; j++)
                printf("%3d", avail[j]);
        }
        printf("\n");
    }
}

/* Execute Dijkstra's Safety Algorithm to verify existence of a safe sequence. */
static void check_safe(void)
{
    int finish[MAX] = {0};
    int work[MAX];
    int safe[MAX];
    int count = 0;
    int i, j;

    /* Step 1: Compute remaining Need matrix = Max - Allocation. */
    for (i = 0; i < n; i++)
        for (j = 0; j < r; j++)
            need[i][j] = max_need[i][j] - alloc[i][j];

    /* Step 2: Initialize Work vector to currently Available resources. */
    for (j = 0; j < r; j++)
        work[j] = avail[j];

    /* Step 3: Greedily find unfinished processes whose Need <= Work. */
    while (count < n) {
        int found = 0;

        for (i = 0; i < n; i++) {
            if (finish[i])
                continue;

            for (j = 0; j < r && need[i][j] <= work[j]; j++)
                ;

            if (j == r) {               /* all needs can be met */
                /* Simulate process completion and reclaim allocated resources. */
                for (j = 0; j < r; j++)
                    work[j] += alloc[i][j];
                finish[i] = 1;
                safe[count++] = i;
                found = 1;
            }
        }

        if (!found)
            break;                      /* no process can finish */
    }

    /* Step 4: Safe iff every process could be scheduled to completion. */
    if (count == n) {
        printf("\nSystem is in a safe state.\nSafe sequence: ");
        for (i = 0; i < n; i++)
            printf("P%d%s", safe[i], i == n - 1 ? "\n" : " -> ");
    } else {
        printf("\nSystem is in an unsafe state (deadlock possible).\n");
    }
}

int main(void)
{
    printf("Banker's Algorithm\n");
    input();
    check_safe();   /* computes need[][] */
    show();

    return 0;
}
