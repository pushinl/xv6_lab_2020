#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]){
    if(argc == 1) {
        printf("Error: Missing argument\n");
    } else if (argc == 2)
    {
        sleep(atoi(argv[1]));
    } else {
        printf("Error: Extra argument\n");
    }
    exit(0);
}