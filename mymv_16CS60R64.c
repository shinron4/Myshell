#include<stdio.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<dirent.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>

int transfer(char s[], char d[]){
	struct directory{
		DIR *dir;
		char dir_name[100]; 
		struct directory *prev;
	} *root = (struct directory *)malloc(sizeof(struct directory)), *temp;
	struct dirent *indir;
	struct stat fileattr1;
	char *ptr1, *ptr2, source[1024], destination[1024];
	strcpy(source, s);
	strcpy(destination, d);
	ptr1 = destination;
	while(*ptr1 != 0)
		ptr1++;
	*ptr1++ = '/';
	*ptr1 = '\0';
	strcpy(root->dir_name, source);
	ptr2 = source;
	while(*ptr2 != 0)
		ptr2++;
	*ptr2++ = '/';
	*ptr2 = '\0';
	root->prev = 0;
	root->dir = opendir(root->dir_name);
	while(root != 0){ 
		while((indir = readdir(root->dir)) != 0){
			if(indir->d_type == DT_DIR){
				if(strcmp(indir->d_name, ".") == 0 || strcmp(indir->d_name, "..") == 0)
					continue;
				temp = (struct directory *)malloc(sizeof(struct directory));
				strcpy(ptr1, indir->d_name);
				strcpy(ptr2, indir->d_name);						
				if(stat(source, &fileattr1) == -1)
					return -1;
				else
					if(mkdir(destination, fileattr1.st_mode) == -1)
						return -1;
				strcpy(temp->dir_name, source);		
				temp->prev = root;				
				temp->dir = opendir(temp->dir_name);
				while(*ptr1 != 0)
					ptr1++;
				*ptr1++ = '/';
				*ptr1 = '\0';
				while(*ptr2 != 0)
					ptr2++;
				*ptr2++ = '/';
				*ptr2 = '\0';
				root = temp;						
				continue;
			}
			else
				strcpy(ptr1 ,indir->d_name);
				strcpy(ptr2 ,indir->d_name);	
				if(link(source, destination) == -1)
					return -1;
				else{
					unlink(source);
				}
		}
		closedir(root->dir);
		temp = root;
		root = root->prev;
		rmdir(temp->dir_name);
		ptr1 = ptr1 - 2;
		while(*ptr1 != '/')
			ptr1--;
		*++ptr1 = '\0';
		ptr2 = ptr2 - 2;
		while(*ptr2 != '/')
			ptr2--;
		*++ptr2 = '\0';
		free(temp);
	}
}


int main(int argc, char *argv[]){
	struct stat fileattr1, fileattr2;
	char source[1024], destination[1024], *ptr1, *ptr2, *ptr3;
	strcpy(source, argv[1]);
	ptr1 = source; ptr3 = ptr1;
	while(*ptr3 != 0){
		if(*ptr3 == '/')
			ptr1 = ptr3 + 1;
		ptr3++;
	}
	strcpy(destination,argv[2]);
	ptr2 = destination; ptr3 = ptr2;
	while(*ptr3 != 0){
		if(*ptr3 == '/')
			ptr2 = ptr3 + 1;
		ptr3++;
	}
	if(stat(destination, &fileattr2) == -1)//checked
		if(stat(source, &fileattr1) != -1)
			if(S_ISDIR(fileattr1.st_mode))
				if(mkdir(destination, fileattr1.st_mode) != -1)
					transfer(source, destination);
				else
					return -1;
			else
				if(link(source, destination) == -1)
					return -1;
				else
					unlink(source);
		else
			return -1;
	else if(S_ISDIR(fileattr2.st_mode))//checked
		if(stat(source, &fileattr1) != -1){
			*ptr3++ = '/';
			strcpy(ptr3, ptr1);
			if(S_ISDIR(fileattr1.st_mode))
				if(stat(destination, &fileattr1) != -1)
					if(!S_ISDIR(fileattr1.st_mode))
						return -1;
					else if((rmdir(destination) == -1) && (errno == ENOTEMPTY || errno == EEXIST))
						return -1;
					else
						if(mkdir(destination, fileattr1.st_mode) != -1)
							transfer(source, destination);
						else
							return -1;
				else
					if(mkdir(destination, fileattr1.st_mode) != -1)
						transfer(source, destination);
					else
						return -1;
			else
				if(stat(destination, &fileattr1) != -1)
					if(S_ISDIR(fileattr1.st_mode))
						return -1;
					else
						if(unlink(destination) == -1)
							return -1;
						else if(link(source, destination) == -1)
							return -1;
						else
							unlink(source);
				else
					if(link(source, destination) == -1)
						return -1;
					else
						unlink(source);
		}				
		else
			return -1;
	else//checked
		if(stat(source, &fileattr1) != -1)
			if(S_ISDIR(fileattr1.st_mode))
				return -1;
			else
				if(unlink(destination) == -1)
					return -1;
				else if(link(source, destination) == -1)
					return -1;
				else
					unlink(source);
				
		else
			return -1;
	
	return 0;
}
