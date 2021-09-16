#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define SIZE_OF_READ_BUF	100

void usage(void){
	printf("usage: pipe_demo demo/sync/redirect\n");
}

static int pipe_demo(void){
	int pipefd[2];
	char buf[SIZE_OF_READ_BUF];

	printf("%s start\n", __func__);
	if(pipe(pipefd) == -1){
		printf("Creating pipe failed.\n");
		return -1;
	}

	switch(fork()){
		case -1:
			printf("Fork failed.\n");
			return -1;
		case 0:
			//child
			if(close(pipefd[1]) == -1){
				printf("Child close write pipefd failed.\n");
				return -1;
			}

			/* reading the pipe */
			read(pipefd[0], buf, sizeof(buf));
			printf("Child read: %s\n", buf);
			break;

		default:
			//parent
			if(close(pipefd[0]) == -1){
				printf("Child close read pipefd failed.\n");
				return -1;
			}

			/* writing the pipe */
			printf("Parent write pipe.\n");
			write(pipefd[1], "123456789", 10);
			break;
	}

	return 0;
}

static int pipe_sync(void){
	int pipefd[2];
	int i;
	char buf[SIZE_OF_READ_BUF];

	printf("%s start\n", __func__);
	if(pipe(pipefd) == -1){
		printf("Creating pipe failed.\n");
		return -1;
	}

	for(i = 0; i < 2; i++){
		switch(fork()){
			case -1:
				printf("fork failed.\n");
				return -1;
			case 0:
				//child
				if(close(pipefd[0]) == -1){
					printf("close read end failed.\n");
					return -1;
				}
				sleep(i+3);
				printf("Child [PID=%d] closing pipe write end.\n", getpid());
				close(pipefd[1]);
				return 0;
			default:
				//parent
				//loops to create next child
				break;
		}
	}

	if(close(pipefd[1]) == -1){
		printf("parent close write end failed.\n");
		return -1;
	}

	printf("Waiting for child close write\n");
	if(read(pipefd[0], buf, SIZE_OF_READ_BUF) !=0){
		printf("parent didn't get EOF\n");
	}

	printf("all child close write end, parent ready to go.\n");
	return 0;
}

static int pipe_redirect(void){
	int pipefd[2];
	char buf[SIZE_OF_READ_BUF];

	printf("%s start\n", __func__);
	if(pipe(pipefd) == -1){
		printf("Creating pipe failed.\n");
		return -1;
	}

	switch(fork()){
		case -1:
			printf("fork failed.\n");
			return -1;
		case 0:
			//child
			close(pipefd[1]);
			while(read(pipefd[0], buf, SIZE_OF_READ_BUF)){
				printf("%s", buf);
				fflush(stdout);
			}
			return 0;
		default:
			//parent
			close(pipefd[0]);
			dup2(pipefd[1], 0);
			write(pipefd[1], "abc===\n", 4);
			sleep(10);
			break;
	}

	return 0;
}

int main(int argc, char *argv[]){
	int ret;

	if(argc < 2){
		usage();
		return -1;
	}

	if(!strcmp("demo", argv[1])){
		ret = pipe_demo();
	}else if(!strcmp("sync", argv[1])){
		ret = pipe_sync();
	}else if(!strcmp("redirect", argv[1])){
		ret = pipe_redirect();
	}

	return ret;
}
