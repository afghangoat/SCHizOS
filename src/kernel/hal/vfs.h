#pragma once
//#include <stdint.h>
//TODO
//typedef int fd_t; //file descript. id

#define VFS_FD_STDIN 0
#define VFS_FD_STDOUT 1
#define VFS_FD_STDERR 2
#define VFS_FD_DEBUG 3

//int VFS_Write(fd_t file,uint8_t* data, int size);