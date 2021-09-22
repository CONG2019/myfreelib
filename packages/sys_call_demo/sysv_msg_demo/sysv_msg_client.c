#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "sysv_msg_demo.h"

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
