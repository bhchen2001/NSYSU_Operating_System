// #ifndef MONITOR_CPP
// #define MONITOR_CPP

#include"monitor.h"

// #include<stdio.h>
// #include<stdlib.h>
// #include<pthread.h>
// #include<unistd.h>

// #define N 5
// #define LEFT (i+N-1)%N
// #define RIGHT (i+1)%N
// #define THINKING 0
// #define HUNGRY 1
// #define EATING 2

// class philosopher_monitor{
//     private:
//         int state[N];
//         pthread_mutex_t mutex;
//         pthread_cond_t chop[N];
//     public:
//         philosopher_monitor();
//         void test(int i);
//         void take_chops(int i);
//         void put_chops(int i);
//         void think();
//         void eat();
// };

philosopher_monitor::philosopher_monitor(){
    pthread_mutex_init(&mutex,NULL);
    for(int ini_index=0;ini_index<N;++ini_index){
        pthread_cond_init(&chop[ini_index],NULL);
    }
}

void philosopher_monitor::test(int i){
    if(state[i]==HUNGRY && state[LEFT]!=EATING && state[RIGHT]!=EATING){
        state[i]=EATING;
        printf("Philosopher %d is EATING\n",i);
        pthread_cond_signal(&chop[i]);
    }
}

void philosopher_monitor::take_chops(int i){
    pthread_mutex_lock(&mutex);
    state[i]=HUNGRY;
    printf("Philosopher %d is HUNGRY\n",i);
    test(i);
    if (state[i]==HUNGRY)
		pthread_cond_wait(&chop[i], &mutex);
    pthread_mutex_unlock(&mutex);
}

void philosopher_monitor::put_chops(int i){
    pthread_mutex_lock(&mutex);
    state[i]=THINKING;
    printf("Philosopher %d is THINKING\n",i);
    test(LEFT);
    test(RIGHT);
    pthread_mutex_unlock(&mutex);
}

void philosopher_monitor::think(){
    usleep(1000000);
}

void philosopher_monitor::eat(){
    usleep(1000000);
}

// #endif