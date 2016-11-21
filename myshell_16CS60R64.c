#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<pwd.h>
#include<string.h>
#include<fcntl.h>

int main(){
	extern char command[1024];
	extern FILE *putptr;
	char buf[1024], part[200], *args[10], *cwd, *ptr = 0, *processCommand(char *), c, path[400];
	int i, pid, status, eatinput();
	putptr = fopen("hist.log", "w+");
	void putCommand(char[]);

	if((cwd = getcwd(buf, 1024)) == 0){
		perror("Error");
		return -1;
	}
	strcpy(path, getenv("PATH")); //changing the PATH variable for the myshell process
	strcat(path,":");
	strcat(path, buf);
	setenv("PATH", path, 1);
	while(1){ //Loop runs untill exit is typed
		if(ptr == 0){ //If previous commnad is read entirely then read next command
			printf("16CS60R64|%s>", cwd);
			ptr = command;
			while((c = eatinput()) != '\n')
				*ptr++ = c;
			*ptr = '\0';
			putCommand(command);
		}
		ptr = processCommand(part);

		if(strcmp(part, "mypwd") == 0){ //Command is mypwd
			cwd = getcwd(buf, 1024);
			if(ptr != 0 && *ptr == '|'){
				args[0] = (char *)malloc(9*sizeof(char));
				strcpy(args[0], "mypipe");
				args[1] = (char *)malloc(strlen(command)*sizeof(char));
				strcpy(args[1], command);
				args[2] = 0;
				if((pid = fork()) == 0)
					execvp("mypipe", args);
				while(ptr = processCommand(part));
			}
			else
				printf("%s\n", cwd);
		}

		else if(strcmp(part, "mycd") == 0){//Command is mycd
			if(ptr == 0)
				continue;
			else if(*ptr != '|'){
				ptr = processCommand(part);
			}
			if (*ptr == '|'){                                            //If next symbol is pipe symbol then call the mypipe process and
				args[0] = (char *)malloc(9*sizeof(char));            //pass the entire command to mypipe process for pipe handling
				strcpy(args[0], "mypipe");
				ptr = processCommand(part);
				args[1] = (char *)malloc(strlen(command)*sizeof(char));
				strcpy(args[1], command);
				args[2] = 0;
				if((pid = fork()) == 0)
					execvp("mypipe", args);
				while(ptr = processCommand(part));
			}
			else{
				if(chdir(part) == 0){
					if((cwd = getcwd(buf, 1024)) == 0)
						perror("Error");
				}
				else
					perror("");
			}
		}

		else if(strcmp(part, "myrm") == 0){  //Command is myrm
			if(ptr == 0){
				printf("myrm: Missing options\n");
				continue;
			}
			args[0] = (char *)malloc(strlen(part)*sizeof(char));
			strcpy(args[0], "myrm");
			ptr = processCommand(part);
			args[1] = (char *)malloc(strlen(part)*sizeof(char));
			strcpy(args[1], part);
			if(ptr == 0){
				printf("myrm: Missing file operand\n");
				continue;
			}
			ptr = processCommand(part);
			args[2] = (char *)malloc(strlen(part)*sizeof(char));
			strcpy(args[2], part);
			args[3] = 0;
			if(ptr != 0 && *ptr == '|'){      //Pipe handling
				args[0] = (char *)malloc(9*sizeof(char));
				strcpy(args[0], "mypipe");
				args[1] = (char *)malloc(strlen(command)*sizeof(char));
				strcpy(args[1], command);
				args[2] = 0;
				if((pid = fork()) == 0)
					execvp("mypipe", args);
				while(ptr = processCommand(part));
			}
			else if((pid = fork()) == 0){
				if(execvp("myrm", args) < 0){
					perror("");
					exit(1);
				}
			}
		}

		else if(strcmp(part, "mymv") == 0){     //command is mymv
			if(ptr == 0){
				printf("mymv: Missing file operand\n");
				continue;
			}
			args[0] = (char *)malloc(strlen(part)*sizeof(char));
			strcpy(args[0], "mymv");
			ptr = processCommand(part);
			args[1] = (char *)malloc(strlen(part)*sizeof(char));
			strcpy(args[1], part);
			if(ptr == 0){
				printf("mymv: Missing file operand\n");
				continue;
			}
			ptr = processCommand(part);
			args[2] = (char *)malloc(strlen(part)*sizeof(char));
			strcpy(args[2], part);
			args[3] = 0;
			if(ptr != 0 && *ptr == '|'){    //Pipe handling
				args[0] = (char *)malloc(9*sizeof(char));
				strcpy(args[0], "mypipe");
				args[1] = (char *)malloc(strlen(command)*sizeof(char));
				strcpy(args[1], command);
				args[2] = 0;
				if((pid = fork()) == 0)
					execvp("mypipe", args);
				while(ptr = processCommand(part));
			}
			else if((pid = fork()) == 0){
				if(execvp("mymv", args) < 0){
					perror("");
					exit(1);
				}
			}
		}

		else if(strcmp(part, "myls") == 0){     //Command is myls
			args[0] = (char *)malloc(strlen(part)*sizeof(char));
			strcpy(args[0], "myls");
			args[1] = 0;
			if(ptr != 0 && *ptr == '|'){
				args[0] = (char *)malloc(9*sizeof(char));
				strcpy(args[0], "mypipe");
				args[1] = (char *)malloc(strlen(command)*sizeof(char));
				strcpy(args[1], command);
				args[2] = 0;
				if((pid = fork()) == 0)
					execvp("mypipe", args);
				while(ptr = processCommand(part));
			}
			else if((pid = fork()) == 0){
				if(execvp("myls", args) < 0){
					perror("");
					exit(1);
				}
			}
		}

		else if(strcmp(part, "mymkdir") == 0){   //Command is mymkdir
			int i = 1;
			args[0] = (char *)malloc(strlen(part)*sizeof(char));
			strcpy(args[0], "mymkdir");
			while(ptr && *ptr != '|' && *ptr != '&'  && i <= 8){
				ptr = processCommand(part);
				args[i] = (char *)malloc(strlen(part)*sizeof(char));
				strcpy(args[i], part);
				i++;
			}
			if(i == 1){
				printf("mymkdir: Missing file operand\n");
				continue;
			}
			args[i] = 0;
			if(ptr != 0 && *ptr == '|'){      //Pipe handling
				args[0] = (char *)malloc(9*sizeof(char));
				strcpy(args[0], "mypipe");
				args[1] = (char *)malloc(strlen(command)*sizeof(char));
				strcpy(args[1], command);
				args[2] = 0;
				if((pid = fork()) == 0)
					execvp("mypipe", args);
				while(ptr = processCommand(part));
			}
			else if((pid = fork()) == 0){
				if(execvp("mymkdir", args) < 0){
					perror("");
					exit(1);
				}
			}

		}

		else if(strcmp(part, "myps") == 0){       //Command is myps
			args[0] = (char *)malloc(strlen(part)*sizeof(char));
			strcpy(args[0], "myps");
			if(ptr != 0 && *ptr == '|'){      //Pipe handling
				args[0] = (char *)malloc(9*sizeof(char));
				strcpy(args[0], "mypipe");
				args[1] = (char *)malloc(strlen(command)*sizeof(char));
				strcpy(args[1], command);
				args[2] = 0;
				if((pid = fork()) == 0)
					execvp("mypipe", args);
				while(ptr = processCommand(part));
			}
			else if(ptr != 0 && *ptr != '&'){
				args[1] = (char *)malloc(3*sizeof(char));
				ptr = processCommand(part);
				strcpy(args[1], part);
				args[2] = 0;
			}
			else{
				args[1] = 0;
				if((pid = fork()) == 0){
					if(execvp("myps", args) < 0){
						perror("");
						exit(1);
					}
				}
			}
		}

		else if(strcmp(part, "mytail") == 0){    //Command is mytail
			int i = 1;
			args[0] = (char *)malloc(strlen(part)*sizeof(char));
			strcpy(args[0], "mytail");
			while(ptr && *ptr != '|' && *ptr != '&'){
				ptr = processCommand(part);
				args[i] = (char *)malloc(strlen(part)*sizeof(char));
				strcpy(args[i], part);
				i++;
			}
			args[i] = 0;
			if(ptr != 0 && *ptr == '|'){    //Pipe handling
				args[0] = (char *)malloc(9*sizeof(char));
				strcpy(args[0], "mypipe");
				args[1] = (char *)malloc(strlen(command)*sizeof(char));
				strcpy(args[1], command);
				args[2] = 0;
				if((pid = fork()) == 0)
					execvp("mypipe", args);
				while(ptr = processCommand(part));
			}
			else if((pid = fork()) == 0){
				if(execvp("mytail", args) < 0){
					perror("");
					exit(1);
				}
			}
		}
		
		else if(strcmp(part, "mygrep") == 0){    //Command is mygrep
			int i = 1;
			if(ptr == 0){
				printf("mygrep: Missing option\n");
				continue;
			}
			args[0] = (char *)malloc(strlen(part)*sizeof(char));
			strcpy(args[0], "mygrep");
			while(ptr && *ptr != '|' && *ptr != '&'){
				ptr = processCommand(part);
				args[i] = (char *)malloc(strlen(part)*sizeof(char));
				strcpy(args[i], part);
				i++;
			}
			args[i] = 0;
			if(ptr != 0 && *ptr == '|'){     //Pipe handling
				args[0] = (char *)malloc(9*sizeof(char));
				strcpy(args[0], "mypipe");
				args[1] = (char *)malloc(strlen(command)*sizeof(char));
				strcpy(args[1], command);
				args[2] = 0;
				if((pid = fork()) == 0)
					execvp("mypipe", args);
				while(ptr = processCommand(part));
			}
			else if((pid = fork()) == 0){
				if(execvp("mygrep", args) < 0){
					perror("");
					exit(1);
				}
			}
		}

		else if(strcmp(part, "mysed") == 0){    //Command is mysed
			int i = 1;
			args[0] = (char *)malloc(strlen(part)*sizeof(char));
			strcpy(args[0], "mysed");
			while(ptr && *ptr != '|' && *ptr != '&'){
				ptr = processCommand(part);
				args[i] = (char *)malloc(strlen(part)*sizeof(char));
				strcpy(args[i], part);
				i++;
			}
			args[i] = 0;
			if(ptr != 0 && *ptr == '|'){     //Pipe handling
				args[0] = (char *)malloc(9*sizeof(char));
				strcpy(args[0], "mypipe");
				args[1] = (char *)malloc(strlen(command)*sizeof(char));
				strcpy(args[1], command);
				args[2] = 0;
				if((pid = fork()) == 0)
					execvp("mypipe", args);
				while(ptr = processCommand(part));
			}
			else if((pid = fork()) == 0){
				if(execvp("mysed", args) < 0){
					perror("");
					exit(1);
				}
			}
		}

		else if(strcmp(part, "exit") == 0){
			fclose(putptr);
			unlink("hist.log");
			break;
		}

		else{    //This is for bash commands or the commands which have no matched ealier
			int i = 1;
			args[0] = (char *)malloc(strlen(part)*sizeof(char));
			strcpy(args[0], part);
			while(ptr && *ptr != '|' && *ptr != '&'){
				ptr = processCommand(part);
				args[i] = (char *)malloc(strlen(part)*sizeof(char));
				strcpy(args[i], part);
				i++;
			}
			args[i] = 0;
			if(ptr != 0 && *ptr == '|'){
				args[0] = (char *)malloc(9*sizeof(char));
				strcpy(args[0], "mypipe");
				args[1] = (char *)malloc(strlen(command)*sizeof(char));
				strcpy(args[1], command);
				args[2] = 0;
				if((pid = fork()) == 0){
					execvp("mypipe", args);
				}
				while(ptr = processCommand(part));
			}
			else if((pid = fork()) == 0){
				if(execvp(args[0], args) < 0){
					perror("");
					exit(1);
				}
			}
			
		}
		if(ptr == 0 || *ptr != '&'){
			waitpid(pid, &status, 0);
			continue;
		}
		ptr = processCommand(part);
	}
	return 0;
}

char *ptr = 0, command[1024];
char *processCommand(char *part){
	char *temp = part;
	if(ptr == 0)
		ptr = command;
	while(*ptr != ' ' && *ptr != '	'&& *ptr != '\0')
		*temp++ = *ptr++;
	*temp = '\0';
	if(*ptr == '\0')
		ptr = 0;
	else
		ptr++;
	return ptr;
}
