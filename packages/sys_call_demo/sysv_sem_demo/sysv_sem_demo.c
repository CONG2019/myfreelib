#include <stdio.h>
#include <stdlib.h>
#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "sysv_sem_demo.h"

static int semid = -1;

static void usage(void){
	printf("Usage: sysv_sem_demo option\n"
"simple argv                run simple test\n"
"\n");
}

static int sysv_sem_simple(int argc, char *argv[]){
	if(argc < 3 || argc > 4 ){
		usage();
		return 0;
	}

	if(argc == 3){
		union semun arg;
		semid = semget(IPC_PRIVATE, 1, S_IRUSR|S_IWUSR);
		if(semid == -1){
			printf("create sem fail.\n");
			return -1;
		}

		arg.val = atoi(argv[2]);
		if(semctl(semid, 0, SETVAL, arg) == -1){
			printf("SETVAL fail.\n");
			return -1;
		}

		printf("Semaphore ID = %d\n", semid);
	}else{
		struct sembuf sop;

		semid = atoi(argv[2]);

		sop.sem_num = 0;
		sop.sem_op = atoi(argv[3]);
		
		sop.sem_flg = 0;

		printf("%ld: about to semop\n", (long)getpid());
		if(semop(semid, &sop, 1) == -1){
			printf("semop fail.\n");
			return -1;
		}
		printf("%ld: semop completed\n", (long)getpid());
	}

	return 0;
}

int main(int argc, char *argv[]){
	//atexit(clean);

	if(argc < 2){
		usage();
		return -1;
	}

	if(!strcmp("simple", argv[1])){
		return sysv_sem_simple(argc, argv);
	}
	else{
		usage();
	}

	return 0;
}
