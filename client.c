#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>

#define PORT "8080"
#define BUFLEN 1000

// Client connects to ":8080", then sends message to server:
// "Hello from client!". Then, gets ack from server, and prints it.
int main(void) {
	// Fetch `addrinfo` of ":8080"
	struct addrinfo hint, *al;
	memset(&hint, 0, sizeof(struct addrinfo));
	hint.ai_flags = AI_PASSIVE;
	hint.ai_family = AF_UNSPEC;
	hint.ai_socktype = SOCK_STREAM;

	int status = getaddrinfo(NULL, PORT, &hint, &al);
	if (status != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
		return 1;
	}

	struct addrinfo *res;
	for (res = al; res != NULL; res = res->ai_next) {
		// Get first IPv4 info
		if (res->ai_family == AF_INET) {
			break;
		}
	}

	// Socket to connect on
	int sfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (sfd == -1) {
		fprintf(stderr, "socket: %d\n", errno);
		return 2;
	}

	// Connect to server
	status = connect(sfd, res->ai_addr, res->ai_addrlen);
	if (status == -1) {
		fprintf(stderr, "connect: %d\n", errno);
		return 3;
	}

	// Send message to server
	char *msg = "Hello from client!";
	int len = strlen(msg);
	int bs = send(sfd, msg, len, 0);
	if (bs == -1) {
		fprintf(stderr, "send: %d\n", errno);
		return 4;
	}
	printf("Sent message to server.\n");

	// Recv + print message from server
	char buf[BUFLEN];
	bzero(buf, BUFLEN);
	status = recv(sfd, buf, BUFLEN, 0);
	if (status == 0) {
		fprintf(stderr, "Server connection closed.\n");
		return 5;
	} else if (status == -1) {
		fprintf(stderr, "recv: %d\n", errno);
		return 6;
	}
	printf("Message from server: %s\nClosing...\n", buf);

	// Die
	status = close(sfd);
	if (status == -1) {
		fprintf(stderr, "close: %d\n", errno);
		return 7;
	}
	freeaddrinfo(al);
	return 0;
}
