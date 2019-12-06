//
// Created by Jiayi Yin on 12/5/2019.
//

#include "fileSystem.h"
#include "disk.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>


int newFAT(FAT** ft, BootSector* bs){
    int j = 0;
    int i;
    for(i = 0; i < bs->fatSize; i++){
        if(!ft[i]->used){
            j = 1;
            break;
        }
    }
    if(!j)
        return -1;
    ft[i]->used = 1;
    ft[i]->end = 1;

    //free data
    char* buf = calloc(BLOCK_SIZE, 1);
    block_write(bs->dataBlockStart + i, buf);
    free(buf);

    return i;
}

int findDir(MetaData* md, BootSector* bs, char *name){
    //in dir entry, looks for the valid and matching dir name
    for(int i = 0; i < bs->dirLength && bs->validFile > 0; i++){
        MetaData* md1 = alloca(sizeof(MetaData));
        dirEntryDisk(md1, bs, i);
        if((md1->valid == 1) && (strcmp(name, md1->fileName) == 0)){
            *md = *md1;
            return i;
        }
        else if(md1->valid ){
            bs->validFile = bs->validFile - 1;
        }
    }
    return -1; //if failed
}

void dirEntryDisk(MetaData* md, BootSector* bs, int i){
    int blockIndex = bs->dirStart + (i / 4);    //4 dir entry per block
    char buf[BLOCK_SIZE];
    getBlock(buf, blockIndex);
    memcpy(md, buf + (i % 4) * sizeof(MetaData), sizeof(MetaData)); //Copies data over to buf[]
}

void updateDir(MetaData* md, BootSector* bs, int i){
    int blockIndex = bs->dirStart + (i / 4);
    char buf[BLOCK_SIZE];
    getBlock(buf, blockIndex);

    //puts the new data in the md
    memcpy(buf + (i % 4) * sizeof(MetaData), md, sizeof(MetaData));

    //writes back to disk
    block_write(i, buf);
}

void getBlock(char *buf, int i){
    if(block_read(i, buf) < 0){
        perror("getBlock error: read failed for buf");
    }
}

