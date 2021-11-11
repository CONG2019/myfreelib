#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include "sysv_shm_demo.h"

int semid = -1;
int shmid = -1;
shmseg *shmp = (void *)-1;

void clean(void){
	if(semid != -1){
		semctl(semid, 2, IPC_RMID);
	}
	if(shmp != (void *)-1 ){
		shmdt(shmp);
	}
	if(shmid != -1){
		shmctl(shmid, IPC_RMID, 0);
	}
}

void sig_handler(int sig){
	if(semid != -1){
		semctl(semid, 2, IPC_RMID);
	}
	if(shmp != (void *)-1 ){
		shmdt(shmp);
	}
	if(shmid != -1){
		shmctl(shmid, IPC_RMID, 0);
	}
}

int main(int argc, char *argv[]){
	int bytes, xfrs;
	unsigned short init[2];
	struct sembuf sem_opt;
	union semun dummy;

	atexit(clean);
	signal(SIGINT, sig_handler);
	signal(SIGTERM, sig_handler);


	semid = semget(SEM_KEY, 2, IPC_CREAT|OBJ_PERMS);
	if(semid == -1){
		printf("creat sem fail.\n");
		return -1;
	}

	init[WRITE_SEM] = 0;	//write available
	init[READ_SEM] = 1;	//read disavailable
	dummy.array = init;

	if(semctl(semid, 2, SETALL, dummy) < 0){
		printf("init sem fail.\n");
		return -1;
	}

#if 0
	init[1] = 0;
	if(semctl(semid, 2, GETALL, dummy) < 0){
		printf("get sem fail.\n");
	}
	printf("sem[0]=%d, sem[1]=%d\n", init[0], init[1]);
#endif

	shmid = shmget(SHM_KEY, sizeof(shmseg), IPC_CREAT|OBJ_PERMS);
	if(shmid == -1){
		printf("creat shm fail.\n");
		return -1;
	}
	shmp = shmat(shmid, NULL, 0);
	if(shmp == (void *) -1){
		printf("shmat fail.\n");
		return -1;
	}

	for(xfrs = 0, bytes = 0; ; xfrs++, bytes += shmp->cnt){
		sem_opt.sem_num = WRITE_SEM;
		sem_opt.sem_op = 0;
		sem_opt.sem_flg = 0;
		//wait for write
		if(semop(semid, &sem_opt, 1) == -1){
			printf("wait WRITE_SEM fail.\n");
			return -1;
		}
	
		shmp->cnt = read(STDIN_FILENO, shmp->buf, BUF_SIZE);
		if(shmp->cnt == -1){
			printf("read stdin fail.\n");
			return -1;
		}

		sem_opt.sem_num = READ_SEM;
		sem_opt.sem_op = -1;
		sem_opt.sem_flg = 0;
		// release read sem
		if(semop(semid, &sem_opt, 1) == -1){
			printf("release read_sem fail.\n");
			return -1;
		}

		sem_opt.sem_num = WRITE_SEM;
		sem_opt.sem_op = 1;
		sem_opt.sem_flg = 0;
		if(semop(semid, &sem_opt, 1) == -1){
			printf("set write fail.\n");
			return -1;
		}

		if(shmp->cnt == 0){
			break;
		}
	}

	sem_opt.sem_num = WRITE_SEM;
	sem_opt.sem_op = 0;
	sem_opt.sem_flg = 0;
	//wait for read finsh
	if(semop(semid, &sem_opt, 1) == -1){
		printf("wait WRITE_SEM fail.\n");
		return -1;
	}

	return 0;
}
