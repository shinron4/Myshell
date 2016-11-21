#include<stdio.h>
#include<unistd.h>

int main(){
	char buf[1024];
	printf("%s\n", getcwd(buf,1024));
	return 0;
}
