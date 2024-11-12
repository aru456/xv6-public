// task2_test3.c
#include "types.h"
#include "stat.h"
#include "user.h"

void itoa(int num, char *str) {
    int i = 0;
    if (num == 0) {
        str[i++] = '0';
    } else {
        // Handle each digit and convert to string in reverse order
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
        printf(1, "Error: Unable to create pipe\n");
        exit();
    }

    // Run 'primeproc 3 &' in the background
    printf(1, "Running 'primeproc 3 &' in the background\n");
    pid = fork();
    if (pid == 0) { // Child process to execute primeproc
        close(pipe_fd[0]); // Close read end in child

        // Convert write end file descriptor to string
        char fd_str[10];
        itoa(pipe_fd[1], fd_str);

        char *args[] = {"primeproc", "3", fd_str, 0};
        exec("primeproc", args);
        printf(1, "Error: exec primeproc failed\n");
        exit();
    } else if (pid > 0) {
        close(pipe_fd[1]); // Close write end in parent

        // Wait for the signal from primeproc
        char buf[5];
        int n = read(pipe_fd[0], buf, 4);
        if (n != 4) {
            printf(1, "Error: Did not receive proper signal from primeproc\n");
            exit();
        }
        buf[4] = '\0';
        if (strcmp(buf, "done") == 0) {
            printf(1, "\nAll children of primeproc are created and running.\n");
        } else {
            printf(1, "Error: Received unexpected signal from primeproc: %s\n", buf);
            exit();
        }
        close(pipe_fd[0]); // Close read end

        // Run 'nice 7 <pid>' where <pid> is the PID of primeproc
        char pid_str[10];
        itoa(pid, pid_str);
        printf(1, "\nRunning 'nice 7 %s'\n", pid_str);
        if (fork() == 0) { // Child process to execute nice
            char *args[] = {"nice", "7", "3", 0};
            exec("nice", args);
            printf(1, "Error: exec nice failed\n");
            exit();
        }
        wait(); // Wait for nice to complete

        // Optionally, run 'ps' to display process statuses
        printf(1, "\nRunning 'ps' to display process statuses\n");
        if (fork() == 0) { // Child process to execute ps
            char *args[] = {"ps", 0};
            exec("ps", args);
            printf(1, "Error: exec ps failed\n");
            exit();
        }
        wait(); // Wait for ps to complete

        // Wait for primeproc to finish
        wait();
    } else {
        printf(1, "Error: fork failed\n");
        exit();
    }

    exit();
}