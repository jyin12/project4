#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>

#include "disk.h"
#include "fileSystem.h"
#include "shell.h"

int main(int argc, char **argv) {
    do{
        char line[30], cmd[30], arg1[30], arg2[30];
        printf("myShell> ");

        if(fgets(line, 10, stdin) == NULL)
            perror("Enter again");

        int args = sscanf(line, "%s %s %s", cmd, arg1, arg2);
        if(args == 0)
            continue;

        if((strcmp(cmd, "make")) == 0){
            makeFs(args, arg1, arg2);
        }
        else if((strcmp(cmd, "mount")) == 0){

        }
        else if((strcmp(cmd, "unmount")) == 0){

        }
        else if((strcmp(cmd, "open")) == 0){

        }
        else if((strcmp(cmd, "close")) == 0){

        }
        else if((strcmp(cmd, "create")) == 0){

        }
        else if((strcmp(cmd, "delete")) == 0){

        }
        else if((strcmp(cmd, "mkdir")) == 0){

        }
        else if((strcmp(cmd, "read")) == 0){

        }
        else if((strcmp(cmd, "write")) == 0){

        }
        else if((strcmp(cmd, "size")) == 0){

        }
        else if((strcmp(cmd, "lseek")) == 0){

        }
        else if((strcmp(cmd, "truncate")) == 0){

        }
        else if((strcmp(cmd, "help")) == 0){
            help();
        }
        else if((strcmp(cmd, "exit")) == 0 || (strcmp(cmd, "quit")) == 0){
            break;
        }
        else{
            printf("Unknown command: %s", line);
            printf("Type 'help' for list of commands\n");
        }
    } while(true);


    return 0;
}