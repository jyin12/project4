//
// Created by Jiayi Yin on 12/1/2019.
//

#ifndef PROJECT4_SHELL_H
#define PROJECT4_SHELL_H

void makeFs(int args, char *arg1, char *arg2);
void mountFs(int args, char *arg1, char *arg2);
void unmountFs(int args, char *arg1, char *arg2);
void openFs(int args, char *arg1, char *arg2);
void closeFs(int args, char *arg1, char *arg2);
void createFs(int args, char *arg1, char *arg2);
void deleteFs(int args, char *arg1, char *arg2);
/*void mkdirFs(int args, char *arg1, char *arg2);
void readFs(int args, char *arg1, char *arg2);
void writeFs(int args, char *arg1, char *arg2);
void getSize(int args, char *arg1, char *arg2);
void lseekFs(int args, char *arg1, char *arg2);
void truncateFs(int args, char *arg1, char *arg2);*/
void help();
#endif //PROJECT4_SHELL_H
