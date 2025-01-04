#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>

#define PORT "8080"
#define BACKLOG 2
#define BUFLEN 1000

// Server boots up on ":8080" and accepts first connection,
// accepts message from client, and prints it. Then sends ack to
// client: "Hello from server!"
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

	// Socket for listening
	int sfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (sfd == -1) {
		fprintf(stderr, "socket: %d\n", errno);
		return 2;
	}

	// Bind socket on `PORT`
	status = bind(sfd, res->ai_addr, res->ai_addrlen);
	if (status == -1) {
		fprintf(stderr, "bind: %d\n", errno);
		return 3;
	}

	// Listen
	printf("Listening on :8080\n");
	status = listen(sfd, BACKLOG);
	if (status == -1) {
		fprintf(stderr, "listen: %d\n", errno);
		return 4;
	}

	// Accept
	struct sockaddr_storage caddr;
	socklen_t caddr_size;
	int nfd = accept(sfd, (struct sockaddr *)&caddr, &caddr_size);
	if (nfd == -1) {
		fprintf(stderr, "accept: %d\n", errno);
		return 5;
	}

	// Close listening connection
	//
	// Don't let other attempts through.
	status = close(sfd);
	if (status == -1) {
		fprintf(stderr, "close: %d\n", errno);
		return 6;
	}

	// Recv + print message from client
	char buf[BUFLEN];
	status = recv(nfd, buf, BUFLEN, 0);
	if (status == 0) {
		fprintf(stderr, "Client connection closed.\n");
		return 7;
	} else if (status == -1) {
		fprintf(stderr, "recv: %d\n", errno);
		return 8;
	}
	printf("Message from client: %s\n", buf);

	// Send ack back to client
	char *msg = "Hello from server!";
	int len = strlen(msg);
	int bs = send(nfd, msg, len, 0);
	if (bs == -1) {
		fprintf(stderr, "send: %d\n", errno);
		return 9;
	}
	printf("Sent ack to client.\nClosing...\n");

	// Die
	status = close(nfd);
	if (status == -1) {
		fprintf(stderr, "close: %d\n", errno);
		return 10;
	}
	freeaddrinfo(al);
	return 0;
}
