#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>

#include "pipe_demo.h"

void removefifo(void){
	unlink(SERVER_FIFO);
}

int main(int argc, char *argv[]){
	int server_fd, dummy_fd, client_fd;
	char client_fifo[CLIENT_FIFO_NAME_LEN];
	int seqNum = 0;
	request_t req;
	response_t resp;

	if(mkfifo(SERVER_FIFO, S_IRUSR|S_IWUSR|S_IRGRP) == -1){
		printf("server make fifo failed.");
	}

	atexit(removefifo);
	server_fd = open(SERVER_FIFO, O_RDONLY);

	if(server_fd == -1){
		printf("server open %s failed.\n", SERVER_FIFO);
	}

	printf("having client connect.\n");
	dummy_fd = open(SERVER_FIFO, O_WRONLY);
	if(dummy_fd == -1){
		printf("server open %s failed.\n", SERVER_FIFO);
	}

	if(signal(SIGPIPE, SIG_IGN) == SIG_ERR){
		printf("server set signal SIGPIPE SIG_IGN failed.\n");
	}
	while(1){
		if(read(server_fd, &req, sizeof(req)) != sizeof(req)){
			printf("server get an error request.\n");
			continue;
		}

		snprintf(client_fifo, CLIENT_FIFO_NAME_LEN, CLIENT_FIFO_TEMPLATE, (long)req.pid);
		client_fd = open(client_fifo, O_WRONLY);
		if(client_fd == -1){
			printf("server open %s failed.\n", client_fifo);
		}

		resp.seqNum = seqNum;
		if(write(client_fd, &resp, sizeof(resp)) != sizeof(resp)){
			printf("response client failed.\n");
		}

		if(close(client_fd) == -1){
			printf("close client_fd failed.\n");
		}

		seqNum += req.seqLen;
	}
}
