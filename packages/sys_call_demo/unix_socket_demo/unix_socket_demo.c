#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <signal.h>
#include <pthread.h>
#include "unix_socket_demo.h"

static void usage(void){
	printf("Usage: unix_socket_demo option\n"
"str_server                    start a stream socket server\n"
"str_client                    start a stream socket client\n"
"data_server                   start a data socket server\n"
"data_client                   start a data socket client\n"
);
}

void clean(void){
	return;
}

void *str_server(void *cfd){
	int *pcfd = (int *)cfd;
	char msg[UNIX_SOCKET_MSG_SIZE];
	pthread_t pid;
	pid = pthread_self();

	while(recv(*pcfd, msg, sizeof(msg), 0) > 0){
		printf("thread[%lu]: %s\n", pid, msg);
	}

	printf("thread[%lu]: client leave\n", pid);
	close(*pcfd);
	free(cfd);
	return NULL;
}

int str_server_demo(){
	int sfd, cfd;
	int *pcfd;
	struct sockaddr_un saddr, csaddr;
	socklen_t addr_len;
	pthread_attr_t attr;
	pthread_t thread;

	addr_len = sizeof(struct sockaddr_un);
	sfd = socket(AF_UNIX, SOCK_STREAM, 0);
	if(sfd == -1){
		printf("socket failed.\n");
		return -1;
	}

	remove(UNIX_SOCKET_PATH);
	saddr.sun_family = AF_UNIX;
	printf("len of sun_path=%lu\n", sizeof(saddr.sun_path));
	strncpy(saddr.sun_path, UNIX_SOCKET_PATH, sizeof(saddr.sun_path));

	if(bind(sfd, (struct sockaddr *)&saddr, sizeof(struct sockaddr_un)) == -1){
		printf("bind failed.\n");
		return -1;
	}

	if(listen(sfd, 10) == -1){
		printf("listen failed.\n");
		return -1;
	}

	printf("waitting client to connect\n");
	while(1){
		cfd = accept(sfd, (struct sockaddr *)&csaddr, &addr_len);
		if(cfd == -1){
			printf("accept failed.\n");
			return -1;
		}else{
			printf("accept a client connect, cfd=%d, sun_path=%s\n", cfd, csaddr.sun_path);
			pcfd = (int *)malloc(sizeof(int));
			if(pcfd != NULL){
				*pcfd = cfd;
			}
			pthread_attr_init(&attr);
			pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
			if(pthread_create(&thread, &attr, str_server, (void *)pcfd) != 0){
				printf("pthread_create failed.\n");
				close(cfd);
				free(pcfd);
			}
			pthread_attr_destroy(&attr);
		}
	}

	close(sfd);
	return 0;	
}

int str_client_demo(){
	int cfd;
	socklen_t addr_len;
	struct sockaddr_un saddr;
	char msg[UNIX_SOCKET_MSG_SIZE];

	addr_len = sizeof(struct sockaddr_un);
	cfd = socket(AF_UNIX, SOCK_STREAM, 0);
	if(cfd == -1){
		printf("socket failed.\n");
		return -1;
	}

	saddr.sun_family = AF_UNIX;
	strncpy(saddr.sun_path, UNIX_SOCKET_PATH, sizeof(saddr.sun_path));

	if(connect(cfd, (struct sockaddr *)&saddr, addr_len) == -1){
		printf("connect failed.\n");
		return -1;
	}

	while(fgets(msg, sizeof(msg), stdin) != NULL){
		send(cfd, msg, sizeof(msg), 0);
	}

	close(cfd);
	return 0;
}

int data_server_demo(){
	int sfd;
	socklen_t addr_len;
	struct sockaddr_un saddr, csaddr;
	char msg[UNIX_SOCKET_MSG_SIZE];

	addr_len = sizeof(saddr);
	sfd = socket(AF_UNIX, SOCK_DGRAM, 0);
	if(sfd == -1){
		printf("socket failed.\n");
		return -1;
	}
	
	remove(UNIX_SOCKET_PATH);
	saddr.sun_family = AF_UNIX;
	strncpy(saddr.sun_path, UNIX_SOCKET_PATH, sizeof(saddr.sun_path));

	if(bind(sfd, (struct sockaddr *)&saddr, sizeof(saddr)) == -1){
		printf("bind failed.\n");
		return -1;
	}

	while(recvfrom(sfd, msg, sizeof(msg), 0, (struct sockaddr *)&csaddr, &addr_len) != -1){
		printf("recv from %s: %s\n", csaddr.sun_path, msg);
	}

	printf("bye...\n");
	close(sfd);
	return 0;	
}

int data_client_demo(){
	int cfd;
	socklen_t addr_len;
	struct sockaddr_un saddr;
	char msg[UNIX_SOCKET_MSG_SIZE];

	addr_len = sizeof(saddr);
	cfd = socket(AF_UNIX, SOCK_DGRAM, 0);
	if(cfd == -1){
		printf("socket failed.\n");
		return -1;
	}
	
	saddr.sun_family = AF_UNIX;
	strncpy(saddr.sun_path, UNIX_SOCKET_PATH, sizeof(saddr.sun_path));

	while(fgets(msg, sizeof(msg), stdin) != NULL){
		sendto(cfd, msg, sizeof(msg), 0, (struct sockaddr *)&saddr, addr_len);
	}
	return 0;
}

int main(int argc, char *argv[]){
	atexit(clean);

	if(argc < 2){
		usage();
		return -1;
	}

	if(!strcmp("str_server", argv[1])){
		return str_server_demo();
	}else if(!strcmp("str_client", argv[1])){
		return str_client_demo();
	}else if(!strcmp("data_server", argv[1])){
		return data_server_demo();
	}else if(!strcmp("data_client", argv[1])){
		return data_client_demo();
	}
	else{
		usage();
	}

	return 0;
}
