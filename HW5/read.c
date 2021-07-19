#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include <fcntl.h> 
#include <time.h>
#include <sys/mman.h> 
#include <sys/stat.h>
#include<string.h>

#define TEXT_SIZE 100

void readFile(){
    int fd=open("text",O_RDWR,S_IWUSR | S_IRUSR);
    if(fd<0){
        printf("ERROR to read text\n");
        exit(EXIT_FAILURE);
    }
    printf("Read Text done\n");

    void *ptr=mmap(NULL,getpagesize(),PROT_READ,MAP_SHARED,fd,0);
    // printf("map\n");
    if(ptr==MAP_FAILED){
        printf("ERROR in mmap(read)\n");
        close(fd);
        exit(EXIT_FAILURE);
    }
    close(fd);
    printf("mmap(read) done , fd closed");

    // struct stat tmp = {0};
    // fstat(fd,&tmp);
    // int len = (int)tmp.st_size;

    printf("\nRead ready...\n\n");
    // sleep(10);
    char result[100];
    memcpy(result,ptr,TEXT_SIZE);
    printf("Read result:%s\n",result);
    munmap(ptr,TEXT_SIZE);
}