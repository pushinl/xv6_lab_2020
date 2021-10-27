#include "kernel/types.h"
#include "user/user.h"
#include "kernel/stat.h"
#include "kernel/fs.h"

char *findFileName(char* path) {
    char *p;
    for(p = path + strlen(path); p >= path && *p != '/'; p--);
    p++;
    return p;
}

void find(char* path, char* target) {
    char buffer[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if((fd = open(path, 0)) < 0) {
        printf("Error: cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0) {
        printf("Error: cannot open %s\n", path);
        return;
    }

    switch(st.type) {
        case T_FILE:
            if(!strcmp(findFileName(path), target)) printf("%s\n", path);
            break;
        case T_DIR:
            if(strlen(path) + 1 + DIRSIZ + 1 > sizeof(buffer)) {
                printf("Error: Find path too long\n");
                break;
            }
            strcpy(buffer, path);
            p=buffer + strlen(buffer);
            *p++ = '/';
            while(read(fd, &de ,sizeof(de)) == sizeof(de)) {
                if(!de.inum || !strcmp(de.name, ".") || !strcmp(de.name, "..")) continue;
                memmove(p,de.name, DIRSIZ);
                p[DIRSIZ] = 0;
                if(stat(buffer, &st) < 0) {
                    printf("Error: cannot stat %s\n", buffer);
                    continue;
	            }
	            find(buffer, target);
            }
            break;
    }
    close(fd);
}

int main(int argc, char *argv[]) {
    if(argc != 3) {
        printf("Error: Insufficient argument\n");	  
        exit(1);
    }
    find(argv[1], argv[2]);
    exit(0);
}