#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "pipe_demo.h"

static char client_fifo[CLIENT_FIFO_NAME_LEN];

static void removeFifo(void){
	unlink(client_fifo);
}

int main(int argc, char *argv[]){
	int server_fd, client_fd;
	request_t req;
	response_t resp;

	snprintf(client_fifo, CLIENT_FIFO_NAME_LEN, CLIENT_FIFO_TEMPLATE, (long)getpid());
	if(mkfifo(client_fifo, S_IRUSR|S_IWUSR|S_IRGRP) == -1){
		printf("client make fifo failed.\n");
		return -1;
	}

	if(atexit(removeFifo) != 0){
		printf("atexit failed.\n");
	}

	req.pid = getpid();
	req.seqLen = 1;

	server_fd = open(SERVER_FIFO, O_WRONLY);
	if(server_fd == -1){
		printf("client open server fifo failed.\n");
	}

	if(write(server_fd, &req, sizeof(req)) != sizeof(req)){
		printf("write failed.\n");
	}

	client_fd = open(client_fifo, O_RDONLY);
	if(client_fd == -1){
		printf("open client fifo failed.\n");
	}

	if(read(client_fd, &resp, sizeof(resp)) != sizeof(resp)){
		printf("read server response failed.\n");
	}

	printf("%d\n", resp.seqNum);
	return 0;
}

