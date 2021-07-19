#ifndef MONITOR_H
#define MONITOR_H

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>

#define N 5
#define LEFT (i+N-1)%N
#define RIGHT (i+1)%N
#define THINKING 0
#define HUNGRY 1
#define EATING 2

class philosopher_monitor{
    private:
        int state[N];
        pthread_mutex_t mutex;
        pthread_cond_t chop[N];
    public:
        philosopher_monitor();
        void test(int i);
        void take_chops(int i);
        void put_chops(int i);
        void think();
        void eat();
};

#endif