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
#include "posix_sem_demo.h"

static void usage(void){
	printf("Usage: posix_mq_demo option\n"
"create               run a demo to create posix sem\n"
"unlink               delete a posix sem\n"
"wait                 wati to sem > 0\n"
"post                 add 1\n"
"get                  get value\n"
"uname                uname posix sem\n"
);
}

void clean(void){
	return;
}

int create_demo(){
	int flags;
	mode_t perms;
	sem_t *sem;

	flags = O_CREAT | O_EXCL;
	perms = S_IRUSR | S_IWUSR;

	sem = sem_open(POSIX_SEM_PATH, flags, perms, POSIX_SEM_INIT);
	if(sem == SEM_FAILED){
		printf("sem_open failed.\n");
		return -1;
	}

	sem_close(sem);
	return 0;
}

int unlink_demo(){
	if(sem_unlink(POSIX_SEM_PATH) == -1){
		printf("sem_unlink failed.\n");
		return -1;
	}

	return 0;
}

int wait_demo(){
	sem_t *sem;
#if POSIX_SEM_TIMED
	struct timespec time;
	if(clock_gettime(0, &time) == -1){
		printf("clock_gettime failed.\n");
		return -1;
	}
	time.tv_sec += 10;
#endif
	
	sem = sem_open(POSIX_SEM_PATH, 0);
	if(sem == SEM_FAILED){
		printf("sem_open failed.\n");
		return -1;	
	}
	printf("waitting sem...\n");

#if POSIX_SEM_TIMED
	if(sem_timedwait(sem, &time) == -1){
		printf("timeout.\n");
		return -1;
	}
#else
	if(sem_wait(sem) == -1){
		printf("sem_wait failed.\n");
		return -1;
	}
#endif

	printf("finshed.\n");
	sem_close(sem);
	return 0;
}

int post_demo(){
	sem_t *sem;

	sem = sem_open(POSIX_SEM_PATH, 0);
	if(sem == SEM_FAILED){
		printf("sem_open failed.\n");
		return -1;	
	}

	if(sem_post(sem) == -1){
		printf("sem_post failed.\n");
		return -1;
	}

	sem_close(sem);
	return 0;
}

int get_demo(){
	sem_t *sem;
	int value;

	sem = sem_open(POSIX_SEM_PATH, 0);
	if(sem == SEM_FAILED){
		printf("sem_open failed.\n");
		return -1;	
	}

	if(sem_getvalue(sem, &value) == -1){
		printf("sem_getvalue failed.\n");
		return -1;
	}

	printf("posix sem value is: %d\n", value);
	sem_close(sem);
	return 0;
}

static int glob = 0;
static sem_t uname_sem;

static void *threadFunc(void *arg){
	printf("thread waitting sem...\n");
	if(sem_wait(&uname_sem) == -1){
		printf("sem_wait fialed.\n");
		return (void *)-1;
	}

	glob++;
	printf("thread finsh.\n");
	return NULL;
}

int uname_demo(){
	pthread_t t1, t2;

	if(sem_init(&uname_sem, 0, 0) == -1){
		printf("sem_init failed.\n");
		return -1;
	}

	if(pthread_create(&t1, NULL, threadFunc, NULL) != 0){
		printf("create t1 failed.\n");
	}

	if(pthread_create(&t2, NULL, threadFunc, NULL) != 0){
		printf("create t2 failed.\n");
	}

	sleep(3);
	sem_post(&uname_sem);
	sleep(3);
	sem_post(&uname_sem);

	if(pthread_join(t1, NULL) != 0){
		printf("join thread t1 failed.\n");
	}

	if(pthread_join(t2, NULL) != 0){
		printf("join thread t2 failed.\n");
	}

	printf("glob=%d\n", glob);
	sem_destroy(&uname_sem);
	printf("uname finsh.\n");
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
	}else if(!strcmp("wait", argv[1])){
		return wait_demo();
	}else if(!strcmp("post", argv[1])){
		return post_demo();
	}else if(!strcmp("get", argv[1])){
		return get_demo();
	}else if(!strcmp("uname", argv[1])){
		return uname_demo();
	}
	else{
		usage();
	}

	return 0;
}
