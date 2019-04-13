/*
 * Inspired from: https://gist.github.com/miekg/a61d55a8ec6560ad6c4a2747b21e6128
 * udpserver.c - A simple UDP echo server
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>

#define BUFSIZE 1024
#define PORTNO 6789
#define DELAY 100000 /* sleep 100ms between signals */

static char *morse_char(char c) {
  switch (toupper(c)) {
    case ' ': return " ";
    case 'A': return ".- ";
    case 'B': return "-... ";
    case 'C': return "-.-. ";
    case 'D': return "-.. ";
    case 'E': return ". ";
    case 'F': return "..-. ";
    case 'G': return "--. ";
    case 'H': return ".... ";
    case 'I': return ".. ";
    case 'J': return ".--- ";
    case 'K': return "-.- ";
    case 'L': return ".-.. ";
    case 'M': return "-- ";
    case 'N': return "-. ";
    case 'O': return "--- ";
    case 'P': return ".--. ";
    case 'Q': return "--.- ";
    case 'R': return ".-. ";
    case 'S': return "... ";
    case 'T': return "- ";
    case 'U': return "..- ";
    case 'V': return "...- ";
    case 'W': return ".-- ";
    case 'X': return "-..- ";
    case 'Y': return "--.- ";
    case 'Z': return "--.. ";
    case '0': return "----- ";
    case '1': return ".---- ";
    case '2': return "..--- ";
    case '3': return "...-- ";
    case '4': return "....- ";
    case '5': return "..... ";
    case '6': return "-.... ";
    case '7': return "--... ";
    case '8': return "---.. ";
    case '9': return "----. ";
    case '/': return "-..-. ";
    case '=': return "-...- ";
    case '-': return "-....- ";
    case '.': return ".-.-.- ";
    case '+': return ".-.-. ";
    case '*': return "*"; /* reset queue */
    default: return NULL;
  }
}

/*
 * error - wrapper for perror
 */
void error(char *msg)
{
	perror(msg);
	exit(1);
}

int main(int argc, char **argv)
{
	int sockfd;		/* socket */
	int ttyfd;
	struct sockaddr_in serveraddr;	/* server's addr */
	char buf[BUFSIZE];	/* message buf */
	int optval;		/* flag value for setsockopt */
	int n;			/* message byte size */

	if (argc != 2) {
		printf("Syntax: cwdaemon /dev/ttyACM0\n");
		exit(1);
	}

	if (!(ttyfd = open(argv[1], O_WRONLY)))
		error("Could not open tty");

	/*
	 * socket: create the parent socket
	 */
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0)
		error("ERROR opening socket");

	/* setsockopt: Handy debugging trick that lets
	 * us rerun the server immediately after we kill it;
	 * otherwise we have to wait about 20 secs.
	 * Eliminates "ERROR on binding: Address already in use" error.
	 */
	optval = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR,
		   (const void *)&optval, sizeof(int));

	/*
	 * build the server's Internet address
	 */
	bzero((char *)&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons((unsigned short)PORTNO);

	/*
	 * bind: associate the parent socket with a port
	 */
	if (bind(sockfd, (struct sockaddr *)&serveraddr,
		 sizeof(serveraddr)) < 0)
		error("ERROR on binding");

	/*
	 * main loop: wait for a datagram, then send it to keyer
	 */
	while (1) {
		n = recv(sockfd, buf, BUFSIZE, 0);
		if (n < 0)
			error("ERROR in recvfrom");

		if (*buf == '\033') {
			write(1, "ESC ", sizeof("ESC "));
			write(1, buf + 1, n - 1);
			write(1, "\n", sizeof("\n"));
			continue;
		}

		for (int i = 0; i < n; i++) {
			write(1, buf + i, 1);
			char *code = morse_char(buf[i]);
			if (code) {
				for (int j = 0; code[j]; j++) {
					//write(1, code + j, 1);
					if (write(ttyfd, code + j, 1) != 1)
						error("write");
					usleep(DELAY);
				}
			} else
				write(1, "?", 1);
		}
		write(1, "\n", 1);
	}
}
