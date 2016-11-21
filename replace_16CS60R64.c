#include<stdio.h>
#include<string.h>
#define MAXLEN 10000

int subrep(char source[], int start, int len, char rep[]){ //replaces the substring starting from 'start' upto 'start + len -1' by string 'rep'
	int end = start + len -1, srclen = strlen(source);
	char temp[MAXLEN];
	if(start > end || (srclen + strlen(rep) - len) > MAXLEN)
		return -1;   //return -1 if final string after replacement will be greater than MAXLEN
	else if(start < 0)
		start = 0;
	else if(end > srclen -1)
		end = srclen -1;
	strcpy(temp, &source[end + 1]);
	strcpy(&source[start], rep);
	strcat(source, temp);
	return 0; //return 0 if successful completion
}

char *changePattern(char pattern[]){  //for changing a+ by aa* and a*aa or c?cc by aaa* and ccc? respectively.
	char newPattern[200], *ptr1 = pattern;
	int i = 0;
	while(*ptr1 != '\0'){
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
	while(*ptr1 != '\0'){
		if((*ptr1 == '?' || *ptr1 == '*') && (*(ptr1 - 1) == *(ptr1 + 1))){
			int temp = *ptr1;
			*ptr1 = *(ptr1 + 1);
			*(ptr1 + 1) = temp;
		}
		ptr1++;
	}
	
	return strcpy(pattern, newPattern);
}

char* patternMatching(int *len, char line[], char pattern[]){ //matches the pattern 'pattern' in the line 'line'
	char *ptr1 = changePattern(pattern), *ptr2, *ptr = ptr2 = line;
	int i = 0;					      
	while(*ptr1 != '\0' && *ptr2 != '\0'){
		if((*(ptr1 + 1) == '?' || *(ptr1 + 1) == '*') && *ptr1 != *ptr2)
			ptr1 = ptr1 + 2;
		else if((*(ptr1 + 1) != '?' && *(ptr1 + 1) != '*') && *ptr1 != *ptr2){
			i++;
			ptr = ptr2 = line + i;
			ptr1 = pattern;
		}
		else{
			if(*(ptr1 + 1) == '?')
				ptr1 = ptr1 + 2;
			else if(*(ptr1 + 1) != '*')
				ptr1 = ptr1 + 1;
			ptr2 = ptr2 + 1;
		}
	}
	if(*ptr1 == '\0' || strcmp(ptr1 + 1, "?") == 0 || strcmp(ptr1 + 1, "*") == 0){
		*len = ptr2 - ptr;  //'*len' stores the length of the matc found.
		return ptr;  //returns the pointer to the 'line' where the match is found.
	}
	*len = 0; //else returns NULL and 0 as the length of the matched pattern
	return 0;
}

//replaces the 'pattern' by 'reptext' in file 'name'
//global = 1 replace all occurances
//inst = 1 replace the 'inst'th occurance
//matchfirst = 1 replace 'pattern' by 'reptext' only in lines where pattern 'optionalpattern' is found

int replace(char name[], char optionalpattern[], char pattern[], char reptext[], int global, int inst, int matchfirst){  
	FILE *fptr = name ? fopen(name, "r") : stdin, *tempfile = fptr == stdin ? stdout: fopen("delfile", "w");
	char c = '\0', line[MAXLEN], *ptr;
	int i = 0, len = 0;
	if(fptr == 0 || tempfile == 0){
		perror("");
		return -1;
	}
	while(c != EOF && (global || inst)){
		i = 0, len = 0;
		while((c = fgetc(fptr)) != '\n' && c != EOF)
			line[i++] = c;
		line[i] = '\0';
		ptr = line;
		if(!matchfirst || patternMatching(&i ,line, optionalpattern))
			while(ptr = patternMatching(&len, ptr + len, pattern))
				if(global || --inst == 0){
					subrep(line, ptr - line, len, reptext);
					len = strlen(reptext);
				}
		if(c == EOF)
			fprintf(tempfile, "%s", line);
		else
			fprintf(tempfile, "%s\n", line);
	}
	while(c != EOF){
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
	return 0; //return 0 on successfull completion
}


/*int main(){
	int len = 0;
	char line[200] = "hello my name is stres\\.............///singh";
	char pattern[200] = "stres\\.+";
	
	char *ptr = patternMatching(&len, line, pattern);
	printf("%d : %s", len, ptr);
	return 0;
}*/
