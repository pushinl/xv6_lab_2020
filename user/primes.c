#include "kernel/types.h"
#include "user/user.h"

#define FDRD 0
#define FDWT 1

void primes(int *input, int num) {
    if (num == 1) {
        printf("prime %d\n", *input);
        return;
    }
    int fd[2], i;
    int prime = *input;
    int temp;
    printf("prime %d\n", prime);
    pipe(fd);
    int pid = fork();
    if (pid > 0) {
        for (i = 0; i < num; i++) {
            temp = *(input + i);
            write(fd[FDWT], (char *)(&temp), 4);
        }
        //exit(0);
    }
    close(fd[FDWT]);
    if (pid == 0) {
        int counter = 0;
        char buffer[4];
        while (read(fd[FDRD], buffer, 4) != 0) {
            temp = *((int *)buffer);
            if (temp % prime != 0) {
                *input = temp;
                input += 1;
                counter++;
            }
        }
        primes(input - counter, counter);
        exit(0);
    }
    wait(0);
    exit(0);
}

int main() {
    int input[34], i;
    for (i = 0; i < 34; i++) {
        input[i] = i + 2;
    }
    primes(input, 34);
    exit(0);
}