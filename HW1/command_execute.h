#ifndef COM_EXE_H
#define COM_EXE_H

#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include <sys/wait.h>

#include "command_line_processing.h"

#define STD_INPUT 0
#define STD_OUTPUT 1

static void myExecvp(char *command){
	// printf("in exe\n");
	// printf("command:%s\n",command);
	char **arg_set;
	char *part_arg,*colon_process=malloc((strlen(command)+1)*sizeof(char));
	strncpy(colon_process,command,strlen(command));
	int i;
	// printf("colon_process:%s\n",colon_process);
	part_arg=strtok(colon_process,";");
	while(part_arg){
		// printf("part_arg:%s\n",part_arg);
		char *infile=calloc(strlen(part_arg),sizeof(char)),
			 *outfile=calloc(strlen(part_arg),sizeof(char));
		infile[0]='\0'; outfile[0]='\0';
		for(i=0;i<strlen(part_arg);i++){
			int file_len=0;
			if(part_arg[i]=='<'){
                // printf("< detect\n");
				int j;
				for(j=i+2;part_arg[j]!=' ' && j<strlen(part_arg);j++){
					infile[file_len++]=part_arg[j];
				}
				infile[file_len]='\0';
				i=j;
			}
			else if(part_arg[i]=='>'){
                // printf("> detect\n");
				int j;
				for(j=i+2;part_arg[j]!=' ' && j<strlen(part_arg);j++){
                    // printf("outfile in for:%s\n",outfile);
					outfile[file_len++]=part_arg[j];
				}
                // printf("outfile:%s\n",outfile);
				outfile[file_len]='\0';
				i=j;
			}
		}
        // printf("detect done\n");
		redirect(infile,outfile);
        // printf("redirect done\n");
		arg_set=argAnalysis(part_arg);
		if(execvp(arg_set[0],arg_set)<0){
			printf("bash: %s:command not found\n",arg_set[0]);
		}
		part_arg=strtok(NULL,";");
        // printf("part_arg_after:%s\n",part_arg);
	}
    return;
}

void commandExecute(char **command_set,int pip_num){
    // printf("commmand_set:%s\n",command_set[0]);
    char *pre_process;
    strncpy(pre_process,command_set[0],strlen(command_set[0]));
    // printf("pre_process:%s\n",pre_process);
    // printf("command_set in commandexecute__1:%s\n",command_set[pip_num]);
    if(!strcmp(strtok(pre_process," "),"cd")){
        chdir(strtok(NULL," "));
        return;
    }
    else if(!strcmp(strtok(pre_process," "),"exit")){
        exit(EXIT_SUCCESS);
    }
    pid_t pid;
    pid=fork();
    if(pid<0){
        // Fork Error
        printf("Fork Error\n");
        exit(EXIT_FAILURE);
    }
    else if(pid!=0){
        // Parent Process
        char *background=strchr(command_set[pip_num],'&');
        if(background){
            printf("%d\n",pid);
            command_set[pip_num][background-command_set[pip_num]+1]='\0';
        }
        else waitpid(pid,NULL,0);
        return;
    }
    else{
        // printf("IN CHILD\n");
        // Child Process
        char **arg;
        if(pip_num){
            // printf("in fd\n");
            int fd[2];
            // while(pip_num--){
                // printf("pip_num:%d\n",pip_num);
                if(pipe(fd)<0){
                    printf("Create Pipe Error");
                    exit(EXIT_FAILURE);
                }
                pid=fork();
                if(pid<0){
                    printf("Fork Error\n");
                    exit(EXIT_FAILURE);
                }
                if(pid>0){
                    // Parent
                    close(fd[1]);
                    // close(STD_INPUT);
                    // dup(fd[0]);
                    dup2(fd[0],STD_INPUT);
                    close(fd[0]);
                    // break;
                    arg=argAnalysis(command_set[1]);
                    if(execvp(arg[0],arg)<0){
                        printf("bash: %s:command not found\n",arg[0]);
                    }
                    // myExecvp(command_set[1]);
                }
                else if(pid==0){
                    // Child
                    close(fd[0]);
                    // close(STD_OUTPUT);
                    // dup(fd[1]);
                    dup2(fd[1],STD_OUTPUT);
                    close(fd[1]);
                    arg=argAnalysis(command_set[0]);
                    if(execvp(arg[0],arg)<0){
                        printf("bash: %s:command not found\n",arg[0]);
                    }
                    // myExecvp(command_set[0]);
                }
            // }
        }
        else{
            // printf("in\n");
            // printf("command in exe%s\n",command_set[pip_num]);
            // arg=argAnalysis(command_set[pip_num]);
            // if(execvp(arg[0],arg)<0){
            //     printf("bash: %s:command not found\n",arg[0]);
            // }
            myExecvp(command_set[pip_num]);
        }
        // FILE *in=stdin; FILE *out=stdout;
        // dup2(fileno(in),STD_INPUT);
        // dup2(fileno(out),STD_OUTPUT);
        // printf("command_set in commandexecute__2:%s\n",command_set[pip_num]);
        // char **arg=argAnalysis(command_set[pip_num]);
        // int i;
        // for(i=0;arg[i];i++) printf("arg:%s\n",arg[i]);
        // if(execvp(arg[0],arg)<0){
        //     printf("bash: %s:command not found\n",arg[0]);
        // }
        exit(0);
        // int i;
        // for(i=0;arg[i];i++) free(arg[i]);
        // free(arg[i]); free(arg);
    }
}
#endif