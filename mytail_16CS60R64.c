#include<stdio.h>
#include<stdlib.h>

int main(int argc, char *argv[]){
	int n = 10, cnt = 0;
	long int li;
	char c = '\0';
	FILE *fptr, *fptr2;
	if(argc > 2){
		n = atoi(argv[1] + 1);
		fptr = fopen(argv[2], "r");
	}
	else{
		fptr = fopen("readInput", "w");
		while((c = getchar()) != EOF)
			fputc(c, fptr);
		fclose(fptr);
		fptr = fopen("readInput", "r");
	}
	if(argc == 2)
		n = atoi(argv[1] + 1);
 	fptr2 = fopen("results", "w+");
	fseek(fptr, -1l, SEEK_END);
	if(fptr == 0)
		return -1;
	while(cnt < n + 1 && (li = ftell(fptr)) != 0){
		if((c = fgetc(fptr)) == '\n')
			cnt++;
		fputc(c, fptr2);
		fseek(fptr, -2l, SEEK_CUR);
	}
	if(li == 0)
		fputc(fgetc(fptr), fptr2);
	fclose(fptr);
	if(argc <= 2)
		unlink("readInput");
	fseek(fptr2, -1l, SEEK_CUR);
	do{
		printf("%c",fgetc(fptr2));
		fseek(fptr2, -2l, SEEK_CUR);
	}while(ftell(fptr2) != 0);
	printf("%c",fgetc(fptr2));
	fclose(fptr2);
	unlink("results");
	return 0;
}
