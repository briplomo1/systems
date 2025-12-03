#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
	pid_t children[5];
    int i;

    for (i = 0; i < 5; i++) {
        const pid_t pid = fork();

        if (pid < 0) {
            perror("ERROR: fork failed");
            exit(1);
        }

        if (pid == 0) {
            printf("Child Process %d, PID: %d, PPID: %d\n", i+1, getpid(), getppid());
            fflush(stdout);
            sleep(1);
            exit(0);
        }
        children[i] = pid;
    }

    for (i = 0; i < 5; i++) {
        int status;
        waitpid(children[i], &status, 0);
    }

    return 0;
}