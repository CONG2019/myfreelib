#ifndef __SYSV_MSG_DEMO_H__
#define __SYSV_MSG_DEMO_H__
#include <stddef.h>

#define MY_KEY 1456
#define SERVER_KEY 0x1aaaaaa1

#define PATH_MAX 300

struct mbuf{
	long mtype;
	char mtext[100];
};

typedef struct{
	long mtype;
	int clientId;
	char pathname[PATH_MAX];
} requestMsg;

#define REQ_MSG_SIZE (offsetof(requestMsg, pathname) - \
                     offsetof(requestMsg, clientId) + PATH_MAX)

#define RESP_MSG_SIZE 8192

typedef struct{
	long mtype;
	char data[RESP_MSG_SIZE];
} responseMsg;

#define RESP_MT_FAILURE 1
#define RESP_MT_DATA    2
#define RESP_MT_END     3

#endif
