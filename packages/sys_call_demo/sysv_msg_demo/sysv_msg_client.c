#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "sysv_msg_demo.h"

#ifdef NEW_SERVER_CLIENT_MODEL
static int clientId;
static void removeQueue(void){
	if(msgctl(clientId, IPC_RMID, NULL) == -1){
		printf("fail to remove messgae %d.\n", clientId);
	}
}

int main(int argc, char *argv[100]){
	requestMsg req;
	responseMsg resp;
	int fd;
	int serverId, numMsgs;
	ssize_t msglen, totBytes;

	if(argc != 2 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0){
		printf("%s pathname\n", argv[0]);
		return -1;
	}

	if(strlen(argv[1]) > sizeof(req.pathname) - 1){
		printf("pathname too long.\n");
		return -1;
	}

	serverId = msgget(SERVER_KEY, S_IWUSR);
	if(serverId == -1){
		printf("fail to open server messgae.\n");
		return -1;
	}

	clientId = msgget(IPC_PRIVATE, S_IRUSR | S_IWUSR | S_IWGRP);
	if(clientId == -1){
		printf("fail to create client messgae queue.\n");
		return -1;
	}

	if(atexit(removeQueue) != 0){
		printf("ateixt fail.\n");
		return -1;
	}

	req.mtype = 1; /* any types will do */
	req.clientId = clientId;
	strncpy(req.pathname, argv[1], sizeof(req.pathname) - 1);
	req.pathname[sizeof(req.pathname) - 1] = '\0';

	if(msgsnd(serverId, &req, REQ_MSG_SIZE, 0) == -1){
		printf("send messgae to server fail.\n");
		return -1;
	}

	msglen = msgrcv(clientId, &resp, RESP_MSG_SIZE, 0, 0);
	if(msglen == -1){
		printf("fail to get messgae from client queue.\n");
		return -1;
	}

	if(resp.mtype == RESP_MT_FAILURE){
		printf("%s\n", resp.data);
		return -1;
	}

	totBytes = msglen;
	fd = open("/tmp/sysv_msg_recive", O_CREAT|O_CLOEXEC|O_RDWR, S_IRUSR | S_IWUSR | S_IWGRP);
	ftruncate(fd, 0);
	lseek(fd, 0, SEEK_SET);
	if(write(fd, resp.data, msglen) == -1){
		printf("write fail.\n");
		//return -1;
	}
	for(numMsgs = 1; resp.mtype == RESP_MT_DATA; numMsgs++){
		msglen = msgrcv(clientId, &resp, RESP_MSG_SIZE, 0, 0);
		if(msglen == -1){
			printf("msgrcv fail.\n");
		}
		totBytes += msglen;

		if(write(fd, resp.data, msglen) == -1){
			printf("write fail.\n");
			//return -1;
		}
	}

	close(fd);
	printf("received %ld bytes (%d messgae)\n", (long)totBytes, numMsgs);
	return 0;
}
#else
int main(int argc, char *argv[]){
	int msgid;
	struct mbuf msg = {
		.mtype = 100,
		.mtext = "hello"
	};

	msgid = msgget(MY_KEY, 0);
	if(msgid == -1){
		printf("Get msgid fail.\n");
		return -1;
	}

	printf("send messgae.\n");
	return msgsnd(msgid, &msg, sizeof(msg.mtext), 0);
}
#endif
