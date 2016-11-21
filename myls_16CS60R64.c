#include<stdio.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<pwd.h>
#include<grp.h>
#include<time.h>
#include<dirent.h>

void printTime(time_t time){
	struct tm *ltm;
	char strtime[100];
	ltm = localtime(&time);
	strftime(strtime,100, "%Y %b %d %X", ltm);
	printf("%s", strtime);
}

void fileOwner(uid_t user, uid_t group){
	struct passwd *pwd;
	struct group *grp;
	if((pwd = getpwuid(user)) != 0)
		printf("%s ", pwd->pw_name);
	else 
		printf("%d ", user);
	if((grp = getgrgid(group)) != 0)
		printf("%s", grp->gr_name);
	else
		printf("%d", group);
}

void printPermission(mode_t mode){
	printf("%c%c%c%c%c%c%c%c%c%c",
	S_ISDIR(mode)? 'd':'-',
	mode & S_IRUSR ? 'r':'-',
	mode & S_IWUSR ? 'w':'-',
	mode & S_IXUSR ? 'x':'-',
	mode & S_IRGRP ? 'r':'-',
	mode & S_IWGRP ? 'w':'-',
	mode & S_IXGRP ? 'x':'-',
	mode & S_IROTH ? 'r':'-',
	mode & S_IWOTH ? 'w':'-',
	mode & S_IXOTH ? 'x':'-');
}

size_t printFileAttr(int dirandsymlinks, char *file){
	struct stat fileattr;
	stat(file, &fileattr);
	printPermission(fileattr.st_mode);
	printf("\t%d\t", dirandsymlinks);
	fileOwner(fileattr.st_uid, fileattr.st_gid);
	printf("\t%zu\t", fileattr.st_size);
	printTime(fileattr.st_mtime);
	printf("\t%s\n", file);
	return fileattr.st_blocks;
}

int main(int argc, char *argv[]){
	DIR *dir, *childDir;
	struct dirent *indir, *childindir;
	int dirandsymlinks = 0;
	size_t totalblks = 0;
	dir = opendir("./");
	chdir("./");
	while((indir = readdir(dir)) != 0)
		if(!(strcmp(indir->d_name, ".") == 0 || strcmp(indir->d_name, "..") == 0) && indir->d_type == DT_DIR){
			childDir = opendir(indir->d_name);
			while((childindir = readdir(childDir)) != 0)
				switch(childindir->d_type)
					case DT_DIR:
					case DT_LNK:dirandsymlinks++;
			printFileAttr(dirandsymlinks, indir->d_name);
			dirandsymlinks = 0;
		}
		else if(indir->d_type == DT_REG || indir->d_type == DT_LNK)
			totalblks += printFileAttr(1, indir->d_name);
	printf("total %zu\n", totalblks);
	chdir("..");
	return 0;
}
