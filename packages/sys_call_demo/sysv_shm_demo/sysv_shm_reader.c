#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <unistd.h>
#include "sysv_shm_demo.h"

shmseg *shmp = (void *) -1;

void clean(void){
	if(shmp != (void *) -1){
		shmdt(shmp);
	}
}

void sig_handler(int sig){
	if(shmp != (void *) -1){
		shmdt(shmp);
	}
}


int main(int argc, char *argv[]){
	int semid, shmid, xfrs, bytes = 0;
	struct sembuf sem_opt;

	atexit(clean);
	signal(SIGINT, sig_handler);
	signal(SIGTERM, sig_handler);


	semid = semget(SEM_KEY, 0, 0);
	if(semid == -1){
		printf("get sem fail.\n");
		return -1;
	}

	shmid = shmget(SHM_KEY, 0, 0);
	if(shmid == -1){
		printf("get shm fail.\n");
		return -1;
	}

	shmp = shmat(shmid, NULL, SHM_RDONLY);
	if(shmp == (void *)-1){
		printf("shmat fail.\n");
		return -1;
	}

	for(xfrs = 0, bytes = 0; ; xfrs++){
		sem_opt.sem_num = READ_SEM;
		sem_opt.sem_op = 0;
		sem_opt.sem_flg = 0;
		if(semop(semid, &sem_opt, 1) == -1){
			printf("wait read fail.\n");
			return -1;
		}

		if(shmp->cnt == 0){
			break;
		}
		bytes += shmp->cnt;

		if(write(STDOUT_FILENO, shmp->buf, shmp->cnt) != shmp->cnt){
			printf("write to stdout fail.\n");
			return -1;
		}

		sem_opt.sem_num = WRITE_SEM;
		sem_opt.sem_op = -1;
		sem_opt.sem_flg = 0;
		if(semop(semid, &sem_opt, 1) == -1){
			printf("release write fail.\n");
			return -1;
		}

		sem_opt.sem_num = READ_SEM;
		sem_opt.sem_op = 1;
		sem_opt.sem_flg = 0;
		if(semop(semid, &sem_opt, 1) == -1){
			printf("set read fail.\n");
			return -1;
		}

	}

	if(shmdt(shmp) == -1){
		printf("shmdt fail.\n");
		return -1;
	}
	shmp = (void *) -1;

	sem_opt.sem_num = WRITE_SEM;
	sem_opt.sem_op = -1;
	sem_opt.sem_flg = 0;
	if(semop(semid, &sem_opt, 1) == -1){
		printf("release write fail.\n");
		return -1;
	}

	printf("reader: bye!");

	return 0;
}
