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
#include "inet_socket_demo.h"

static void usage(void){
	printf("Usage: unix_socket_demo option\n"
"str_server port               start a stream socket server\n"
"str_client                    start a stream socket client\n"
"data_server                   start a data socket server\n"
"data_client                   start a data socket client\n"
"addrinfo                      getaddrinfo demo\n"
"nameinfo                      getnameinfo demo\n"
);
}

void clean(void){
	return;
}

int data_server_demo(int argc, char *argv[]){
	struct addrinfo hints;
	struct addrinfo *result, *rp;
	int sfd, s;
	struct sockaddr_storage peer_addr;
	socklen_t peer_addr_len;
	ssize_t nread;
	char buf[BUF_SIZE];

	if (argc != 3) {
		fprintf(stderr, "Usage: %s str_server port\n", argv[0]);
		return -1;
	}

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
	hints.ai_socktype = SOCK_DGRAM; /* Datagram socket */
	hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
	hints.ai_protocol = 0;          /* Any protocol */
	hints.ai_canonname = NULL;
	hints.ai_addr = NULL;
	hints.ai_next = NULL;

	s = getaddrinfo(NULL, argv[2], &hints, &result);
	if (s != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
		return -1;
	}

	/* getaddrinfo() returns a list of address structures.
		Try each address until we successfully bind(2).
		If socket(2) (or bind(2)) fails, we (close the socket
		and) try the next address. */

	for (rp = result; rp != NULL; rp = rp->ai_next) {
		sfd = socket(rp->ai_family, rp->ai_socktype,
				rp->ai_protocol);
		if (sfd == -1)
			continue;

		if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0)
			break;                  /* Success */

		close(sfd);
	}

	freeaddrinfo(result);           /* No longer needed */

	if (rp == NULL) {               /* No address succeeded */
		fprintf(stderr, "Could not bind\n");
		return -1;
	}

	/* Read datagrams and echo them back to sender. */

	for (;;) {
		peer_addr_len = sizeof(peer_addr);
		nread = recvfrom(sfd, buf, BUF_SIZE, 0,
				(struct sockaddr *) &peer_addr, &peer_addr_len);
		if (nread == -1)
			continue;               /* Ignore failed request */

		char host[NI_MAXHOST], service[NI_MAXSERV];

		s = getnameinfo((struct sockaddr *) &peer_addr,
						peer_addr_len, host, NI_MAXHOST,
						service, NI_MAXSERV, NI_NUMERICSERV);
		if (s == 0)
			printf("Received %zd bytes from %s:%s\n",
					nread, host, service);
		else
			fprintf(stderr, "getnameinfo: %s\n", gai_strerror(s));

		if (sendto(sfd, buf, nread, 0,
					(struct sockaddr *) &peer_addr,
					peer_addr_len) != nread)
			fprintf(stderr, "Error sending response\n");
	}

	return 0;
}

int data_client_demo(int argc, char *argv[]){
	struct addrinfo hints;
	struct addrinfo *result, *rp;
	int sfd, s;
	size_t len;
	ssize_t nread;
	char buf[BUF_SIZE];

	if (argc < 4) {
		fprintf(stderr, "Usage: %s host port msg...\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	/* Obtain address(es) matching host/port. */

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
	hints.ai_socktype = SOCK_DGRAM; /* Datagram socket */
	hints.ai_flags = 0;
	hints.ai_protocol = 0;          /* Any protocol */

	s = getaddrinfo(argv[2], argv[3], &hints, &result);
	if (s != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
		return -1;
	}

	/* getaddrinfo() returns a list of address structures.
		Try each address until we successfully connect(2).
		If socket(2) (or connect(2)) fails, we (close the socket
		and) try the next address. */

	for (rp = result; rp != NULL; rp = rp->ai_next) {
		sfd = socket(rp->ai_family, rp->ai_socktype,
					rp->ai_protocol);
		if (sfd == -1)
			continue;

		if (connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1)
			break;                  /* Success */

		close(sfd);
	}

	freeaddrinfo(result);           /* No longer needed */

	if (rp == NULL) {               /* No address succeeded */
		fprintf(stderr, "Could not connect\n");
		exit(EXIT_FAILURE);
	}

	/* Send remaining command-line arguments as separate
		datagrams, and read responses from server. */

	for (int j = 4; j < argc; j++) {
		len = strlen(argv[j]) + 1;
				/* +1 for terminating null byte */

		if (len > BUF_SIZE) {
			fprintf(stderr,
					"Ignoring long message in argument %d\n", j);
			continue;
		}

		if (write(sfd, argv[j], len) != len) {
			fprintf(stderr, "partial/failed write\n");
			return -1;
		}

		nread = read(sfd, buf, BUF_SIZE);
		if (nread == -1) {
			perror("read");
			return -1;
		}

		printf("Received %zd bytes: %s\n", nread, buf);
	}

	return 0;

}

/***
struct addrinfo {
	int              ai_flags;
	int              ai_family;
	int              ai_socktype;
	int              ai_protocol;
	socklen_t        ai_addrlen;
	struct sockaddr *ai_addr;
	char            *ai_canonname;
	struct addrinfo *ai_next;
};
****/
int addrinfo_demo(int argc, char *argv[]){
	struct addrinfo *res, *tmp;
	struct sockaddr_in *saddr;
	char buf[INET_ADDRSTRLEN];

	if(argc < 3){
		usage();
		return -1;
	}

	if(getaddrinfo(argv[2], NULL, NULL, &res) != 0){
		printf("getaddrinfo failed.\n");
		return -1;
	}

	tmp = res;
	while(tmp != NULL){
		if(tmp->ai_family == AF_INET){
			saddr = (struct sockaddr_in *)tmp->ai_addr;
			printf("%s\n", inet_ntop(AF_INET, (void *)&saddr->sin_addr.s_addr, buf, sizeof(buf)));
		}else{
			printf("not AF_INET\n");
		}
		tmp = tmp->ai_next;
	}

	freeaddrinfo(res);
	return 0;
}

int nameinfo_demo(int argc, char *argv[]){
	struct sockaddr_in saddr;
	socklen_t addr_len;
	char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];
	addr_len = sizeof(saddr);

	if(argc < 3){
		usage();
		return -1;
	}

	saddr.sin_family = AF_INET;
	inet_pton(AF_INET, argv[2], (void *)&saddr.sin_addr.s_addr);
	if(getnameinfo((struct sockaddr *)&saddr, addr_len, hbuf, (socklen_t)sizeof(hbuf), sbuf, (socklen_t)sizeof(sbuf), NI_NUMERICHOST|NI_NUMERICHOST) == 0){
		printf("hosts=%s, server=%s\n", hbuf, sbuf);
	}

	return 0;

}

int main(int argc, char *argv[]){
	atexit(clean);

	if(argc < 2){
		usage();
		return -1;
	}

	if(!strcmp("data_server", argv[1])){
		return data_server_demo(argc, argv);
	}else if(!strcmp("data_client", argv[1])){
		return data_client_demo(argc, argv);
	}else if(!strcmp("addrinfo", argv[1])){
		return addrinfo_demo(argc, argv);
	}else if(!strcmp("nameinfo", argv[1])){
		return nameinfo_demo(argc, argv);
	}
	else{
		usage();
	}

	return 0;
}
