#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid;
    pid_t children[5];
    int i;

    for (i = 1; i <= 5; i++) {
        pid = fork();

        if (pid < 0) {
            perror("ERROR: fork failed");
            exit(1);
        }

        if (pid == 0) {
            printf("Child PID: %d, Parent PID: %d, Child num: %d\n", getpid(), getppid(), i);
            fflush(stdout);
            sleep(1);
            exit(0);
        }

        children[i] = pid;
    }

    for (i = 1; i <= 5; i++) {
        int status;
        pid_t pid = waitpid(children[i], &status, 0);
        if (wpid == -1) {
            perror("ERROR: Wait failed");
        } else {
            printf("Parent: child %d (PID=%d) finished.\n", i + 1, wpid);
        }
    }

    printf("Parent %d: all children finished.\n", getpid());

    return 0;
}