#include "types.h"
#include "stat.h"
#include "user.h"

// Function to convert an integer to a string
void int_to_string(int num, char *str) {
    int idx = 0;
    if (num == 0) {
        str[idx++] = '0';
    } else {
        int temp = num;
        while (temp != 0) {
            str[idx++] = (temp % 10) + '0';
            temp /= 10;
        }
    }
    str[idx] = '\0';

    // Reverse the string to get the correct order
    for (int i = 0; i < idx / 2; i++) {
        char temp = str[i];
        str[i] = str[idx - i - 1];
        str[idx - i - 1] = temp;
    }
}

int main() {
    int child_pid;
    int signal_pipe[2];

    if (pipe(signal_pipe) < 0) {
        printf(1, "Error: Failed to create signal pipe\n");
        exit();
    }

    // Run 'printprimes 3 &' in the background
    printf(1, "Starting 'printprimes 3 &' in the background\n");
    child_pid = fork();
    if (child_pid == 0) { // Child process to execute printprimes
        close(signal_pipe[0]); // Close the read end in the child process

        // Convert the write-end file descriptor to a string
        char write_fd_str[10];
        int_to_string(signal_pipe[1], write_fd_str);

        char *args[] = {"printprimes", "3", write_fd_str, 0};
        exec("printprimes", args);
        printf(1, "Error: exec printprimes failed\n");
        exit();
    } else if (child_pid > 0) {
        close(signal_pipe[1]); // Close the write end in the parent process

        // Wait for the "done" signal from printprimes
        char signal_buffer[5];
        int bytes_read = read(signal_pipe[0], signal_buffer, 4);
        if (bytes_read != 4) {
            printf(1, "Error: Did not receive expected signal from printprimes\n");
            exit();
        }
        signal_buffer[4] = '\0';
        if (strcmp(signal_buffer, "done") == 0) {
            printf(1, "\nAll child processes of printprimes have been created and are running.\n");
        } else {
            printf(1, "Error: Received unexpected signal from printprimes: %s\n", signal_buffer);
            exit();
        }
        close(signal_pipe[0]); // Close the read end

        // Set the priority of the printprimes process using the `nice` command
        if (fork() == 0) { // Child process to execute nice
            char *args[] = {"nice", "5", "1", 0};
            printf(1, "\nExecuting 'nice 5 1'\n");
            exec("nice", args);
            printf(1, "Error: exec nice failed\n");
            exit();
        }
        wait(); // Wait for the nice command to complete

        // Wait for printprimes to finish its execution
        wait();
    } else {
        printf(1, "Error: fork failed\n");
        exit();
    }

    exit();
}
