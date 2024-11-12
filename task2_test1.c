#include "types.h"
#include "stat.h"
#include "user.h"

// Integer to ASCII conversion
void itoa(int num, char *str) {
    int i = 0;
    if (num == 0) {
        str[i++] = '0';
    } else {
        int temp = num;
        while (temp != 0) {
            str[i++] = (temp % 10) + '0';
            temp /= 10;
        }
    }
    str[i] = '\0';

    // Reverse the string
    for (int j = 0; j < i / 2; j++) {
        char swap = str[j];
        str[j] = str[i - j - 1];
        str[i - j - 1] = swap;
    }
}

int main() {
    int pid;
    int pipe_fd[2];

    if (pipe(pipe_fd) < 0) {
        printf(1, "Pipe creation failed\n");
        exit();
    }

    // Start printprimes in the background with argument 3
    printf(1, "Launching 'printprimes 4 &' as a background process\n");
    pid = fork();
    if (pid == 0) { // Child process to execute printprimes
        close(pipe_fd[0]); // Close read end in child

        // Convert write-end file descriptor to string
        char fd_str[10];
        itoa(pipe_fd[1], fd_str);

        char *args[] = {"printprimes", "4", fd_str, 0};
        exec("printprimes", args);
        printf(1, "Error: exec for printprimes failed\n");
        exit();
    } else if (pid > 0) {
        close(pipe_fd[1]); // Close write end in parent

        // Wait for completion signal from printprimes
        char buf[5];
        int n = read(pipe_fd[0], buf, 4);
        if (n != 4) {
            printf(1, "Error: Signal from printprimes was not received correctly\n");
            exit();
        }
        buf[4] = '\0';
        if (strcmp(buf, "done") == 0) {
            printf(1, "\nprintprimes has successfully created all child processes.\n");
        } else {
            printf(1, "Error: Unexpected message from printprimes: %s\n", buf);
            exit();
        }
        close(pipe_fd[0]); // Close read end

        char pid_str[10];
        itoa(pid, pid_str);
        if (fork() == 0) { // Child process to execute ps
            char *args[] = {"ps", 0};
            printf(1, "\nExecuting 'ps' to display process status\n");
            exec("ps", args);
            printf(1, "Error: exec for ps failed\n");
            exit();
        }
        wait(); // Wait for ps command to complete

        // Wait for printprimes to finish
        wait();
    } else {
        printf(1, "Error: fork operation failed\n");
        exit();
    }

    exit();
}
