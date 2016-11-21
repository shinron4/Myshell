#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

int main(int argc, char *argv[]){
	int fd[2], j = 0, pid;
	extern char command[1024];
	char *ptr = 0, part[200], c, *args[10], *processCommand(char *);
	strcpy(command, argv[1]);
	ptr = command;
	while(ptr != 0){  //Untill entire command is read.
		ptr = processCommand(part);
		int i = 1;
		args[0] = (char *)malloc(strlen(part)*sizeof(char));
		strcpy(args[0], part);
		while(ptr != 0 && *ptr != '|'){ //Untill a pipe symbol or end of command is reached. 
			ptr = processCommand(part);
			args[i] = (char *)malloc(strlen(part)*sizeof(char));
			strcpy(args[i], part);
			i++;
		}
		args[i] = 0;
		if(ptr != 0 && *ptr == '|'){ //Create a pipe for the child if pipe symbol is reached in the command.
			if(pipe(fd)){
				perror("");
				exit(1);
			}
			if((pid = fork()) == 0){
				dup2(fd[1], 1);
				if(execvp(args[0], args) < 0){
					perror("");
					exit(1);
				}
			}
			dup2(fd[0], 0);
			close(fd[1]);
			ptr = processCommand(part);
		}
		else{ //If the command read is the last in the pipeline.
			if((pid = fork()) == 0)
				if(execvp(args[0], args) < 0){
					perror("");
					exit(1);
				}
				wait(NULL);
		}
	}
	return 0;			
}	

char *ptr = 0, command[1024];
char *processCommand(char *part){ //Function stores the a part of entire command in part array on each subsequent call
	char *temp = part;        //Returns the next non-null char in the command else 0
	if(ptr == 0)
		ptr = command;
	while(*ptr != ' ' && *ptr != '\t' && *ptr != '\0')
		*temp++ = *ptr++;
	*temp = '\0';
	if(*ptr == '\0')
		ptr = 0;
	else
		ptr++;
	return ptr;
}
