#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include "sysv_msg_demo.h"

static int msgid;

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
