#include "mm.c"

void print(int *ptr,int size){
    printf("Base address:%p\n",ptr);
    for(int i=0;i<size;i++){
        printf("%d \n",ptr[i]);
    }
}

void test_model(int mode,int size){
    int *array;
    if(mode==1){
        printf("mymalloc test(size=10):\n");
        array=(int *)mymalloc(sizeof(int)*size);
    }
    else if(mode==2){
        printf("myrealloc test(base size=10,+/- 5 size for realloc):\n");
        array=(int *)mymalloc(sizeof(int)*size);
    }
    else if(mode==3){
        printf("mycalloc test(size=10):\n");
        print(array,size);
    }
    for(int i=0;i<size;i++) array[i]=rand()%10;
    print(array,size);
    if(mode==2){
        printf("size +5:\n");
        array=(int*)myrealloc(array,sizeof(int)*(size+5));
        print(array,size+5);
        printf("size -5:\n");
        array=(int*)myrealloc(array,sizeof(int)*(size-5));
        print(array,size-5);
    }
    printf("End of test\n\n\n");
    myfree(array);
}

int main(){
    srand(time(NULL));
    for(int i=1;i<4;i++){
        test_model(i,10);
    }
}