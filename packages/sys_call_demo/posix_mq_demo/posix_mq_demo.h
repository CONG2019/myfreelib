#ifndef _POSIX_MQ_DEMO
#define _POSIX_MQ_DEMO

#define POSIX_MQ_PATH "/posix_mq_demo"
#define USE_TIMED     1
#define MSG_SIZE      2048
#define MAX_MSG       5
#define NOTIFY_SIG    SIGUSR1

typedef struct {
    int type;
    char data[128];
} msg_t;

#endif