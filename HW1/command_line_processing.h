#ifndef COM_PRO_C
#define COM_PRO_C
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include <fcntl.h>
#include<sys/stat.h>
#include<unistd.h>

#define read_buf_size 64
#define coms_buf_size 2
#define arg_buf_size 2
// #define sep_buf_size 5
#define coms_ana_delim "|"
#define args_ana_delim " <>&"
#define STD_INPUT 0
#define STD_OUTPUT 1
#define read_flag O_RDONLY
#define write_flag O_RDWR | O_CREAT | O_TRUNC
// #define args_ana_delim " "

static char *readCommand(){
	int buf_size=read_buf_size,com_len=0;
	char *command=realloc(NULL,buf_size),com_ch;
	
	if(!command){
		printf("command realloc error!\n");
		//exit(EXIT_FAILURE);
		return NULL;
	}
	
	// if EOF or endl,return command 
	while((com_ch=fgetc(stdin))!=EOF){
		if(com_ch=='\n' || com_ch=='\r'){
			command[com_len++]='\0';
			return command;
		}
		command[com_len++]=com_ch;
		
		//if command size>buf_size,realloc command
		if(com_len==buf_size){
			command=realloc(command,buf_size+=read_buf_size);
			if(!command){
				printf("command realloc ERROR!\n");
				return "\0";
			}
		}
	}
	printf("outofwhile\n");
	return "\0";
}

static char **commandPipSlice(const char *input,int *pip_num){
	int c_buf_size=coms_buf_size,coms_len=0;
	char **commands=malloc(c_buf_size*sizeof(char*));
	char *part_com,*copy=malloc((strlen(input)+1)*sizeof(char));
	strncpy(copy,input,strlen(input)+1);
	if(!commands){
		printf("commands malloc error!\n");
		exit(EXIT_FAILURE);
	}
	(*pip_num)=-1;
	part_com=strtok(copy,coms_ana_delim);
	if(!part_com) return NULL;
	while(part_com){
		// printf("pip_in_func:%d\n",(*pip_num));
		(*pip_num)=(*pip_num)+1;
		commands[coms_len++]=part_com;
		// printf("command[com_len]:%s\n",commands[coms_len-1]);
		if(coms_len==c_buf_size){
			commands=realloc(commands,c_buf_size+=sizeof(char*));
			if(!commands){
				printf("commands realloc error!\n");
				exit(EXIT_FAILURE);
			}
		}
		part_com=strtok(NULL,coms_ana_delim);
	}
	commands[coms_len]='\0';
	//free(copy);
	return commands;
}

static void redirect(const char *infile,const char *outfile){
	int in,out;
	if(!infile && !outfile) return;
	// printf("redirect continue\n");
	if(infile){
		// printf("infile\n");
		in=open(infile,read_flag,0666);
		dup2(in,STD_INPUT);
		close(in);
	}
	if(outfile){
		out=open(outfile,write_flag,0666);
		dup2(out,STD_OUTPUT);
		close(out);
	}
	return;
}

// static char **argAnalysis(char *command){
// 	printf("command in arganalysis:%s\n",command);
// 	int arg_size=arg_buf_size,arg_len=0;
// 	char **arg=malloc(arg_size*sizeof(char*));
// 	char *part_arg;
// 	if(!arg){
// 		printf("arg malloc Error!\n");
// 		exit(EXIT_FAILURE);
// 	}
// 	part_arg=strtok(command,args_ana_delim);
// 	while(part_arg){
// 		// printf("part_arg:%s\n",part_arg);
// 		arg[arg_len++]=part_arg;
// 		if(arg_len==arg_size){
// 			arg=(char **)realloc(arg,arg_size+=sizeof(char*));
// 			if(!arg){
// 				printf("arg realloc Error!\n");
// 				exit(EXIT_FAILURE);
// 			}
// 		}
// 		part_arg=strtok(NULL,args_ana_delim);
// 		// printf("part_arg_after:%s\n",part_arg);
// 	}
// 	arg[arg_len]=NULL;
// 	return arg;
// }

static char **argAnalysis(char *command){
	// printf("command in arganalysis:%s\n",command);
	int arg_size=arg_buf_size,arg_len=0;
	size_t i=0;
	char **arg=malloc(arg_size*sizeof(char*));
	char *main_arg=malloc(strlen(command)*sizeof(char)),*part_arg;
	if(!arg){
		printf("arg malloc Error!\n");
		exit(EXIT_FAILURE);
	}
	// printf("before strtok\n");
	// printf("command_size:%d",strlen(command));
	while(i<strlen(command) && command[i]!='<' && command[i]!='>' && command[i]!='&'){
		i++;
	}
	strncpy(main_arg,command,i);
	// main_arg=strtok(command,args_ana_delim);
	// printf("main_arg:%s",main_arg);
	part_arg=strtok(main_arg," ");
	while(part_arg){
		// printf("part_arg:%s\n",part_arg);
		arg[arg_len++]=part_arg;
		if(arg_len==arg_size){
			arg=(char **)realloc(arg,arg_size+=sizeof(char*));
			if(!arg){
				printf("arg realloc Error!\n");
				exit(EXIT_FAILURE);
			}
		}
		part_arg=strtok(NULL," ");
		// printf("part_arg_after:%s\n",part_arg);
	}
	arg[arg_len]='\0';
	return arg;
}

// static void myExecvp(char *command){
// 	printf("in exe\n");
// 	printf("command:%s",command);
// 	char **arg_set;
// 	char *part_arg,*colon_process;
// 	strncpy(colon_process,command,strlen(command));
// 	int i;
// 	// func-->1 : '<'
// 	//     -->2 : '>'
// 	//     -->3 : '<' and '>'
// 	//     -->0 : none
// 	printf("colon_process:%s\n",colon_process);
// 	part_arg=strtok(colon_process,";");
// 	while(part_arg){
// 		printf("part_arg:%s\n",part_arg);
// 		char *infile=calloc(strlen(part_arg),sizeof(char)),
// 			 *outfile=calloc(strlen(part_arg),sizeof(char));
// 		infile[0]='\0'; outfile[0]='\0';
// 		for(i=0;i<strlen(part_arg);i++){
// 			int file_len=0;
// 			if(part_arg[i]=='<'){
// 				int j;
// 				for(j=i+2;part_arg[j]!=' ';j++){
// 					infile[file_len++]=part_arg[j];
// 				}
// 				infile[file_len]='\0';
// 				i=j;
// 			}
// 			else if(part_arg[i]=='>'){
// 				int j;
// 				for(j=i+2;part_arg[j]!=' ';j++){
// 					outfile[file_len++]=part_arg[j];
// 				}
// 				outfile[file_len]='\0';
// 				i=j;
// 			}
// 		}
// 		redirect(infile,outfile);
// 		arg_set=argAnalysis(part_arg);
// 		if(execvp(arg_set[0],arg_set)<0){
// 			printf("bash: %s:command not found\n",arg_set[0]);
// 		}
// 		part_arg=strtok(NULL,";");
// 	}
// }

// static char **commandAnalysis(const char *input,int* sep){
// 	int c_buf_size=coms_buf_size,s_buf_size=sep_buf_size,coms_len=0,sep_len=0,in_count=0;
// 	char **commands=malloc(c_buf_size*sizeof(char*));
// 	char *part_com;
// 	char *copy=calloc(strlen(input)+1,sizeof(char));
// 	strncpy(copy,input,strlen(input));
// 	if(!commands){
// 		printf("commands malloc error!\n");
// 		return NULL;
// 	}
// 	// printf("%s",copy);
// 	part_com=strtok(copy," ");
// 	while(part_com){
// 		// printf("part_com:%s",part_com);
// 		if(!(strcmp(part_com,"&") && strcmp(part_com,"|") && strcmp(part_com,"<") && strcmp(part_com,">"))){
// 			sep[sep_len++]=in_count;
// 			if(sep_len==s_buf_size){
// 				sep=realloc(sep,s_buf_size+=sizeof(int));
// 				if(!sep){
// 					printf("sep realloc error!\n");
// 					exit(EXIT_FAILURE);
// 				}
// 			}
// 			printf("sep:%d\n",sep[sep_len-1]);
// 		}
// 		commands[coms_len++]=part_com;
// 		if(coms_len==c_buf_size){
// 			commands=realloc(commands,c_buf_size+=coms_buf_size);
// 			if(!commands){
// 				printf("command realloc error!\n");
// 				exit(EXIT_FAILURE);
// 			}
// 		}
// 		in_count+=1;
// 		part_com=strtok(NULL," ");
// 	}
// 	sep[sep_len]=-1;
// 	commands[coms_len]=NULL;
// 	return commands;
// }
#endif
