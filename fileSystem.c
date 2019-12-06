//
// Created by Jiayi Yin on 11/30/2019.
//
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

#include "fileSystem.h"
#include "fileSystem2.h"
#include "disk.h"

BootSector* bs;     //vcb* disk_vcb
MetaData** md;      //dirent
FAT** ft;           //fatent** fatents
FAT* ft1;

int make_fs(char *disk_name, int size){
    if(make_disk(disk_name) != 0 || open_disk(disk_name) != 0){
        perror("Could not be created, opened, or properly initialized.");
        return -1;
    }

    //If works, initialize meta-data in
    make_disk(disk_name);
    open_disk(disk_name);   //Creates disk with 4096 blocks/array called "buf" (from disk.c)

    //initialize disk and boot sector
    BootSector bs1;
    bs1.blockSize = BLOCK_SIZE;
    bs1.dirStart = 1;
    int dirEntry = ((size - 1) / 2.25);
    bs1.dirLength = ceil(dirEntry / 4);  //4 directory entry per block
    bs1.fatStart = bs1.dirLength + 1;
    bs1.dataBlockStart = bs1.fatStart + bs1.fatSize;
    bs1.validFile = 0;

    //write to the disk
    memset(disk_name, 0, BLOCK_SIZE);
    memcpy(disk_name, &bs1, sizeof(BootSector));

    if(block_write(0, disk_name) != 0){
        perror("make_fs error: error writing to disk");
        return -1;
    }

    //initialize directory entry
    MetaData md1;
    md1.valid = 0;

    memset(disk_name, 0, BLOCK_SIZE);
    for(int i = 0; i < 4; i++){
        memcpy(disk_name + i * sizeof(MetaData), &md1, sizeof(MetaData));
    }

    //blank directory entry
    for(int j = bs1.dirStart; j < bs->dirLength + bs->dirStart; j++){
        if(block_write(j, disk_name) != 0){
            perror("make_fs error: failed writing disk");
        }
        block_write(j, disk_name);   //First block is always the boot sector: info for whole file system
                                //Writes the filesystem name onto block 0 in disk disk_name (Initializes metadata)
    }

    //initialize FAT
    FAT fat;
    fat.used = 0;

    memset(disk_name, 0, BLOCK_SIZE);
    memcpy(disk_name, &fat, sizeof(FAT));

    //write blank FAT for later use
    for(int i = bs1.fatStart; i < bs1.fatStart + bs1.fatSize; i++){
        if(block_write(i, disk_name) < 0){
            perror("make_fs error: failed writing to disk");
            return -1;
        }
        block_write(i, disk_name);
    }
    return 0;
}

int mount_fs(char *disk_name){
    if(make_disk(disk_name) != 0 || open_disk(disk_name) != 0){
        perror("Mount error: Cannot be opened or doesn't contain the valid fs");
        return -1;
    }

    //Data blocks
    open_disk(disk_name);
    block_read(0, disk_name);
    if(block_read(0, disk_name) < 0){
        perror("Mount error: Read failed for block");
        return -1;
    }

    //Put data from disk to data[]
    bs = calloc(1, sizeof(BootSector));     //Allocates memory and initialize memory block to 1
    memcpy(bs, &disk_name, sizeof(BootSector));  //Copy data from disk to data[]

    //FAT entries
    for(int i = bs->fatStart; i < bs->fatStart + bs->fatSize; i++){
        if(block_read(i, disk_name) < 0){
            perror("Mount error: read failed for FAT entry");
        }
        ft1 = calloc(1, sizeof(FAT));    //Allocates memory and initialize memory block to 1
        memcpy(ft1, &disk_name, sizeof(FAT));     //Copy data from FAT to fatEntry[]

        ft = realloc(ft, (i - bs->fatStart + 1) * sizeof(FAT*));    //Allocates memory for FAT entry
        ft[i - bs->fatStart] = ft1;     //Transfers data to the ft[]
    }
    return 0;
}

int unmount_fs(char *disk_name){
    if(close_disk(disk_name) != 0 || block_write(0, disk_name) != 0){
        perror("Unmount error: cannot be closed or no data written");
        return -1;
    }

    block_write(0, disk_name);     //writes boot sector (all the metadata information) to disk
    close_disk(disk_name);
    return 0;
}

//Include file descriptor
int fs_open(char *name){
    int index1;
    MetaData* md1 = alloca(sizeof(MetaData));
    if(strrchr(name, '/') > name){
        perror("fs_open error: unable to get multi-level dir");
        return -1;
    }

    //finds dir entry by name
    int dirEntry = findDir(md1, bs, name);
    if(dirEntry == -1){
        perror("fs_open error: file name could not be found\n");
        return -1;
    }

    //Gets the info from dir entries
    else{
        //first char '/' is a root dir
        if(*name == '/' && *(name + 1) == '\0'){
            bs->fileName = name;
            bs->fileSize = sizeof(bs);
            bs->blocks = 1; //Only takes 1 block
        }
        else{
            md1->fileName = name;
            md1->size = md1->size;
            md1->blocks = ceil(md1->size / BLOCK_SIZE); //file size within the blocks
        }
    }
    return 0;
}

int fs_close(int fildes){

}

int fs_create(char *name){
    if(strrchr(name, '/') > name){
        perror("fs_create: unable to create on multilevel dir\n");
        return -1;
    }

    //find unused dir entry
    int used = 1;
    int i;
    MetaData* md1 = alloca(sizeof(MetaData));
    for(i = 0; i < bs->dirLength; i++){
        dirEntryDisk(md1, bs, i);
        if(md1->valid == 0){
            used = 0;
            break;
        }
    }

    //when dir entry is full
    if(used == 1){
        perror("fs_create error: No more avaliable directory entries\n");
        return -1;
    }

    //add new file to dir entry
    md1->fileName = name;
    memcpy(md1->fileName, name, strlen(name) + 1);
    md1->valid = 1;
    md1->firstBlock =newFAT(ft, bs);    //new FAT entry for the new block
    updateDir(md1, bs, i);

    //update the number of files in disk
    bs->validFile+=bs->validFile + 1;

    return 0;
}

int fs_delete(char *name){
    int index1;
    if(strrchr(name, '/') > name){
        perror("fs_delete error: Unable to delete");
        return -1;
    }

    MetaData* md1 = alloca(sizeof(MetaData));
    int dirEntry = findDir(md1, bs, name);

    //dir entry doesn't exist
    if(dirEntry ==-1){
        perror("fs_delete error: directory entry name doesn't exist");
        return -1;
    }

    md1->valid = 0;
    updateDir(md1, bs, index1);  //update dir entry

    //unused FAT entry
    unsigned int b = md1->firstBlock;
    while(!ft[b]->end){
        ft[b]->used = 0;
        b = ft[b]->next;
    }

    if(ft[b]->end){
        ft[b]->used = 0;
    }

    //Decrease # of files
    bs->validFile--;
    return 0;
}

int fs_mkdir(char *name){

}

int fs_read(int fildes, void *buf, size_t nbyte){
    int size = nbyte;

    //access file directory entry
    MetaData* md1 = alloca(sizeof(MetaData));

}

int fs_write(int fildes, void *buf, size_t nbyte){

}

int fs_get_filesize(int fildes){

}

int fs_lseek(int fildes, off_t offset){

}

int fs_truncate(int fildes, off_t length){

}



