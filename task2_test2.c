#include "types.h"
#include "stat.h"
#include "user.h"

// Integer to ASCII conversion function
void int_to_string(int num, char *str) {
    int idx = 0;
    if (num == 0) {
        str[idx++] = '0';
    } else {
        int temp_num = num;
        while (temp_num != 0) {
            str[idx++] = (temp_num % 10) + '0';
            temp_num /= 10;
        }
    }
    str[idx] = '\0';

    // Reverse the string
    for (int i = 0; i < idx / 2; i++) {
        char temp = str[i];
        str[i] = str[idx - i - 1];
        str[idx - i - 1] = temp;
    }
}

int main() {
    int child_pid;
    int status_pipe[2];

    if (pipe(status_pipe) < 0) {
        printf(1, "Error: Unable to create status pipe\n");
        exit();
    }

    // Run 'printprimes 3 &' in the background
    printf(1, "Launching 'printprimes 3 &' in the background\n");
    child_pid = fork();
    if (child_pid == 0) { // Child process to execute printprimes
        close(status_pipe[0]); // Close read end in child

        // Convert write end file descriptor to string
        char write_fd_str[10];
        int_to_string(status_pipe[1], write_fd_str);

        char *args[] = {"printprimes", "3", write_fd_str, 0};
        exec("printprimes", args);
        printf(1, "Error: exec printprimes failed\n");
        exit();
    } else if (child_pid > 0) {
        close(status_pipe[1]); // Close write end in parent

        // Wait for the signal from printprimes
        char signal_buf[5];
        int bytes_read = read(status_pipe[0], signal_buf, 4);
        if (bytes_read != 4) {
            printf(1, "Error: Did not receive expected signal from printprimes\n");
            exit();
        }
        signal_buf[4] = '\0';
        if (strcmp(signal_buf, "done") == 0) {
            printf(1, "\nAll child processes of printprimes are created and running.\n");
        } else {
            printf(1, "Error: Received unexpected signal from printprimes: %s\n", signal_buf);
            exit();
        }
        close(status_pipe[0]); // Close read end

        // Adjust priority using `nice` command for a specific child process
        if (fork() == 0) { // Child process to execute nice
            char *args[] = {"nice", "5", "5", 0};
            printf(1, "\nExecuting 'nice 5 5'\n");
            exec("nice", args);
            printf(1, "Error: exec nice failed\n");
            exit();
        }
        wait(); // Wait for nice command to complete

        // Wait for printprimes to finish
        wait();
    } else {
        printf(1, "Error: fork failed\n");
        exit();
    }

    exit();
}
