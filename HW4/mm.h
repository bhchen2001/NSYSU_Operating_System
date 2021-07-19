#ifndef __MY_MM_H_INCLUDED__
#define __MY_MM_H_INCLUDED__

#include<stddef.h>
#include<stdio.h>
#include<stddef.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

void *myalloc(size_t size);
void myfree(void *ptr);
void *myrealloc(void *ptr,size_t size);
void *mycalloc(size_t nmemb,size_t size);

#endif