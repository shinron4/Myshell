#include<stdlib.h>
#include<stdio.h>
#include<string.h>

char *strrev(char str[]){                                   //reverses a string
	int len = strlen(str), i = 0, temp;
	while(i < len/2){
		temp = str[len - 1 - i];
		str[len - 1 - i] = str[i];
		str[i] = temp;
		i++;
	}
	return str;
}

FILE *putptr;                                              //File pointer to put the commands into hist.log

char *getCommand(char command[], char opt){                //gets the commnad from the file pointed by putptr
	int i = 0,c;
	while(opt == 66 && !feof(putptr)){ //for down button
		i = 0;
		if(ftell(putptr) == 0)
			while(c = fgetc(putptr) != '\n');
		while((c = fgetc(putptr)) != '\n' && c != EOF)
			command[i++] = c;
		command[i] = '\0';
		return command;
	}
	while(opt == 65 && ftell(putptr) > 0){ //for up button
		i = 0;
		fseek(putptr, -2l, SEEK_CUR);
		while(ftell(putptr) > 0 && (c = fgetc(putptr)) != '\n'){
			command[i++] = c;
			fseek(putptr, -2l, SEEK_CUR);
		}
		if(ftell(putptr) == 0){
			command[i++] = fgetc(putptr);
			fseek(putptr, 0l, SEEK_SET);
		}
		command[i] = '\0';
		strrev(command);
		return command;
	}
}

void putCommand(char command[]){                          //Puts to command in the file pointed by putptr
	fseek(putptr, 0l, SEEK_END);
	fprintf(putptr, "%s\n", command);
	fseek(putptr, -1l, SEEK_END);
}
