//
// Created by Jiayi Yin on 11/30/2019.
//

#ifndef PROJECT4_FILESYSTEM_H
#define PROJECT4_FILESYSTEM_H

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

#endif //PROJECT4_FILESYSTEM_H


//FAT entries
typedef struct FAT_s{
    unsigned int used;    //Marks with FAT block are used
    unsigned int end;     //Tells where the last FAT pointer block is
    unsigned int next;   //Tells where the next pointer is
}FAT;

//Boot sector is metadata for entire file system
typedef struct BootSector_s{
  char *fileSysName;    //Refers to root directory???
  int blockSize;
  int fatSize;     //Size of the entire files system used
  int fatStart; //Where the FAT starts in data block (array made in the mount_fs() method)
  int validFile;
  int dirStart;   //Which block the dir entry starts
  int dirLength;    //Size of dir in the file system used
  int dataBlockStart;   //Where the data block starts
  int blocks;

  //meta data for root dir (name root dir "//" and other files with a name)
  char *fileName;   //Name of file
  int fileSize;     //Size of file
}BootSector;

//For all other directory (name root dir "//" and other files with a name)
typedef struct MetaData_s{
    char *fileName;   //Name of file
    unsigned int size;   //Size of file
    unsigned int firstBlock;   //Where the first file is located in block
    unsigned int valid;
    int blocks;
}MetaData;

//Function implementation
int make_fs(char *disk_name, int size);
int mount_fs(char *disk_name);
int unmount_fs(char *disk_name);
int fs_open(char *name);
int fs_close(int fildes);
int fs_create(char *name);
int fs_delete(char *name);
int fs_mkdir(char *name);
int fs_read(int fildes, void *buf, size_t nbyte);
int fs_write(int fildes, void *buf, size_t nbyte);
int fs_get_filesize(int fildes);
int fs_lseek(int fildes, off_t offset);
int fs_truncate(int fildes, off_t length);