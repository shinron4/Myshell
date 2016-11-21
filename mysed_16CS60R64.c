#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int handleReplace(char option[], char *file){ //handle the replacements of 'pattern' by 'reptext' in '*file'
	char *ptr = option, part[100] = "", pattern[100] = "", reptext[100] = "", optionalpattern[100] = "";
	int i, inst, global, matchfirst = global = inst = 0, replace(char[], char[] ,char[], char[], int, int, int);
	part[i = 0] = '\0';
	while(*ptr != '\0' && *ptr != '/')
		part[i++] = *ptr++;
	part[i] = '\0';
	if(*ptr == '\0')
		return -1;
	if(strcmp(part, "s") != 0){
		matchfirst = 1;
		strcpy(optionalpattern, part);
		ptr++;
		part[i = 0] = '\0';
		while(*ptr != '\0' && *ptr != '/')
			part[i++] = *ptr++;
		part[i] = '\0';
		if(*ptr == '\0')
			return -1;
	}	
	ptr++;
	part[i = 0] = '\0';
	while(*ptr != '\0' && *ptr != '/')
		part[i++] = *ptr++;
	part[i] = '\0';
	if(*ptr == '\0')
		return -1;
	strcpy(pattern, part);
	ptr++;
	part[i = 0] = '\0';
	while(*ptr != '\0' && *ptr != '/')
		part[i++] = *ptr++;
	part[i] = '\0';
	if(*ptr == '\0')
		return -1;
	strcpy(reptext, part);
	ptr++;
	part[i = 0] = '\0';
	while(*ptr != '\0' && *ptr != '/')
		part[i++] = *ptr++;
	part[i] = '\0';
	if(strcmp(part, "g") == 0)
		global = 1;
	else
		if((inst = atoi(part)) == -1)
			return -1;
	return replace(file, optionalpattern, pattern, reptext, global, inst, matchfirst);
}
	
int handleDelete(char option[], char *file){  //handle the deletion of lines	
	char *ptr, part[10];
	int i = 0, start, start2, inverse, lineDelete(char [], int, int, int);
	if(inverse = option[0] == '\\' ? 1 : 0)  //backslash given in starting then inverse = 1
		ptr = option + 2;
	else
		ptr = option + 1;
	while(*ptr != ']' && *ptr != '.' && *ptr != ',')
		part[i++] = *ptr++;
	part[i] = '\0';
	if(*ptr == ']'){
		start = atoi(part);
		lineDelete(file, start, 1, inverse);
	}
	else if(*ptr == ','){
		start = atoi(part);
		ptr++, i = 0;
		while(*ptr != ']')
			part[i++] = *ptr++;
		start2 = atoi(part);
		if(inverse){
			lineDelete(file, start, start2 - start + 1, inverse);
			lineDelete(file, 2, start2 - start -1, 0);
		}
		else{
			lineDelete(file, start, 1, inverse);
			lineDelete(file, start2 - 1, 1, inverse);
		}
	}
	else if(*ptr == '.'){
		start = atoi(part);
		ptr += 3, i = 0;
		while(*ptr != ']')
			part[i++] = *ptr++;
		lineDelete(file, start, atoi(part) - start + 1, inverse);
	}
	return 0;
}

int main(int argc, char *argv[]){
	int len;
	FILE *fptr;
	if(argc < 2){ //options are not given
		printf("Error: Missing options\n");
		exit(1);
	}
	len = strlen(argv[1]);
	switch(argv[1][len -1]){
		case 'd': handleDelete(argv[1], argv[2]);  //Handle delete
			  break;
		case 'g': case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':
			  handleReplace(argv[1], argv[2]);   //Handle repalce
			  break;
		default:  printf("Error: Missing option\n");
			  exit(1);
			  break;
	}
	return 0;
}
