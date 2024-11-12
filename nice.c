#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

// Helper function to validate and parse priority as an integer within range
int validate_priority(const char *str) {
    int priority = 0;

    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] < '0' || str[i] > '9') return -1; // Non-numeric input
        priority = priority * 10 + (str[i] - '0');
    }

    return (priority >= 1 && priority <= 5) ? priority : -1;
}

// Helper function to validate and parse PID as a positive integer
int validate_pid(const char *str) {
    int pid = 0;

    // Check for non-numeric input and prevent `0` from being considered valid
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] < '0' || str[i] > '9') return -1;  // Non-numeric input
        pid = pid * 10 + (str[i] - '0');
    }

    return (pid > 0) ? pid : -1;  // Only return pid if greater than 0
}


int main(int argc, char *argv[]) {
    int pid, priority, old_priority;

    // Determine PID and priority based on argument count
    if (argc == 2) {
        pid = getpid();  // Default to current process
        priority = validate_priority(argv[1]);
    } else if (argc == 3) {
        pid = validate_pid(argv[1]);
        priority = validate_priority(argv[2]);
    } else {
        printf(2, "Usage: nice <value> or nice <value> <priority>\n");
        exit();
    }

    // Check for invalid PID or priority input
    if (pid == -1) {
        printf(2, "Error: PID must be a valid positive integer.\n");
        exit();
    }
    if (priority == -1) {
        printf(2, "Error: Priority must be an integer from 1 to 5.\n");
        exit();
    }

    // Attempt to change priority and output result
    old_priority = nice(pid, priority);
    if (old_priority == -1) {
        printf(2, "Error: Failed to set priority for PID %d. Process may not exist.\n", pid);
    } else {
        printf(1, "%d %d\n", pid, old_priority);
    }

    exit();
}
