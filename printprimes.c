#include "types.h"
#include "stat.h"
#include "user.h"

// Waits for the specified number of seconds
void wait_for_seconds(int seconds) {
    int start_ticks = uptime();
    while (uptime() - start_ticks < seconds * 100);
}

// Checks if a number is prime
int is_prime(int num) {
    if (num <= 1) return 0;
    for (int i = 2; i <= num / 2; i++) {
        if (num % i == 0) return 0;
    }
    return 1;
}

// Calculates and prints prime numbers up to the specified limit
void calculate_primes(int limit, int mutexid) {
    int num = 2, count = 0;

    while (count < limit) {
        if (is_prime(num)) {
            lock(mutexid);
            printf(1, "PID %d printing %d th Prime number:%d\n", getpid(), count + 1, num);
            unlock(mutexid);

            count++;
            wait_for_seconds(1);
        }
        num++;
    }
}

// Creates child processes to calculate primes concurrently
void create_child_processes(int n, int limit, int mutexid) {
    for (int i = 0; i < n; i++) {
        int pid = fork();
        if (pid < 0) {
            printf(1, "Error: fork failed\n");
            exit();
        } else if (pid == 0) {
            printf(1, "Child %d started\n", getpid());
            calculate_primes(limit, mutexid);
            printf(1, "Child %d finished\n", getpid());
            exit();
        } else {
            printf(1, "Parent %d created child %d\n", getpid(), pid);
        }
    }

    for (int i = 0; i < n; i++) {
        wait();
    }
}

// Signals that child processes have been created by writing to a pipe
void signal_creation_complete(int pipe_fd) {
    if (pipe_fd >= 0) {
        write(pipe_fd, "done", 4);
        close(pipe_fd);
    }
}

int main(int argc, char *argv[]) {
    int n = (argc > 1) ? atoi(argv[1]) : 1;
    int pipe_fd = (argc > 2) ? atoi(argv[2]) : -1;

    if (n < 1) n = 1;

    int mutexid = getmutex();
    if (mutexid < 0) {
        printf(1, "Error: Unable to get mutex\n");
        exit();
    }

    signal_creation_complete(pipe_fd);  // Signal all child processes created
    create_child_processes(n, 1000, mutexid);

    exit();
}
