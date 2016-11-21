#include<stdio.h>
#include<sys/stat.h>

int main(int argc, char *argv[]){;
	while(*++argv != 0)
		if(mkdir(*argv, 0777) == -1)
			return -1;
		
	return 0;
}
