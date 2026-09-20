/*
 *	Talker program:
 *	sends UDP packet with a sequence ID
 */
#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>
#include<netdb.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/types.h>

#define BUFF_SIZE 500

struct payload{
	uint32_t seq_id;
};

int main(int argc, char *argv[]){

	int 		sfd, s;
	char		buf[BUFF_SIZE];
	size_t		size;
	ssize_t 	nread;
	struct addrinfo	hints;
	struct addrinfo *results, *rp;

	if(argc < 3){
		fprintf(stderr, "Missing host and port");
	        exit(EXIT_FAILURE);	
	}

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC; 		/* Allow IPv4 or IPv6	*/
	hints.ai_socktype = SOCK_DGRAM;		/* Datagram socket	*/
	hints.ai_flags = 0;
	hints.ai_protocol = 0;			/* Any protocol		*/

	s = getaddrinfo(argv[1], argv[2], &hints, &results);
	if(s != 0){
		fprintf(stderr, "Failed getaddrinfo");
		exit(EXIT_FAILURE);
	}

	for(rp = results; rp != NULL; rp = rp->ai_next){
		sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		
		if(sfd == -1) continue;

		if(connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1) break;

		close(sfd);
	}

	freeaddrinfo(results);

	if(rp == NULL){
		fprintf(stderr, "Could not connect");
		exit(EXIT_FAILURE);
	}

	uint32_t seq_id = 1;
	struct payload packet;

	for(;;){
		packet.seq_id = htonl(seq_id);
		printf("Sending packet with id: %d \n", seq_id);	
		ssize_t n = write(sfd, &packet, sizeof(packet));
		if(n < 0){
			perror("Failed to write");
			exit(EXIT_FAILURE);
		}

		seq_id++;
		sleep(2);	
	}


	exit(EXIT_SUCCESS);
}
