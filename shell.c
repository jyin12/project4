//
// Created by Jiayi Yin on 12/4/2019.
//

#include <stdio.h>

#include "shell.h"
#include "fileSystem.h"
#include "disk.h"

void makeFs(int args, char *arg1, char *arg2){
    if(args != 3){
        printf("makeFs error: Invalid arguments\n");
        return;
    }
    int size = atoi(arg2);
    make_fs(arg1, size);
}
void mountFs(int args, char *arg1, char *arg2){
    if(args != 2){
        printf("mountFs error: Invalid arguments\n");
        return;
    }
    mount_fs(arg1);
}
void unmountFs(int args, char *arg1, char *arg2){
    if(args != 2){
        printf("unmountFs error: Invalid arguments\n");
        return;
    }
    unmount_fs(arg1);
}
void openFs(int args, char *arg1, char *arg2){
    if(args != 2){
        printf("openFs error: Invalid arguments\n");
        return;
    }
    fs_open(arg1);
}
//void closeFs(int args, char *arg1, char *arg2);
void createFs(int args, char *arg1, char *arg2){
    if(args != 2){
        printf("createFs error: Invalid arguments\n");
        return;
    }
    fs_create(arg1);
}
void deleteFs(int args, char *arg1, char *arg2){
    if(args != 2){
        printf("deleteFs error: Invalid arguments\n");
        return;
    }
    fs_delete(arg1);
}


/*void mkdirFs(int args, char *arg1, char *arg2);
void readFs(int args, char *arg1, char *arg2);
void writeFs(int args, char *arg1, char *arg2);
void getSize(int args, char *arg1, char *arg2);
void lseekFs(int args, char *arg1, char *arg2);
void truncateFs(int args, char *arg1, char *arg2);*/

void help(){
    printf("Commands are:\n");
    printf("     make <file>\n");
    printf("     mount\n");
    printf("     umount\n");
    printf("     open\n");
    printf("     close\n");
    printf("     create\n");
    printf("     delete <file>\n");   //from FAT
    printf("     mkdir\n");
    printf("     read <file>\n");
    printf("     write <file>\n");
    printf("     size\n");
    printf("     lseek\n");
    printf("     truncate\n");
    printf("     help\n");
    printf("     exit\n");
    printf("     quit\n");
}
