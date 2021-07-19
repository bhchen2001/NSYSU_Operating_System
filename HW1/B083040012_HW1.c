#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>

#include "command_line_processing.h"
#include "command_execute.h"

void type_prompt(){
    char dir[50];
    getcwd(dir,sizeof(dir));
    printf("%s~B083040012~$",dir);
}

int main(){
    pid_t pid;
    char *command,**command_set;
    int pip_num;
    while(1){
        type_prompt();
        command=readCommand();
        // printf("read done\n");
        // if(!command) continue;
        if(!command || command[strlen(command)-1]=='\0' || !strcmp(command,"|")) continue;
        command_set=commandPipSlice(command,&pip_num);
        // printf("%s\n",command_set[0]);
        // int i=0;
        // for(i=0;command_set[i];i++){
        //     printf("command_set:%s\n",command_set[i]);
        // }
        commandExecute(command_set,pip_num);
        // printf("pip_num:%d\n",pip_num);

    }
    // char*command[3];
    // command[0]="ls";
    // command[1]="-al";
    // command[2]=NULL;
    // execvp(command[0],command);
}
