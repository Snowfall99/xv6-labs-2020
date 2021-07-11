#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(void) {
    int fds_1[2];
    int fds_2[2];
    int pid;
    char buf[100];

    pipe(fds_1);
    pipe(fds_2);

    pid = fork();

    if (pid < 0) {
        fprintf(2, "fork error\n");
        exit(1);
    } else if (pid == 0) {
        close(fds_1[1]);
        close(fds_2[0]);
        read(fds_1[0], buf, sizeof(buf));
        printf("%d: received %s\n", getpid(), buf);
        write(fds_2[1], "pong", 4);
        close(fds_2[1]);
    } else {
        close(fds_1[0]);
        close(fds_2[1]);
        write(fds_1[1], "ping", 4);
        close(fds_1[1]);
        read(fds_2[0], buf, sizeof(buf));
        printf("%d: received %s\n", getpid(), buf);
    }
    exit(0);
}