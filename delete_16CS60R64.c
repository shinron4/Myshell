#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#define MAXLEN 10000

int lineDelete(char name[], int ln, int length, int inverse){       //function takes filename, starting line, number of lines and inverse as input
 	int  i, count = 0, c = '\0';				    //invese = 0 remove the lines, inverse = 1 delete every other lines except the given lines
	char line[MAXLEN];
	FILE *fptr = name ? fopen(name, "r"): stdin, *tempfile = fptr == stdin ? stdout : fopen("delfile", "w");
	if(fptr == 0 || tempfile == 0){ 
		perror("");
		return -1;
	}
	for(;count < ln - 1 && c != EOF; count++){                 //for copying or deleting of lines before the starting line according to the value of inverse
		line[i = 0] = '\0';
		while(i < MAXLEN -1 && (c = fgetc(fptr)) != '\n' && c != EOF){  //read the line
			line[i] = c;
			i++;
		}
		line[i++] = '\0';
		if(!inverse)
			if(i == MAXLEN || c == EOF)
				fprintf(tempfile, "%s", line);
			else
				fprintf(tempfile, "%s\n", line);
	}
	for(;count < ln + length - 1 && c != EOF; count++){  //for copying or deleting lines starting from ln to ln + length according to value for inverse
		line[i = 0] = '\0';
		while(i < MAXLEN -1 && (c = fgetc(fptr)) != '\n' && c != EOF){	
			line[i] = c;
			i++;
		}
		line[i++] = '\0';
		if(inverse)
			if(i == MAXLEN || c == EOF)
				fprintf(tempfile, "%s", line);
			else
				fprintf(tempfile, "%s\n", line);
	}
	if(c == '\n')
		c = fgetc(fptr);
	while(c != EOF){  //for copying or deleting the lines after the line ln + length according to the value of inverse
		if(!inverse)
			fputc(c, tempfile);
		c = fgetc(fptr);
	}
	if(tempfile != stdout){
		fclose(fptr);
		fclose(tempfile);
		unlink(name);
		link("delfile", name);
		unlink("delfile");
	}
	return 0;
}
