#include "types.h"
#include "stat.h"
#include "user.h"

// Helper function to run each test with description and arguments
void run_test(char *desc, char *arg1, char *arg2) {
    printf(1, "Test: %s | Command: nice %s %s\n", desc, arg1, arg2 ? arg2 : "");
    if (fork() == 0) {
        char *args[] = {"nice", arg1, arg2, 0};
        exec("nice", args);
        exit();
    }
    wait();
}

int main() {
    printf(1, "Running refactored test1.c for nice CLI edge cases\n");

    // Test: Valid priority for the current process
    run_test("Setting priority to 1 for current process", "1", 0);
    
    // Test: Valid high priority for the current process
    run_test("Setting priority to 5 for current process", "5", 0);

    // Test: Priority out of range for current process
    run_test("Setting priority to 9 (out of range) for current process", "9", 0);

    // Test: Zero priority (invalid)
    run_test("Setting priority to 0 (out of range) for current process", "0", 0);

    // Test: Negative priority (invalid)
    run_test("Setting priority to -9 (invalid) for current process", "-9", 0);

    // Test: Non-numeric priority
    run_test("Setting priority to a non-numeric value ','", ",", 0);

    // Test: Priority out of range for specific PID
    run_test("Setting priority to 9 for PID 1 (invalid priority)", "1", "9");

    // Test: Valid priority for specific PID
    run_test("Setting priority to 5 for PID 1 (valid)", "1", "5");

    // Test: Non-numeric priority for specific PID
    run_test("Setting priority to 'abc' for PID 1 (invalid priority)", "1", "abc");

    // Test: Non-numeric PID
    run_test("Setting priority to 3 for non-numeric PID 'abc'", "abc", "3");

    // Test: Valid priority for non-existing PID
    run_test("Setting priority to 5 for non-existing PID 40", "40", "5");

    // Test: Valid priority for a non-zero PID that might not exist
    run_test("Setting priority to 4 for PID 0 (invalid PID)", "0", "4");

    exit();
}
