#ifndef _SYSV_SHM_DEMO
#define _SYSV_SHM_DEMO
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>


#define SHM_KEY 0x1234
#define SEM_KEY 0x5678

#define OBJ_PERMS (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP)

#define WRITE_SEM 0
#define READ_SEM 1

#define BUF_SIZE 1024

typedef struct {
	int cnt;
	char buf[BUF_SIZE];
} shmseg;

union semun {
	int              val;    /* Value for SETVAL */
	struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
	unsigned short  *array;  /* Array for GETALL, SETALL */
	struct seminfo  *__buf;  /* Buffer for IPC_INFO
                               (Linux-specific) */
};
#endif
