#include<stdio.h>
#include<stdlib.h>
#include<string.h>

char* patternMatching(char line[], char pattern[]){                                   //Pattern mathcing function where pattern is regular expression
	char newPattern[200], *ptr1 = pattern, *ptr2 = line, *ptr = line;
	int i = 0;
	while(*ptr1 != '\0'){                                                         //Replaces a+ by aa*
		newPattern[i++] = *ptr1;
		if(*(ptr1 + 1) == '+'){
			newPattern[i++] = *ptr1;
			newPattern[i++] = '*';
			ptr1++;
		}
		ptr1++;
	}
	newPattern[i] = '\0';
	ptr1 = newPattern;
	while(*ptr1 != '\0'){                                                         //Replaces c*cc.. by ccc* and c?cc by ccc?
		if((*ptr1 == '?' || *ptr1 == '*') && (*(ptr1 - 1) == *(ptr1 + 1))){
			int temp = *ptr1;
			*ptr1 = *(ptr1 + 1);
			*(ptr1 + 1) = temp;
		}
		ptr1++;
	}
	ptr1 = newPattern;
	i = 0;
	while(*ptr1 != '\0' && *ptr2 != '\0'){                                       //Regular expression matching loop
		if((*(ptr1 + 1) == '?' || *(ptr1 + 1) == '*') && *ptr1 != *ptr2)
			ptr1 = ptr1 + 2;
		else if((*(ptr1 + 1) != '?' && *(ptr1 + 1) != '*') && *ptr1 != *ptr2){
			i++;
			ptr = ptr2 = line + i;
			ptr1 = newPattern;
		}
		else{
			if(*(ptr1 + 1) == '?')
				ptr1 = ptr1 + 2;
			else if(*(ptr1 + 1) != '*')
				ptr1 = ptr1 + 1;
			ptr2 = ptr2 + 1;
		}
	}
	if(*ptr1 == '\0' || strcmp(ptr1 + 1, "?") == 0 || strcmp(ptr1 + 1, "*") == 0)
		return ptr;                                                          //Return the starting place from where a match is found in the string else 0
	return 0;
}

int main(int argc, char *argv[]){
	FILE *fptr;
	char line[10000], c, *ptr;
	int i, j = 0, NUM;
	if(strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "-m") == 0){
		switch(argv[1][1]){
			case 'v': if(argv[3] == 0) // Command line argument for inverse of matching
					fptr = stdin; //Reading from stdin if no input file is given
				  else if((fptr = fopen(argv[3], "r")) == 0){
					printf("Error: file operand nor found\n");
					exit(1);
				  }
				  while(!feof(fptr)){
					i = 0;
					while((c = fgetc(fptr)) != '\n')
						line[i++] = c;
					line[i] = '\0';
		                        if ((ptr = patternMatching(line ,argv[2])) == 0)
						printf("%s\n", line);
				  }
				  break;
			case 'm': NUM = atoi(argv[2]);  //Command line argument for matching till NUM number of lines from the file is read.
				  if(argv[4] == 0)
					fptr = stdin; //Reading from stdin if no input file is given
				  else if((fptr = fopen(argv[4], "r")) == 0){
					printf("Error: file operand nor found\n");
					exit(1);
				  }				
				  while(!feof(fptr) && j < NUM){
					i = 0;
					while((c = fgetc(fptr)) != '\n')
						line[i++] = c;
					line[i] = '\0';
					if ((ptr = patternMatching(line ,argv[3])) != 0){
						printf("%s\n", line);
						j++;
					}
				  }
				  break;
			default:;
		}
		fclose(fptr);
		exit(0);
	}
	if(argv[2] == 0)
		fptr = stdin; //Reading from stdin if no input file is given
	else if((fptr = fopen(argv[2], "r")) == 0){
		printf("Error: file operand nor found\n");
		exit(1);
	}
	while(!feof(fptr)){
		i = 0;
		while((c = fgetc(fptr)) != '\n' && c != EOF)
			line[i++] = c;
		line[i] = '\0';
		if ((ptr = patternMatching(line ,argv[1])) != 0)
			printf("%s\n", line);
	}
	fclose(fptr);
	return 0;
}
