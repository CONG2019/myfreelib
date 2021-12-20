#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <signal.h>
#include <pthread.h>
#include "posix_shm_demo.h"

static void usage(void){
	printf("Usage: posix_shm_demo option\n"
"create               run a demo to create posix mq\n"
"unlink               delete posix mq\n"
"client               run a shm client\n"
"server               run a shm server\n"
);
}

void clean(void){
	return;
}

int create_demo(){
	int shmd;

	shmd = shm_open(POSIX_SHM_PATH, O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
	if(shmd == -1){
		printf("shm_open failed.\n");
		return -1;
	}

	if(ftruncate(shmd, SHM_SIZE) == -1){
		printf("ftruncate failed.\n");
		return -1;
	}

	return 0;
}

int unlink_demo(){
	shm_unlink(POSIX_SHM_PATH);
	return 0;
}

int server_demo(){
	int shmd;
	shm_t *shmp;

	shmd = shm_open(POSIX_SHM_PATH, O_RDWR, 0);
	if(shmd == -1){
		printf("shm_open failed.\n");
		return -1;
	}

	shmp = (shm_t *)mmap(NULL, SHM_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, shmd, 0);
	if(shmp == MAP_FAILED){
		printf("mmap failed.\n");
		return -1;
	}

	if(sem_init(&shmp->read, 1, 0) == -1){
		printf("init read sem failed.\n");
		return -1;
	}

	if(sem_init(&shmp->write, 1, 1) == -1){
		printf("init write sem failed.\n");
		return -1;
	}

	while(1){
		if(sem_wait(&shmp->read) == -1){
			printf("wait read failed.\n");
		}

		shmp->data[sizeof(shmp->data) - 1] = 0;
		printf("read: %s\n", shmp->data);
		
		if(sem_post(&shmp->write) == -1){
			printf("post write failed.\n");
			return -1;
		}
	}
	
	return 0;
}

int client_demo(){
	int shmd;
	shm_t *shmp;

	shmd = shm_open(POSIX_SHM_PATH, O_RDWR, 0);
	if(shmd == -1){
		printf("shm_open failed.\n");
		return -1;
	}

	shmp = (shm_t *)mmap(NULL, SHM_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, shmd, 0);
	if(shmp == MAP_FAILED){
		printf("mmap failed.\n");
		return -1;
	}

	while(1){
		if(sem_wait(&shmp->write) == -1){
			printf("wait write failed.\n");
			return -1;
		}

		if(scanf("%s", shmp->data) == EOF){
			printf("gets failed.\n");
			return -1;
		}

		if(sem_post(&shmp->read) == -1){
			printf("post read failed.\n");
			return -1;
		}
	}

	return 0;
}

int main(int argc, char *argv[]){
	atexit(clean);

	if(argc < 2){
		usage();
		return -1;
	}

	if(!strcmp("create", argv[1])){
		return create_demo();
	}else if(!strcmp("unlink", argv[1])){
		return unlink_demo();
	}else if(!strcmp("client", argv[1])){
		return client_demo();
	}else if(!strcmp("server", argv[1])){
		return server_demo();
	}
	else{
		usage();
	}

	return 0;
}
