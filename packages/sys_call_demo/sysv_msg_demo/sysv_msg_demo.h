#ifndef __SYSV_MSG_DEMO_H__
#define __SYSV_MSG_DEMO_H__

#define MY_KEY 1456

struct mbuf{
	long mtype;
	char mtext[100];
};

#endif
