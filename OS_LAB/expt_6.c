/*
 * Experiment 6: Producer-Consumer Problem Using Semaphores
 * What it does: Simulates bounded-buffer synchronization (capacity = 3)
 *               between a producer and a consumer via an interactive menu.
 * How it works: Models binary semaphore `mutex` (mutual exclusion) and
 *               counting semaphores `empty` (free slots) and `full` (occupied
 *               slots) with wait_sem() (P) and signal_sem() (V) operations,
 *               guarding produce() when empty > 0 and consume() when full > 0.
 * Related OS Concepts:
 *   - Process Synchronization and the Bounded-Buffer (Producer-Consumer) Problem
 *   - Binary Semaphores (Mutex) and Counting Semaphores (Full/Empty Slots)
 *   - Mutual Exclusion, Critical Section, and Race Condition Prevention
 */

#include <stdio.h>

#define BUFFER_SIZE 3

/* Synchronization primitives and shared buffer item counter. */
int mutex = 1;              /* binary semaphore for critical section access */
int full = 0;               /* counting semaphore: occupied buffer slots */
int empty = BUFFER_SIZE;    /* counting semaphore: available buffer slots */
int item = 0;               /* simulated item sequence number */

/* Simulated semaphore P (wait) and V (signal) primitive operations. */
static int wait_sem(int s)   { return s - 1; }
static int signal_sem(int s) { return s + 1; }

/* Enter critical section, reserve an empty slot, produce, and signal full. */
static void produce(void)
{
    mutex = wait_sem(mutex);
    empty = wait_sem(empty);
    full = signal_sem(full);

    printf("Producer produced item %d\n", ++item);

    mutex = signal_sem(mutex);
}

/* Enter critical section, claim a full slot, consume, and signal empty. */
static void consume(void)
{
    mutex = wait_sem(mutex);
    full = wait_sem(full);
    empty = signal_sem(empty);

    printf("Consumer consumed item %d\n", item--);

    mutex = signal_sem(mutex);
}

int main(void)
{
    int choice;

    /* Interactive dispatcher checking buffer preconditions before invocation. */
    while (1) {
        printf("\n1. Produce\n2. Consume\n3. Exit\nChoice: ");
        if (scanf("%d", &choice) != 1)
            break;

        switch (choice) {
        case 1:
            if (empty > 0)
                produce();
            else
                printf("Buffer is full.\n");
            break;
        case 2:
            if (full > 0)
                consume();
            else
                printf("Buffer is empty.\n");
            break;
        case 3:
            return 0;
        default:
            printf("Invalid choice.\n");
        }
    }

    return 0;
}
