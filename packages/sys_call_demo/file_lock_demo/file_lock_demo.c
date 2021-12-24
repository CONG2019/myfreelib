#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/file.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <signal.h>
#include <pthread.h>
#include "file_lock_demo.h"

//forec add lock: chmod g+s，g-x FILE_LOCK_PATH
static void usage(void){
	printf("Usage: file_lock_demo option\n"
"init                          initialization\n"
"share_lc                      add a share lock\n"
"mutex_lc                      add a mutex lock\n"
"fcntl_re                      add a read lock\n"
"fcntl_wr                      add a write lock\n"
"fcntl_un                      delect a lock\n"
);
}

void clean(void){
	return;
}

int init(){
	char cmd[128];
	snprintf(cmd, sizeof(cmd), "ps -A > %s", FILE_LOCK_PATH);
	return system(cmd);
}

int share_lc_demo(){
	int fd;
	int pid;

	pid = (int)getpid();

	fd = open(FILE_LOCK_PATH, O_RDWR);
	if(fd == -1){
		printf("open file %s failed.\n", FILE_LOCK_PATH);
		return -1;
	}

	if(flock(fd, LOCK_SH) == -1){
		printf("add LOCK_SH failed.\n");
		return -1;
	}

	printf("process %d add LOCK_SH.\n", pid);

	sleep(10);

	if(flock(fd, LOCK_UN) == -1){
		printf("delete LOCK_SH failed.\n");
		return -1;
	}

	printf("process %d delete LOCK_SH.\n", pid);
	close(fd);
	return 0;
}

int mutex_lc_demo(){
	int fd;
	int pid;

	pid = (int)getpid();

	fd = open(FILE_LOCK_PATH, O_RDWR);
	if(fd == -1){
		printf("open file %s failed.\n", FILE_LOCK_PATH);
		return -1;
	}

	if(flock(fd, LOCK_EX) == -1){
		printf("add LOCK_EX failed.\n");
		return -1;
	}

	printf("process %d add LOCK_EX.\n", pid);

	sleep(10);

	if(flock(fd, LOCK_UN) == -1){
		printf("delete LOCK_EX failed.\n");
		return -1;
	}

	printf("process %d delete LOCK_EX.\n", pid);
	close(fd);
	return 0;
}

int fcntl_re_demo(){
	int fd;
	struct flock fl;
	int pid;

	pid = (int)getpid();

	fd = open(FILE_LOCK_PATH, O_RDWR);
	if(fd == -1){
		printf("open file %s failed.\n", FILE_LOCK_PATH);
		return -1;
	}

	fl.l_type = F_RDLCK;
	fl.l_whence = SEEK_CUR;
	fl.l_start = 0;
	fl.l_len = 0;

	if(fcntl(fd, F_SETLKW, &fl) == -1){
		printf("fcntl failed.\n");
		return 0;
	}
	printf("pid(%d) add read lock.\n", pid);
	sleep(10);
	printf("pid(%d) exit.\n", pid);

	return 0;
}

int fcntl_wr_demo(){
	int fd;
	struct flock fl;
	int pid;

	pid = (int)getpid();

	fd = open(FILE_LOCK_PATH, O_RDWR);
	if(fd == -1){
		printf("open file %s failed.\n", FILE_LOCK_PATH);
		return -1;
	}

	fl.l_type = F_WRLCK;
	fl.l_whence = SEEK_CUR;
	fl.l_start = 0;
	fl.l_len = 0;

	if(fcntl(fd, F_SETLKW, &fl) == -1){
		printf("fcntl failed.\n");
		return 0;
	}
	printf("pid(%d) add write lock.\n", pid);
	sleep(10);
	printf("pid(%d) exit.\n", pid);

	return 0;
}

int fcntl_un_demo(){
	int fd;
	struct flock fl;
	int pid;

	pid = (int)getpid();

	fd = open(FILE_LOCK_PATH, O_RDWR);
	if(fd == -1){
		printf("open file %s failed.\n", FILE_LOCK_PATH);
		return -1;
	}

	fl.l_type = F_UNLCK;
	fl.l_whence = SEEK_CUR;
	fl.l_start = 0;
	fl.l_len = 0;

	if(fcntl(fd, F_SETLKW, &fl) == -1){
		printf("fcntl failed.\n");
		return 0;
	}
	printf("pid(%d) delete a write lock.\n", pid);
	sleep(10);
	printf("pid(%d) exit.\n", pid);

	return 0;
}

int main(int argc, char *argv[]){
	atexit(clean);

	if(argc < 2){
		usage();
		return -1;
	}

	if(!strcmp("share_lc", argv[1])){
		return share_lc_demo();
	}else if(!strcmp("mutex_lc", argv[1])){
		return mutex_lc_demo();
	}else if(!strcmp("init", argv[1])){
		return init();
	}else if(!strcmp("fcntl_re", argv[1])){
		return fcntl_re_demo();
	}else if(!strcmp("fcntl_wr", argv[1])){
		return fcntl_wr_demo();
	}else if(!strcmp("fcntl_un", argv[1])){
		return fcntl_un_demo();
	}
	else{
		usage();
	}

	return 0;
}
