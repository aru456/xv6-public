// task2_test2.c
#include "types.h"
#include "stat.h"
#include "user.h"

#define TICKS_PER_SECOND 100 // Assuming 100 ticks per second in xv6

// Simple integer-to-string conversion function
void itoa(int num, char *str) {
    int i = 0;
    if (num == 0) {
        str[i++] = '0';
    } else {
        // Handle the number and convert to string in reverse order
        while (num != 0) {
            str[i++] = (num % 10) + '0';
            num /= 10;
        }
    }
    str[i] = '\0';

    // Reverse the string
    for (int j = 0; j < i / 2; j++) {
        char temp = str[j];
        str[j] = str[i - j - 1];
        str[i - j - 1] = temp;
    }
}

int main() {
    int pid;

    // Run printprimes with argument 3 in the background
    printf(1, "Running 'printprimes 3 &' in the background\n");
    pid = fork();
    if (pid == 0) { // Child process to execute printprimes
        char *args[] = {"printprimes", "3", 0};
        exec("printprimes", args);
        exit();
    } else if (pid > 0) {
        printf(1, "Parent %d created child %d\n", getpid(), pid);
    } else {
        printf(1, "Failed to fork printprimes\n");
        exit();
    }

    // Busy-wait for approximately 1 second to ensure printprimes starts
    int start_ticks = uptime();
    while (uptime() - start_ticks < TICKS_PER_SECOND);

    // Run the nice command to set priority to 7 for the captured PID
    printf(1, "\nRunning 'nice 7 %d'\n", pid);
    if (fork() == 0) { // Child process to execute nice
        char pid_str[10];
        itoa(pid, pid_str); // Convert PID to string for exec arguments
        char *args[] = {"nice", "7", pid_str, 0};
        exec("nice", args);
        exit();
    }
    wait(); // Wait for the nice command to complete

    // Wait for all child processes to finish
    wait();

    exit();
}