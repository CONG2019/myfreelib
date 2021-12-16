#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <mqueue.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <signal.h>
#include <pthread.h>
#include "posix_mq_demo.h"

static void threadFunc(union sigval sv);

static void usage(void){
	printf("Usage: posix_mq_demo option\n"
"create               run a demo to create posix mq\n"
"getattr              get posix mq attr\n"
"unlink               delete posix mq\n"
"send                 send a message\n"
"receive              receive a message\n"
"sig_notify           through signal notify\n"
"thr_notify           through threat notify\n");
}

void clean(void){
	return;
}

int create_demo(){
	int flags;
	mode_t mode;
	mqd_t mqd;
	struct mq_attr attr, *attrp;

	attrp = &attr;
	attr.mq_maxmsg = MAX_MSG;
	attr.mq_msgsize = MSG_SIZE;
	flags = O_RDWR|O_CREAT|O_EXCL;

	mode = S_IRUSR|S_IWUSR;

	mqd = mq_open(POSIX_MQ_PATH, flags, mode, attrp);

	if(mqd == (mqd_t) -1){
		printf("create posix mq fail.\n");
		return -1;
	}
	mq_close(mqd);
	return 0;
}

int getattr_demo(){
	int flags;
	mode_t mode;
	mqd_t mqd;
	struct mq_attr attr, *attrp;

	attrp = &attr;
	flags = O_RDWR|O_CREAT|O_EXCL;

	mode = S_IRUSR | S_IWUSR;

	mqd = mq_open("/test", flags, mode, NULL);

	if(mqd == (mqd_t) -1){
		printf("create posix mq fail.\n");
		return -1;
	}

	mq_getattr(mqd, attrp);
	printf("default mq_maxmsg = %ld\ndefault mq_msgsize = %ld\n", attrp->mq_maxmsg, attrp->mq_msgsize);
	mq_close(mqd);
	mqd = (mqd_t) -1;
	mq_unlink("/test");

	flags = O_RDWR;
	mqd = mq_open(POSIX_MQ_PATH, flags, 0, NULL);
	if(mqd == (mqd_t) -1){
		printf("create posix mq fail.\n");
		return -1;
	}
	memset(attrp, 0, sizeof(struct mq_attr));
	mq_getattr(mqd, attrp);
	mq_close(mqd);
	printf("mq_maxmsg = %ld\nmq_msgsize = %ld\n", attrp->mq_maxmsg, attrp->mq_msgsize);
	return 0;
}

int unlink_demo(){
	mq_unlink(POSIX_MQ_PATH);
	return 0;
}

int send_demo(){
	int flags;
	mqd_t mqd;
	msg_t msg;
#ifdef USE_TIMED
	struct timespec time_out;
	time_out.tv_sec = 5;
#endif

	flags = O_RDWR;

	mqd = mq_open(POSIX_MQ_PATH, flags);
	if(mqd == (mqd_t) -1){
		printf("create posix mq fail.\n");
		return -1;
	}

	msg.type = 1;
	snprintf(msg.data, sizeof(msg.data), "Hello world.\n");

#ifdef USE_TIMED
	if(mq_timedsend(mqd, (char *)&msg, sizeof(msg), 1, &time_out) == -1){
		printf("mq_send fail.\n");
		return -1;
	}
#else
	if(mq_send(mqd, (char *)&msg, sizeof(msg), 1) == -1){
		printf("mq_send fail.\n");
		return -1;
	}
#endif

	mq_close(mqd);
	return 0;
}

int receive_demo(){
	int flags;
	mqd_t mqd;
	msg_t *msg;
	unsigned int prio;
#ifdef USE_TIMED
	struct timespec time_out;
	time_out.tv_sec = (time(NULL) + 10);
	time_out.tv_nsec = 0;
#endif

	msg = (msg_t *)malloc(MSG_SIZE);
	if(msg == NULL){
		printf("malloc fail.\n");
	}
	flags = O_RDONLY;
	prio = 1;

	mqd = mq_open(POSIX_MQ_PATH, flags);
	if(mqd == (mqd_t) -1){
		printf("create posix mq fail.\n");
		return -1;
	}

#ifdef USE_TIMED
	if(mq_timedreceive(mqd, (char *)msg, MSG_SIZE, &prio, &time_out) == -1){
		printf("%s fail, errno=%d.\n", __func__, errno);
		return -1;
	}
#else
	if(mq_receive(mqd, (char *)msg, MSG_SIZE, &prio) == -1){
		printf("%s fail, errno=%d.\n", __func__, errno);
		return -1;
	}
#endif

	printf("type=%d, data=%s\n", msg->type, msg->data);

	mq_close(mqd);
	return 0;
}

static void handler(int sig){
	/* just interrupt sigsuspend */
}

int sig_notify_demo(){
	struct sigevent sev;
	mqd_t mqd;
	char buffer[MSG_SIZE];
	ssize_t numRead;
	sigset_t blockMask, emptyMask;

	struct sigaction sa;

	mqd = mq_open(POSIX_MQ_PATH, O_RDONLY|O_NONBLOCK);
	if(mqd == (mqd_t) -1){
		printf("mq_open fail.\n");
		return -1;
	}

	sigemptyset(&blockMask);
	sigaddset(&blockMask, NOTIFY_SIG);
	if(sigprocmask(SIG_BLOCK, &blockMask, NULL) == -1){
		printf("sigprocmask failed.\n");
		return -1;
	}

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = handler;
	if(sigaction(NOTIFY_SIG, &sa, NULL) == -1){
		printf("sigaction failed.\n");
		return -1;
	}

	sev.sigev_notify = SIGEV_SIGNAL;
	sev.sigev_signo = NOTIFY_SIG;
	if(mq_notify(mqd, &sev) == -1){
		printf("mq_notify failed.\n");
		return -1;
	}

	sigemptyset(&emptyMask);

	while(1){
		sigsuspend(&emptyMask);

		if(mq_notify(mqd, &sev) == -1){
			printf("mq_notify failed.\n");
			return -1;
		}
		while((numRead = mq_receive(mqd, buffer, MSG_SIZE, NULL)) >= 0){
			printf("read %ld bytes.\n", numRead);
		}
	}

	return 0;
}

static void threadFunc(union sigval sv){
	ssize_t numRead;
	mqd_t *mqdp;
	char buffer[MSG_SIZE];
	struct sigevent sev;

	mqdp = sv.sival_ptr;

	sev.sigev_notify = SIGEV_THREAD;
	sev.sigev_notify_function = threadFunc;
	sev.sigev_notify_attributes = NULL;
	sev.sigev_value.sival_ptr = mqdp;
	if(mq_notify(*mqdp, &sev) == -1){
		printf("mq_notify failed.\n");
		return ;
	}

	while((numRead = mq_receive(*mqdp, buffer, MAX_MSG, NULL)) >= 0){
		printf("read %ld bytes.\n", numRead);
	}

	pthread_exit(NULL);
}

int thr_notify_demo(){
	mqd_t mqd;
	struct sigevent sev = {0};

	mqd = mq_open(POSIX_MQ_PATH, O_RDWR);
	if(mqd == (mqd_t) -1){
		printf("mq_open failed.\n");
		return -1;
	}

	sev.sigev_notify = SIGEV_THREAD;
	sev.sigev_notify_function = threadFunc;
	sev.sigev_notify_attributes = NULL;
	sev.sigev_value.sival_ptr = &mqd;
	if(mq_notify(mqd, &sev) == -1){
		printf("mq_notify failed.\n");
		return -1;
	}
	while(1){
		printf("sleep...\n");
		sleep(1);
	}
	return 0;
}

int main(int argc, char *argv[]){
	atexit(clean);

	if(argc < 2){
		usage();
		return -1;
	}

	if(!strcmp("create", argv[1])){
		return create_demo();
	}else if(!strcmp("getattr", argv[1])){
		return getattr_demo();
	}else if(!strcmp("unlink", argv[1])){
		return unlink_demo();
	}else if(!strcmp("send", argv[1])){
		return send_demo();
	}else if(!strcmp("receive", argv[1])){
		return receive_demo();
	}else if(!strcmp("sig_notify", argv[1])){
		return sig_notify_demo();
	}else if(!strcmp("thr_notify", argv[1])){
		return thr_notify_demo();
	}
	else{
		usage();
	}

	return 0;
}
