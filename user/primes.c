#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(void) {
    int numbers[36];
    int fds[2];
    int pid;
    int i;
    int index = 0;

    for (i = 2; i < 36; i ++) {
        numbers[index++] = i;
    }

    while (index > 0) {
        pipe(fds);

        pid = fork();
        if(pid < 0) {
            fprintf(2, "fork error\n");
            exit(1);
        } else if (pid > 0) {
            close(fds[0]);
            for (i = 0; i < index; i++) {
                write(fds[1], &numbers[i], sizeof(numbers[i]));
            }
            close(fds[1]);
            wait((int*)0);
            exit(0);
        } else {
            close(fds[1]);
            int prime = 0;
            int temp = 0;
            index = -1;

            while (read(fds[0], &temp, sizeof(temp)) != 0) {
                if (index < 0) {
                    prime = temp;
                    index ++;
                } else {
                    if (temp % prime != 0) {
                        numbers[index++] = temp;
                    }
                }
            }

            printf("prime %d\n", prime);
            close(fds[0]);
        }
    }

    exit(0);
}