#include "monitor.cpp"

philosopher_monitor monitor;

void *sit(void *i){
    long philo_num=(long)i;
    printf("Philosopher %ld is THINKING\n",philo_num);
    while(1){
        monitor.think();
        monitor.take_chops(philo_num);
        monitor.eat();
        monitor.put_chops(philo_num);
    }
}

int main(){
    int join_index;
    long ini_index;
    pthread_t* philosopher=(pthread_t *)malloc(N*sizeof(pthread_t));

    for(ini_index=0;ini_index<N;++ini_index){
        pthread_create(&philosopher[ini_index],NULL,sit,(void*)ini_index);
    }

    for(join_index=0;join_index<N;++join_index){
        pthread_join(philosopher[join_index],NULL);
    }

    printf("All test done,exit program\n");
    return 0;
}