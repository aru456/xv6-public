#include "types.h"
#include "stat.h"
#include "user.h"

int main() {
    int pid = getpid();  // Current process PID

    printf(1, "Running comprehensive tests for nice system call\n");

    // Test 1: Set priority for the current process (valid range)
    int old_priority = nice(pid, 10);
    if (old_priority >= 0) {
        printf(1, "Test 1 Passed: PID %d, Old Priority %d, New Priority 10\n", pid, old_priority);
    } else {
        printf(1, "Test 1 Failed: Unable to change priority for current process\n");
    }

    // Test 2: Set out-of-range priority (25) for the current process
    old_priority = nice(pid, 25);
    if (old_priority == -1) {
        printf(1, "Test 2 Passed: Out-of-range priority (25) correctly rejected\n");
    } else {
        printf(1, "Test 2 Failed: Out-of-range priority not handled\n");
    }

    // Test 3: Set negative priority (-5) for the current process
    old_priority = nice(pid, -5);
    if (old_priority == -1) {
        printf(1, "Test 3 Passed: Negative priority (-5) correctly rejected\n");
    } else {
        printf(1, "Test 3 Failed: Negative priority not handled\n");
    }

    // Test 4: Set maximum valid priority (20) for the current process
    old_priority = nice(pid, 20);
    if (old_priority >= 0) {
        printf(1, "Test 4 Passed: PID %d, Old Priority %d, New Priority 20\n", pid, old_priority);
    } else {
        printf(1, "Test 4 Failed: Unable to change priority to maximum valid value\n");
    }

    // Test 5: Set minimum valid priority (0) for the current process
    old_priority = nice(pid, 0);
    if (old_priority >= 0) {
        printf(1, "Test 5 Passed: PID %d, Old Priority %d, New Priority 0\n", pid, old_priority);
    } else {
        printf(1, "Test 5 Failed: Unable to change priority to minimum valid value\n");
    }

    // Test 6: Change priority of a different process
    int child_pid = fork();
    if (child_pid == 0) {  // Child process
        sleep(5);
        exit();
    } else if (child_pid > 0) {  // Parent process
        old_priority = nice(child_pid, 15);  // Change child's priority to 15
        if (old_priority >= 0) {
            printf(1, "Test 6 Passed: Changed priority of PID %d, Old Priority %d, New Priority 15\n", child_pid, old_priority);
        } else {
            printf(1, "Test 6 Failed: Unable to change priority of another process\n");
        }
        wait();
    } else {
        printf(1, "Test 6 Failed: Forking child process failed\n");
    }

    printf(1, "All tests completed\n");
    exit();
}
