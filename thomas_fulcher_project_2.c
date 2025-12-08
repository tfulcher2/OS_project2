#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>


char line[100];
int PID[25];
int arr_time[25];
int burst_time[25];
int priority[25];
int count = 0;

int main() {
    FILE *file = fopen("processes.txt", "r");

    if (file == NULL) {
        printf("Error opening file\n");
        return 1;
    }

    printf("Opened file\n");

    while (fgets(line, 100, file) && count < 25) {
        line[strcspn(line, "\n")] = 0;
        sscanf(line, "%d %d %d %d",
            &PID[count], &arr_time[count],
            &burst_time[count], &priority[count]);
        count ++;
    }
    fclose(file);

    printf("–––– Dining Philosophers –––– \n \n");
}
