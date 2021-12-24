#ifndef _POSIX_SHM_DEMO
#define _POSIX_SHM_DEMO

#include <semaphore.h>

#define POSIX_SHM_PATH "/posix_shm_demo"
#define SHM_SIZE      sizeof(shm_t)

typedef struct {
    sem_t read;
    sem_t write;
    char data[1024];
} shm_t;

#endif