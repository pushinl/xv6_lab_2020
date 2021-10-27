#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char* argv[]) {
    if(argc > 1) {
        printf("Error: Extra argument\n");
        exit(1);
    }
    int parent_p[2], child_p[2];//pipe[1]:write  pipe[0]:read
    if (pipe(parent_p) == -1 || pipe(child_p) == -1) {
        printf("Error: Pipe creation failed\n");
        exit(1);
    }
    int pid = fork();
    if(pid < 0) {
        printf("Error: Fork failed\n");
        exit(1);
    } else if(pid == 0) { //child
        close(parent_p[1]);
        close(child_p[0]);
        char buf[5];
        read(parent_p[0], buf, 4);
        close(parent_p[0]);
        printf("%d: received %s\n", getpid(), buf);
        write(child_p[1], "pong", strlen("pong"));
        close(child_p[1]);
    } else { //parent
        close(parent_p[0]);
        close(child_p[1]);
        write(parent_p[1], "ping", strlen("ping"));
        close(parent_p[1]);
        char buf[5];
        read(child_p[0], buf, 4);
        printf("%d: received %s\n", getpid(), buf);
        close(child_p[0]);
    }  
    exit(0);
}