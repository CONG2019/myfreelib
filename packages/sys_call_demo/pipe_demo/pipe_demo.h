#ifndef __PIPE_DEMO_H__
#define __PIPE_DEMO_H__

#define SERVER_FIFO "/tmp/seqnum_sv"
#define CLIENT_FIFO_TEMPLATE "/tmp/seqnum_cl.%ld"
#define CLIENT_FIFO_NAME_LEN (sizeof(CLIENT_FIFO_TEMPLATE) + 20)

typedef struct{
	pid_t pid;
	int seqLen;
} request_t;

typedef struct{
	int seqNum;
} response_t;

#endif
