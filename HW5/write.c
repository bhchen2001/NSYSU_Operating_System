#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include <fcntl.h> 
#include <time.h>
#include <sys/mman.h> 
#include <sys/stat.h>
#include<string.h>

#define TEXT_SIZE 100

void writeFile(){
    int fd=open("text",O_RDWR | O_CREAT,S_IWUSR | S_IRUSR);
    if(fd<0){
        printf("ERROR to create text\n");
        exit(EXIT_FAILURE);
    }
    printf("Text create done\n");

    lseek(fd,TEXT_SIZE+1,SEEK_SET);
	write(fd,"", 1);
	lseek(fd,0,SEEK_SET);

    // lseek(fd,TEXT_SIZE,SEEK_SET);
    // write(fd, "", 1);

    void *ptr=mmap(NULL,TEXT_SIZE,PROT_WRITE,MAP_SHARED,fd,0);
    if(ptr==MAP_FAILED){
        printf("ERROR in mmap(write)\n");
        close(fd);
        exit(EXIT_FAILURE);
    }
    close(fd);
    printf("mmap(write) done , fd closed\n");
    
    // struct stat tmp = {0};
    // fstat(fd,&tmp);
    // int len = (int)tmp.st_size;

    // struct stat tmp;
    // int len=(int)(fstat(fd,&tmp).st_size);

    // while(1){
    //     printf("Input(q for quit):");
    //     scanf("%s",input);
    //     printf("Input:",input);
    //     if(input=="q") break;
    //     lseek(fd,len+strlen(input),SEEK_SET);
    //     write(fd,"",1);
    //     lseek(fd,0,SEEK_SET);
    //     memcpy(ptr,input,strlen(input));
    //     len+=strlen(input);
    // }
    char input[TEXT_SIZE]="write_test";
    // printf("Input:");
    // scanf("%s",input);
    printf("write word [write_test] into mapped memory\n");
    memcpy(ptr,input,strlen(input));
    // sleep(10);
    munmap(ptr,TEXT_SIZE);
}