//
// Created by Jiayi Yin on 12/5/2019.
//

#ifndef PROJECT4_FILESYSTEM2_H
#define PROJECT4_FILESYSTEM2_H

#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <assert.h>
#include <sys/statfs.h>
#include <alloca.h>

#endif //PROJECT4_FILESYSTEM2_H


int newFAT(FAT** ft, BootSector* bs);   //creates new FAT entry
int findDir(MetaData* md, BootSector* bs, char *name);  //finds dir entry by name
void dirEntryDisk(MetaData* md, BootSector* bs, int i); //finds dir entry on disk
void updateDir(MetaData* md, BootSector* bs, int i);    //updates dir entry
void getBlock(char *buf, int i);    //retrieves the block given by index and buf