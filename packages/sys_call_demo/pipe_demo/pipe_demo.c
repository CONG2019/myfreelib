#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SIZE_OF_READ_BUF	100

static int pipe_demo(void){
	int pipefd[2];
	char buf[SIZE_OF_READ_BUF];

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

int main(int argc, char *agrv[]){
	int ret;

	ret = pipe_demo();

	return ret;
}
