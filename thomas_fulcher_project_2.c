#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

#define MAX_THREADS 25

pthread_mutex_t *forks;
int NUM_THREADS;

int TID[MAX_THREADS];
int arr_time[MAX_THREADS];
int burst_time[MAX_THREADS];
int priority[MAX_THREADS];
int count = 0;

void *philosopher(void* arg) {
    int id = *(int*)arg;
    int left = id;
    int right = (id + 1) % NUM_THREADS;

    //pick the lowest numbered fork
    int first = left < right ? left : right;
    int second = left < right ? right : left;

    sleep(arr_time[id]);

    printf("Philosopher %d is thinking.\n", TID[id]);
    sleep(1); //thinking
    printf("\n");

    //try to pick up forks
    printf("Philosopher %d tries to pick up forks %d and %d.\n", TID[id], first, second);
    pthread_mutex_lock(&forks[first]);
    pthread_mutex_lock(&forks[second]);
    printf("\n");
    //eating simulator
    printf("Philosopher %d is eating... (Burst = %d sec)\n", TID[id], burst_time[id]);
    sleep(burst_time[id]);
    printf("\n");

    pthread_mutex_unlock(&forks[first]);
    pthread_mutex_unlock(&forks[second]);

    printf("Philosopher %d is done.\n", TID[id]);
    printf("\n");

    return NULL;
}

int main() {
    FILE *file = fopen("threads.txt", "r");

    if (file == NULL) {
        printf("Error opening file\n");
        return 1;
    }

    printf("Opened file\n");

    char line[100];

    fgets(line, 100, file);

    while (fgets(line, 100, file) && count < 25) {
        line[strcspn(line, "\n")] = 0;
        sscanf(line, "%d %d %d %d",
            &TID[count], &arr_time[count],
            &burst_time[count], &priority[count]);
        count ++;
    }
    fclose(file);

    NUM_THREADS = count;

    // allocate mutexes for forks
    forks = malloc(sizeof(pthread_mutex_t) * NUM_THREADS);

    // initialize each fork (mutex)
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_mutex_init(&forks[i], NULL);
    }

    // declare arrays
    pthread_t threads[NUM_THREADS];
    int ids[NUM_THREADS];

    // create philosopher threads
    for (int i = 0; i < NUM_THREADS; i++) {
        ids[i] = i; // Thread ID
        pthread_create(&threads[i], NULL, philosopher, &ids[i]);
    }

    // wait for all threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // destroy mutexes
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_mutex_destroy(&forks[i]);
    }

    // free
    free(forks);

    printf("All philosophers are done eating.\n");
}