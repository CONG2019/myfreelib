#include <stdio.h>
#include <stdlib.h>
#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <fcntl.h>
#include "sysv_msg_demo.h"

static int msgid = -1;

static void usage(void){
	printf("Usage: sysv_msg_demo option\n"
"ctl                        run msgctl test code\n"
"show                       show all message queue in system");
}

static void clean(void){
	printf("Bye!\n");
	if(msgid != -1){
		msgctl(msgid, IPC_RMID, NULL);
	}
}

static int sysv_msg_ctl(void){
	struct msqid_ds msg_ds;

	msgid = msgget(MY_KEY, IPC_CREAT | IPC_EXCL);
	if(msgid == -1){
		printf("Create message queue fail.\n");
		return -1;
	}

	if(msgctl(msgid, IPC_STAT, &msg_ds) == -1){
		printf("msgctl get IPC_STAT fail.\n");
		return -1;
	}

	printf("msg_qbytes=%ld, mode=%u\n", msg_ds.msg_qbytes, msg_ds.msg_perm.mode);

	msg_ds.msg_qbytes = 1024000;
	msg_ds.msg_perm.mode = msg_ds.msg_perm.mode|S_IRUSR|S_IWUSR|S_IRGRP;

	if(msgctl(msgid, IPC_SET, &msg_ds) == -1){
		printf("msgctl get IPC_SET fail.\n");
		return -1;
	}

	if(msgctl(msgid, IPC_STAT, &msg_ds) == -1){
		printf("msgctl get IPC_STAT fail.\n");
		return -1;
	}

	printf("After set: msg_qbytes=%ld, mode=%u\n", msg_ds.msg_qbytes, msg_ds.msg_perm.mode);

	return 0;
}

static int sysv_msg_show(void){
	int maxind, ind, msqid;
	struct msqid_ds ds;
	struct msginfo msginfo;

	/* Obtain size of kernel 'entries' array */
	maxind = msgctl(0, MSG_INFO, (struct msqid_ds *)&msginfo);
	if(maxind == -1){
		printf("msgctl MSG_INFO fail.\n");
		return -1;
	}

	printf("maxind:%d\n\n", maxind);
	printf("index\tid\tkey\tmessage\t\n");

	for(ind = 0; ind <= maxind; ind++){
		msqid = msgctl(ind, MSG_STAT, &ds);
		if(msqid == -1){
			//printf("msgctl MSG_STAT[%d] fail, continue\n", ind);
			continue;
		}

		printf("%d\t%d\t%ld\t%ld\n", ind, msqid, (long)ds.msg_perm.__key, (long)ds.msg_qnum);
	}

	return 0;
}

int main(int argc, char *argv[]){
	atexit(clean);

	if(argc != 2){
		usage();
		return -1;
	}

	if(!strcmp("ctl", argv[1])){
		return sysv_msg_ctl();
	}else if(!strcmp("show", argv[1])){
		return sysv_msg_show();
	}
	else{
		usage();
	}

	return 0;
}
