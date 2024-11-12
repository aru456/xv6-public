#include "types.h"
#include "stat.h"
#include "user.h"

int main() {
    int pid = getpid();  // Current process PID

    printf(1, "Executing comprehensive tests for the nice system_call with nice <value> and PID is current PID\n");

    // Test 1: Set the maximum valid priority (5) for the current process
    int old_priority = nice(pid, 5);
    if (old_priority >= 0) {
        printf(1, "Test 1 Passed: Successfully set PID %d from Old Priority %d to New Priority 5\n", pid, old_priority);
    } else {
        printf(1, "Test 1 Failed: Unable to set priority to 5\n");
    }

    // Test 2: Set an out-of-range priority (25) for the current process
    old_priority = nice(pid, 25);
    if (old_priority == -1) {
        printf(1, "Test 2 Passed: Rejected invalid high priority (25)\n");
    } else {
        printf(1, "Test 2 Failed: Did not handle out-of-range priority\n");
    }

    // Test 3: Set priority within valid range (4) for the current process
    old_priority = nice(pid, 4);
    if (old_priority >= 0) {
        printf(1, "Test 3 Passed: Changed PID %d from Old Priority %d to New Priority 4\n", pid, old_priority);
    } else {
        printf(1, "Test 3 Failed: Failed to set priority to 4\n");
    }

    // Test 4: Set minimum valid priority (1) for the current process
    old_priority = nice(pid, 1);
    if (old_priority >= 0) {
        printf(1, "Test 4 Passed: Updated PID %d from Old Priority %d to New Priority 1\n", pid, old_priority);
    } else {
        printf(1, "Test 4 Failed: Unable to set priority to 1\n");
    }

    // Test 5: Attempt to set a negative priority (-5) for the current process
    old_priority = nice(pid, -5);
    if (old_priority == -1) {
        printf(1, "Test 5 Passed: Correctly rejected negative priority (-5)\n");
    } else {
        printf(1, "Test 5 Failed: Negative priority not handled properly\n");
    }
    // chedking nice <PID> <VALUE>

     printf(1, "Now testing nice <PID> <value>\n");
     // Test 1: Attempt to set a negative priority (-5) for PID 1
    old_priority = nice(1, -5);
    if (old_priority == -1) {
        printf(1, "Test 1 Passed: Correctly rejected negative priority (-5) for PID-1\n");
    } else {
        printf(1, "Test 1 Failed: Negative priority not handled properly for PID-1\n");
    }
    
    // Test 2: Set a valid priority (1) for PID 2
    old_priority = nice(2, 1);
    if (old_priority >= 0) {
        printf(1, "Test 2 Passed: Successfully changed priority to 1 for PID-2\n");
    } else {
        printf(1, "Test 2 Failed: Did not change the priority for PID-2\n");
    }

    // Test 3: Attempt to set an out-of-range priority (10) for PID 2
    old_priority = nice(2, 10);
    if (old_priority == -1) {
        printf(1, "Test 3 Passed: Correctly rejected out-of-range priority (10) for PID-2\n");
    } else {
        printf(1, "Test 3 Failed: Out-of-range priority not handled properly for PID-2\n");
    }

    // Test 4: Set maximum valid priority (5) for PID 4
    old_priority = nice(1, 5);
    if (old_priority >= 0) {
        printf(1, "Test 4 Passed: Successfully set maximum priority (5) for PID-1\n");
    } else {
        printf(1, "Test 4 Failed: Unable to set maximum priority (5) for PID-1\n");
    }

    // Test 5: Set minimum valid priority (1) for PID 1
    old_priority = nice(1, 1);
    if (old_priority >= 0) {
        printf(1, "Test 5 Passed: Successfully set minimum priority (1) for PID-1\n");
    } else {
        printf(1, "Test 5 Failed: Unable to set minimum priority (1) for PID-1\n");
    }

    // Test 6: Attempt to set priority (3) for a non-existent PID (e.g., 9999)
    old_priority = nice(9999, 3);
    if (old_priority == -1) {
        printf(1, "Test 6 Passed: Correctly handled non-existent PID (9999)\n");
    } else {
        printf(1, "Test 6 Failed: Non-existent PID not handled correctly\n");
    }

    printf(1, "All tests completed successfully\n");
    exit();
}
