#include <sys/types.h> 
#include <sys/wait.h>
#include"write.c"
#include"read.c"

int main(){
    pid_t pid=fork();

    if(pid>0){
        writeFile();
        waitpid(pid,NULL,0);
    }
    else{
        char sig='n';
        while(sig!='y' && sig!='Y'){
            printf("\nReady for read? y/n:");
            scanf(" %c",&sig);
            // if(sig=='y' || sig=='Y') break;
        }
        readFile();
    }
    return 0;
}