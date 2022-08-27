#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>

char *recvpkt();

#define GROUP	"239.2.6.97"
#define PORT	56789
#define TTL	15
#define PROMPT  "-> "
#define READ	0
#define WRITE	1

int debug;
int sdr, sdw;			/* descripteurs de socket */
struct sockaddr_in sr, sw;    	/* sockets */
struct ip_mreq imr;

 
main(int argc, char ** argv)
{
	int sel;
	fd_set  mask_R;
	char *pbuf;
	char user[10], host[256], line[BUFSIZ], mess[BUFSIZ];

	if (argc > 1 && !strncmp(argv[1], "-d", 2))
		debug = 1;
	/* Prendre le nom de l'utilisateur */
	user = cuserid( (char *)NULL);
	/* Prendre le nom de la machine locale */
	gethostname(host, sizeof(host));

	sdr = OpenSocketMulticast(GROUP, PORT, READ);
	sdw = OpenSocketMulticast(GROUP, PORT, WRITE);

	sprintf(mess, "[%s@%s joins]\n", user, host);
	sendpkt(sdw, mess);

	while (1) {
		/* printf("%s", PROMPT); fflush(stdout); */
		FD_ZERO(&mask_R);
		FD_SET(0, &mask_R);      
		FD_SET(sdr, &mask_R);
		sel = select(32, &mask_R, 0, 0, 0);
		switch (sel) {
		case -1:
			perror("select");
			RAZ_Sock(sdr);
		case 0:
			printf("timeout: Abandon\n");
			RAZ_Sock(sdr);
		} 
		/*  verifier si cela vient du clavier */	
		if (FD_ISSET(0, &mask_R)) {
			if ((fgets(line, sizeof(line), stdin)) == NULL) {
				sprintf(mess, "[%s@%s quits]\n", user, host);
				sendpkt(..., mess);
				drop(...);
				exit(0);
			}
			if (*line == '\n')
				continue;
			if (*line == '/') {
				switch (line[1]) {
				case 'q':
					sprintf(mess, "[%s@%s quits]\n", user, host);
					sendpkt(..., mess);
					drop(...);
					exit(0);
					break;
				case 'h':
				case '?':
					printf("usage: /q quit, /h help\n");
					continue;
				}
				fprintf(stderr, "unknown command\n");
				continue;
			}
			
			sprintf(mess, "[%s@%s] %s", user, host, line);
			if (debug) {
				fprintf(stderr, "KBD: %s", mess);
				fprintf(stderr, "KBD: %d", strlen(mess));
			}
			sendpkt(..., mess);
		}
		/*  ou des sockets :  */	
		if (FD_ISSET(sdr, &mask_R)) {
			pbuf = recvpkt(sdr);
			if (pbuf == NULL)
				continue;
			if (debug) {
				fprintf(stderr, "NET: %s", pbuf);
				fprintf(stderr, "NET: %d", strlen(pbuf));
			}
			else
				printf("%s", pbuf);
		}
	}
} 

/********************** Fermer socket ****************************/
RAZ_Sock(int sock)
{
	if (close(sock))
		perror ("close");
	exit (1);
}

/********************* OpenSocketMulticast ***********************/
OpenSocketMulticast(char * group, unsigned short port, int sens)
{
	static int sock;
	int one = 1;

	if ((sock = socket(PF_INET, ..., 0)) == -1) {
        	perror( "socket");
		exit(1);
	}
	if (sens == READ) {
		/*
		 * Preparation de la socket de reception
		 */
		memset(&sr, 0, sizeof(sr));
		sr.sin_family = AF_INET; 
		sr.sin_port = ...;
		sr.sin_addr.s_addr = ...;
		/*
		 * reuse port
		 */
		setsockopt(...);
		/*
		 * bind (addr, port)
		 */
		if (bind(sock, (struct sockaddr *) &sr, sizeof(sr))) {
			if (errno == EADDRNOTAVAIL) {
				sr.sin_addr.s_addr = htonl(INADDR_ANY);
				if (bind(sock, (struct sockaddr *) &sr, sizeof(sr))) {
        				perror("bind");
					RAZ_Sock(sock);
				}
			}
		}
		/*
		 * join group
		 */
		join(sock, group);
	}
	else {
		/*
		 * Preparation de la socket d'emission
		 */
		memset(&sw, 0, sizeof(sw));
		sw.sin_family = AF_INET; 
		sw.sin_port = htons(port);
		sw.sin_addr.s_addr = htonl(INADDR_ANY); 
		/*
		 * initialisation de la portee
		 */
		scope(sock, TTL);
	}
	return(sock);
}

/*********************** Joindre groupe  ****************************/
join(int sock, char * group)
{
	imr.imr_multiaddr.s_addr = ...;
	imr.imr_interface.s_addr = htonl(INADDR_ANY);

	if (setsockopt(sock, ...) < 0) {
		perror("...");
		RAZ_Sock(sock);
	}
}

/*********************** Portee d'emission ****************************/
scope(int sock, int tt)
{
	unsigned char ttl;

	ttl = tt;
	if (setsockopt(sock, ...) < 0) {
		perror("...");
		RAZ_Sock(sock);
	}
}

/*********************** Quitter groupe  ****************************/
drop(int sock)
{
	if (setsockopt(sock, ...) < 0) {
		perror("...");
		RAZ_Sock(sock);
	}
	printf("quit!\n");
}

/*********************** loopback  ****************************/
loopback(int sock, char toggle)
{
	unsigned char loop;

	loop = (unsigned char) toggle;
	if (setsockopt(sock, ...) < 0) {
		perror("...");
	}
}

/******************* Recevoir un paquet *********************************/
char * recvpkt(int sock)
{	
	unsigned int lgpkt, lgsr;
	int cnt;
	static char pkt[BUFSIZ];

	lgpkt = sizeof(pkt);
	lgsr = sizeof(sr);
	memset(pkt, 0, sizeof(pkt));
	cnt = recvfrom(sock, pkt, lgpkt, 0, (struct sockaddr *) &sr, &lgsr);
	if (cnt < 0) {
		perror("recvfrom");
		RAZ_Sock(sock);
	}
	if (debug) fprintf(stderr, "recv cnt=%d\n", cnt);
	if (cnt == 0)
		return ((char *) NULL);
	return(pkt);
}

/******************* Envoyer paquet *********************************/
sendpkt(int sock, char * pkt)
{	
	unsigned int lgpkt, lgsw;
	int cnt;

	lgpkt = strlen(pkt);
	lgsw = sizeof(sw);
	cnt = sendto(sock, pkt, lgpkt, 0, (struct sockaddr *) &sw, lgsw);
	if (cnt < 0) {
		perror("sendto");
		RAZ_Sock(sock);
	}
	if (debug) fprintf(stderr, "send cnt=%d\n", cnt);
}
