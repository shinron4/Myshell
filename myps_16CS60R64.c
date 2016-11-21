#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<dirent.h>
#include<unistd.h>
#include<pwd.h>
#include<grp.h>
#include<string.h>

char *conhhmmss(long int seconds)
{
    char *time=(char *)calloc(sizeof(char),10);
    int hr=3600,min=60;
    int hour=seconds/hr;
    int sec=seconds % hr;
    int minute=sec/min;
     sec = sec%min;
    sprintf(time,"%.2d:%.2d:%.2d",hour,minute,sec);   
    return time;
}

char *gettty(char tty[], int major, int minor){
	struct stat fileattr;
	struct dirent *indir;
	struct group *grp;
	char pts[50] = "/dev/pts/", dev[50] = "/dev/", *ptr1 = &pts[strlen(pts)], *ptr2 = &dev[strlen(dev)], temp[4]; 
	DIR *dir1 = opendir("/dev/pts"), *dir2 = opendir("/dev");
	sprintf(temp, "%d", minor);
	while((indir = readdir(dir1)) != 0)
		if(indir->d_type != DT_DIR){
			strcpy(ptr1, indir->d_name);
			stat(pts, &fileattr);
			if(major == major(fileattr.st_rdev))
				return strcat(strcat(strcpy(tty, "pts"), "/"),temp);
		}
	while((indir = readdir(dir2)) != 0)
		if(indir->d_type != DT_DIR){
			strcpy(ptr2, indir->d_name);
			stat(dev, &fileattr);
			if(major == major(fileattr.st_rdev) && strcmp((grp = getgrgid(fileattr.st_gid))->gr_name,"tty") == 0)
				return strcat(strcpy(tty, "tty"), temp);
		}
	strcpy(tty, "?");
	return tty;
}

int main(int argc, char *argv[]){
	DIR *dir;
	struct dirent *indir;
	char process[20], comm[100], temp[100], tty[10], *ptr, opt;
	FILE *fptr;
	int pid, tty_nr, uid, myid = getuid();
        long unsigned int  utime, stime;
	long int cummtime = 0l;
	strcpy(process ,"/proc/");
	ptr = &process[6];
	dir = opendir("/proc");
	chdir("/proc");
	printf("%6s %-5s %12s  %-20s\n", "PID", "TTY", "TIME", "CMD");
	while((indir = readdir(dir)) != 0)
		if(!(strcmp(indir->d_name, ".") == 0 || strcmp(indir->d_name, "..") == 0) && (indir->d_type == DT_DIR) && (atoi(indir->d_name) != 0)){
			strcpy(ptr, indir->d_name);
			strcpy(&process[strlen(process)], "/status");
			fptr = fopen(process, "r");
			temp[0] = '\0';
			while(strcmp(temp, "Uid:"))
				fscanf(fptr, "%s", temp);
			fscanf(fptr, "%d", &uid);
			fclose(fptr);
			opt = '\0';
			if(argv[1] != 0)
				opt =  argv[1][1];
			if( opt == 'a' || myid == uid){
				strcpy(ptr, indir->d_name);
				strcpy(&process[strlen(process)], "/stat");
				fptr = fopen(process, "r");
				fscanf(fptr, "%d (%[^)]) %*c %*d %*d %*d %d %*d %*u %*u %*u %*u %*u %lu %lu", &pid, comm, &tty_nr, &utime, &stime);
				printf("%6d %-5s %12s %-20s\n", pid, gettty(tty, major(tty_nr), minor(tty_nr)), conhhmmss((utime + stime)/sysconf(_SC_CLK_TCK)), comm);
				fclose(fptr);
			}
		}
	chdir("..");
	return 0;
}
