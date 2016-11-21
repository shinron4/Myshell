#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
#include<string.h>
#include<errno.h>

int main(int argc, char *argv[]){
	int i;
	struct directory{
		DIR *dir;
		char dir_name[100]; 
		struct directory *prev;
	} *root = (struct directory *)malloc(sizeof(struct directory)), *temp;
	struct dirent *indir;
	char opt = argv[1][1];
	switch(opt){
		case 'f':i = 2;
			while(argv[i] != 0){
				unlink(argv[i]);
				i++;
			}
		break;
		case 'd':
		case 'r':root->dir = opendir(argv[2]);
			strcpy(root->dir_name, argv[2]);
			root->prev = 0;
			chdir(argv[2]);
			while(root != 0){ 
				while((indir = readdir(root->dir)) != 0){
					if(indir->d_type == DT_REG)
						unlink(indir->d_name);
					else if(indir->d_type == DT_DIR && strcmp(indir->d_name, ".") != 0 && strcmp(indir->d_name, "..") != 0 ){
						temp = (struct directory *)malloc(sizeof(struct directory));
						temp->dir = opendir(indir->d_name);
						temp->prev = root;
						strcpy(temp->dir_name, indir->d_name);
						root = temp;
						chdir(indir->d_name);
						continue;
				}
			}
			closedir(root->dir);
			temp = root;
			root = root->prev;
			chdir("..");
			rmdir(temp->dir_name);
			free(temp);
			}
		break;
	}
	
	return 0;
}
