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

typedef pthread_mutex_t semaphore;

int state[N];
semaphore mutex;
semaphore chop[N];

void test(int i){
    if(state[i]==HUNGRY && state[LEFT]!=EATING && state[RIGHT]!=EATING){
        state[i]=EATING;
        printf("Philosopher %d is EATING\n",i);
        pthread_mutex_unlock(&chop[i]);
    }
}

void take_chops(int i){
    pthread_mutex_lock(&mutex);
    state[i]=HUNGRY;
    printf("Philosopher %d is HUNGRY\n",i);
    test(i);
    pthread_mutex_unlock(&mutex);
    pthread_mutex_lock(&chop[i]);
}

void put_chops(int i){
    pthread_mutex_lock(&mutex);
    state[i]=THINKING;
    printf("Philosopher %d is THINKING\n",i);
    test(LEFT);
    test(RIGHT);
    pthread_mutex_unlock(&mutex);
}

void think(){
    usleep(1000000);
}

void eat(){
    usleep(1000000);
}

void *sit(void *i){
    long philo_num=(long)i;
    printf("Philosopher %ld is THINKING\n",philo_num);
    while(1){
        think();
        take_chops(philo_num);
        eat();
        put_chops(philo_num);
    }
}

int main(){
    int join_index;
    long ini_index;
    pthread_t* philosopher=(pthread_t *)malloc(N*sizeof(pthread_t));
    // pthread_t philosopher[N];
    // pthread_attr_t attr;
    // pthread_attr_init(&attr);
    pthread_mutex_init(&mutex,NULL);

    for(ini_index=0;ini_index<N;++ini_index){
        pthread_mutex_init(&chop[ini_index],NULL);
        // pthread_mutex_lock(&chop[ini_index]);
        pthread_create(&philosopher[ini_index],NULL,sit,(void*)ini_index);
    }

    // pthread_attr_destroy(&attr);

    for(join_index=0;join_index<N;++join_index){
        pthread_join(philosopher[join_index],NULL);
    }

    printf("All test done,exit program\n");
    return 0;
}