#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main(int argc, char *argv[]) {
    int priority, pid, old_priority;

    // Validate argument count and priority
    if (argc == 2) {
        pid = getpid();  // Get the PID of the current process
        priority = atoi(argv[1]);
    } else if (argc == 3) {
        pid = atoi(argv[1]);
        // Directly checking the input argument to ensure valid range
        if (argv[2][0] == '-') {
            printf(2, "Invalid priority: negative values not allowed\n");
            exit();
        }
        priority = atoi(argv[2]);
    } else {
        printf(2, "Usage: nice pid priority\n");
        exit();
    }

    // Final check if priority is within range after conversion
    if (priority < 0 || priority > 20) {
        printf(2, "Invalid priority (0-20)!\n");
        exit();
    }

    // Call the system call to change the nice value and get the old priority
    old_priority = nice(pid, priority);
    if (old_priority < 0) {
        printf(2, "Failed to change priority for PID %d\n", pid);
    } else {
        printf(1, "%d %d\n", pid, old_priority); // Output: PID and old priority value
    }

    exit();
}
