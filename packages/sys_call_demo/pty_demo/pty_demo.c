#define _GNU_SOURCE
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
#include <termios.h>
#include <sys/ioctl.h>
#include <termios.h>
#include "pty_demo.h"

//extern int posix_openpt(int flags);
static void usage(void){
	printf("Usage: io_module_demo option\n"
"pty               start a pty demo\n"
);
}

void clean(void){
	return;
}

int ptyMasterOpen(char *slaveName, size_t snLen){
	int masterFd, savedErrno;
	char *p;

	masterFd = posix_openpt(O_RDWR|O_NOCTTY);
	if(masterFd == -1){
		printf("posix_openpt failed.\n");
		return -1;
	}

	if(grantpt(masterFd) == -1){
		savedErrno = errno;
		close(masterFd);
		errno = savedErrno;
		return -1;
	}

	if(unlockpt(masterFd) == -1){
		savedErrno = errno;
		close(masterFd);
		errno = savedErrno;
		return -1;
	}

	p = ptsname(masterFd);
	if(p == NULL){
		savedErrno = errno;
		close(masterFd);
		errno =savedErrno;
		return -1;
	}

	printf("slave pty: %s\n", p);
	if(strlen(p) < snLen){
		strncpy(slaveName, p, snLen);
	}else{
		close(masterFd);
		errno = EOVERFLOW;
		return -1;
	}

	return masterFd;
}

pid_t ptyFork(int *masterFd, char *slaveName, size_t snLen,
              const struct termios *slaveTermios, const struct winsize *slaveWS){
	int mfd, slaveFd, savedErrno;
	pid_t childPid;
	char slname[128];

	mfd = ptyMasterOpen(slname, sizeof(slname));
	if(mfd == -1){
		return -1;
	}

	if(slaveName != NULL){
		if(strlen(slname) < snLen){
			strncpy(slaveName, slname, snLen);
		}else{
			close(mfd);
			errno = EOVERFLOW;
			return -1;
		}
	}

	childPid = fork();

	if(childPid == -1){
		savedErrno = errno;
		close(mfd);
		errno = savedErrno;
		return -1;
	}

	if(childPid != 0){
		*masterFd = mfd;
		return childPid;
	}

	if(setsid() == -1){
		return -1;
	}

	close(mfd);

	slaveFd = open(slname, O_RDWR);
	if(slaveFd == -1){
		return -1;
	}

#ifdef TIOCSCTTY
	if(ioctl(slaveFd, TIOCSCTTY, 0) == -1){
		return -1;
	}
#endif

	if(slaveTermios != NULL){
		if(tcsetattr(slaveFd, TCSANOW, slaveTermios) == -1){
			return -1;
		}
	}

	if(slaveWS != NULL){
		if(ioctl(slaveFd, TIOCSWINSZ, slaveWS) == -1){
			return -1;
		}
	}

	if(dup2(slaveFd, 0) != 0){
		return -1;
	}

	if(dup2(slaveFd, 1) != 1){
		return -1;
	}

	if(dup2(slaveFd, 2) != 2){
		return -1;
	}

	if(slaveFd > 2){
		close(slaveFd);
	}

	return 0;
}

struct termios ttyOrig;
static void ttyReset(void){
	tcsetattr(0, TCSANOW, &ttyOrig);
}

int pty_demo(){
	char slaveName[128];
	char *shell;
	int masterFd;
	struct winsize ws;
	fd_set inFfs;
	char buf[128];
	size_t numRead;
	pid_t childPid;

	if(tcgetattr(0, &ttyOrig) == -1){
		return -1;
	}
	if(ioctl(0, TIOCGWINSZ, &ws) == -1){
		return -1;
	}

	childPid = ptyFork(&masterFd, slaveName, sizeof(slaveName), &ttyOrig, &ws);
	if(childPid ==-1){
		return -1;
	}

	if(childPid == 0){
		shell = getenv("SHELL");
		if(shell == NULL || *shell == '\0'){
			shell = "/bin/bash";
		}

		execlp(shell, shell, (char *)NULL);
		return -1;
	}

	if(atexit(ttyReset) != 0){
		return -1;
	}

	while(1){
		FD_ZERO(&inFfs);
		FD_SET(0, &inFfs);
		FD_SET(masterFd, &inFfs);

		if(select(masterFd + 1, &inFfs, NULL, NULL, NULL) == -1){
			return -1;
		}

		if(FD_ISSET(0, &inFfs)){
			numRead = read(0, buf, sizeof(buf));
			if(numRead <= 0){
				return -1;
			}

			printf("stdin-->pty:");
			fflush(stdout);
			if(write(masterFd, buf, numRead) != numRead){
				return -1;
			}
		}

		if(FD_ISSET(masterFd, &inFfs)){
			numRead = read(masterFd, buf, sizeof(buf));
			if(numRead <= 0){
				return -1;
			}

			printf("pty-->stdin:");
			fflush(stdout);
			if(write(1, buf, numRead) != numRead){
				return -1;
			}
		}
	}
}

int main(int argc, char *argv[]){
	atexit(clean);

	if(argc < 2){
		usage();
		return -1;
	}

	if(!strcmp("pty", argv[1])){
		return pty_demo();
	}
	else{
		usage();
	}

	return 0;
}
