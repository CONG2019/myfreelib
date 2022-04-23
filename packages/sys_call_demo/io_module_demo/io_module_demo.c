#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <signal.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/epoll.h>
#include "io_module_demo.h"

static void usage(void){
	printf("Usage: io_module_demo option\n"
"select               start a select demo\n"
"epoll                start a epoll demo\n"
);
}

void clean(void){
	return;
}

int select_demo(void){
	fd_set rfds;
	struct timeval tv;
	char tmp[2048];
	int ret;

	FD_ZERO(&rfds);
	FD_SET(0, &rfds);

	tv.tv_sec = 10;
	tv.tv_usec = 0;

	ret = select(1, &rfds, NULL, NULL, &tv);

	if(ret == -1){
		printf("select fail.\n");
		return -1;
	}else{
		printf("readable.\n");
		scanf("%s", tmp);
		printf("tv_sec=%ld, input=%s\n", tv.tv_sec, tmp);
	}

	return 0;
}

int epoll_demo(void){
	int epfd, ret, i;
	char tmp[2048];
	struct epoll_event ev;
	struct epoll_event evlist[5];

	epfd = epoll_create(5);
	if(epfd == -1){
		printf("epoll_create failed.\n");
		return -1;
	}

	ev.events = EPOLLIN;
	ev.data.fd = 0;
	if(epoll_ctl(epfd, EPOLL_CTL_ADD, 0, &ev) == -1){
		printf("epoll_ctl fail.\n");
		return -1;
	}

	ret = epoll_wait(epfd, evlist, 5, -1);

	if(ret == -1){
		printf("epoll_wait fail.\n");
		return -1;
	}

	for(i = 0; i < ret; i++){
		printf("fd=%d, events=%s%s%s\n", evlist[i].data.fd, 
		(evlist[i].events & EPOLLIN) ? "EPOLLIN" : "", 
		(evlist[i].events & EPOLLHUP) ? "EPOLLHUP" : "",
		(evlist[i].events & EPOLLERR) ? "EPOLLERR" : "");

		if(evlist[i].events & EPOLLIN){
			scanf("%s", tmp);
			printf("read: %s\n", tmp);
		}
	}

	return 0;
}

int main(int argc, char *argv[]){
	atexit(clean);

	if(argc < 2){
		usage();
		return -1;
	}

	if(!strcmp("select", argv[1])){
		return select_demo();
	}else if(!strcmp("epoll", argv[1])){
		return epoll_demo();
	}
	else{
		usage();
	}

	return 0;
}
