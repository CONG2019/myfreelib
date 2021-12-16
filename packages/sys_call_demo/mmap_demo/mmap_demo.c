#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

static void usage(void){
	printf("Usage: mmap_demo option\n"
"cat filename               run demo like cat\n"
"share file                 share mmap\n");
}

void clean(void){
	return;
}

int cat_demo(int argc, char *argv[]){
	char *addr;
	int fd;
	struct stat sb;

	fd = open(argv[2], O_RDONLY);
	if(fd == -1){
		printf("open file %s fail.\n", argv[2]);
		return -1;
	}

	if(fstat(fd, &sb) == -1){
		printf("get file stat fail.\n");\
		return -1;
	}

	addr = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if(addr == NULL){
		printf("mmap fail.\n");
		return -1;
	}

	if(write(STDOUT_FILENO, addr, sb.st_size) != sb.st_size){
		printf("write to stdout fail.\n");
		return -1;
	}

	munmap(addr, sb.st_size);
	return 0;
}

#define SHARE_MMAP_FILE_PATH "/tmp/share_mmap"
#define MMAP_SIZE 128
//dd if=/dev/zero of=/tmp/share_mmap bs=1 count=128
//od -c -w8 /tmp/share_mmap
int share_demo(int argc, char *argv[]){
	char *addr;
	int fd;

	if(argc < 3){
		printf("please input a string\n");
		return -1;
	}

	fd = open(SHARE_MMAP_FILE_PATH, O_RDWR|O_CREAT, S_IRWXU|S_IRWXG);
	if(fd == -1){
		printf("open file %s fail.\n", SHARE_MMAP_FILE_PATH);
		return -1;
	}

	addr = mmap(NULL, MMAP_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if(addr == MAP_FAILED){
		printf("mmap fail.\n");
		return -1;
	}

	if(close(fd) == -1){
		printf("close fail.\n");
		return -1;
	}

	printf("Current string=%.*s\n", MMAP_SIZE, addr);

	if(strlen(argv[2]) > MMAP_SIZE){
		printf("string too long");
		return -1;
	}

	memset(addr, 0, MMAP_SIZE);
	snprintf(addr, MMAP_SIZE-1, "%s\n", argv[2]);
	//strncpy(addr, argv[2], MMAP_SIZE - 1);
	if(msync(addr, MMAP_SIZE, MS_SYNC) == -1){
		printf("msync fail.\n");
		return -1;
	}

	return 0;
}

int main(int argc, char *argv[]){
	atexit(clean);

	if(argc < 2){
		usage();
		return -1;
	}

	if(!strcmp("cat", argv[1])){
		return cat_demo(argc, argv);
	}else if(!strcmp("show", argv[1])){
		 usage();
	}else if(!strcmp("share", argv[1])){
		return share_demo(argc, argv);
	}
	else{
		usage();
	}

	return 0;
}
