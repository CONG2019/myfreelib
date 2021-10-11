#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include "sysv_msg_demo.h"

static int msgid;

#ifdef NEW_SERVER_CLIENT_MODEL
static void sig_handler(int sig){
	printf("Receive signal(%d).\n", sig);
	msgctl(msgid, SERVER_KEY, NULL);
	exit(-1);
}

void clean(void){
	printf("Remove message queue.\n");
	msgctl(msgid, SERVER_KEY, NULL);
}

static void grimReaper(int sig){
	int savedError;

	savedError = errno;

	while(waitpid(-1, NULL, WNOHANG) > 0){
		continue;
	}

	errno = savedError;
}

static void serverRequest(const requestMsg *req){
	int fd;
	ssize_t numRead;
	responseMsg resp;

	fd = open(req->pathname, O_RDONLY);
	if(fd == -1){
		resp.mtype = RESP_MT_FAILURE;
		snprintf(resp.data, sizeof(resp.data), "Couldn't open %s.\n", req->pathname);
		msgsnd(req->clientId, &resp, strlen(resp.data) + 1, 0);
		return exit(EXIT_FAILURE);
	}

	resp.mtype = RESP_MT_DATA;
	while((numRead = read(fd, resp.data, RESP_MSG_SIZE)) > 0){
		if(msgsnd(req->clientId, &resp, numRead, 0) == -1){
			break;
		}
	}

	resp.mtype = RESP_MT_END;
	msgsnd(req->clientId, &resp, 0, 0);
}

int main(int argc, char *argv[]){
	requestMsg req;
	pid_t pid;
	ssize_t msglen;
	int serverId;
	struct sigaction sa;

	serverId = msgget(SERVER_KEY, IPC_CREAT|IPC_EXCL|S_IRUSR|S_IWUSR|S_IRGRP);
	if(serverId == -1){
		printf("Create server message queue fail.\n");
		return -1;
	}

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sa.sa_handler = grimReaper;
	if(sigaction(SIGCHLD, &sa, NULL) == -1){
		printf("Failed to create signal processor.\n");
		return -1;
	}

	msgid = serverId;
	atexit(clean);
	sa.sa_handler = sig_handler;
	if(sigaction(SIGINT, &sa, NULL) == -1){
		printf("Failed to create signal processor.\n");
		return -1;
	}
	if(sigaction(SIGTERM, &sa, NULL) == -1){
		printf("Failed to create signal processor.\n");
		return -1;
	}

	while(1){
		msglen = msgrcv(serverId, &req, REQ_MSG_SIZE, 0, 0);
		if(msglen == -1){
			if(errno == EINTR){	/* interrupted by SIGCHLD handler */
				continue;
			}else{
				printf("msgrcv fail.\n");
				return -1;
			}
		}

		pid = fork();
		if(pid == -1){
			printf("fork fail.\n");
			break;
		}

		if(pid == 0){
			serverRequest(&req);
			return 0;
		}

		/* parent will continue */
	}
}
#else
static void sig_handler(int sig){
	printf("Receive signal(%d).\n", sig);
	msgctl(msgid, IPC_RMID, NULL);
	exit(-1);
}

void clean(void){
	printf("Remove message queue.\n");
	msgctl(msgid, IPC_RMID, NULL);
}

int main(int argc, char *argv[]){
	struct mbuf msg;

	signal(SIGINT, sig_handler);
	signal(SIGTERM, sig_handler);
	atexit(clean);

	msgid = msgget(MY_KEY, IPC_CREAT|IPC_EXCL|S_IRUSR|S_IWUSR|S_IRGRP);
	if(msgid == -1){
		printf("Create message queue fail, errno=%d.\n", errno);
		return -1;
	}

	while(1){
		if(msgrcv(msgid, &msg, sizeof(msg.mtext), 0, 0) != -1){
			printf("Receive message: type=%ld, data=%s.\n", msg.mtype, msg.mtext);
		}else{
			sleep(1);
		}
	}

	return -1;
}
#endif
