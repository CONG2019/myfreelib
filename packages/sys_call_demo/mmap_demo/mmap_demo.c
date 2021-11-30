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
"");
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
	}
	else{
		usage();
	}

	return 0;
}
