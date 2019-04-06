#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define BUFFER_LENGTH 1500
#define PORT 31415

int main(void){

	int			fd, n;  //  n is for random number gen
	ssize_t			rlen;
	time_t			t;
	struct sockaddr_in	addr, conn_addr;
	socklen_t		conn_addr_len;
	int			written;
	char			buffer[BUFFER_LENGTH]; //randomnumber[written];
	int			flags = 0;

	// initialise random number generator
	srand((unsigned int)time(NULL));

	// needed for UDP connection
	struct sockaddr_storage	client_addr;
	socklen_t		client_addr_len;

	// create UDP sock_dgram over IP socket
	fd = socket (AF_INET, SOCK_DGRAM, 0);
	if (fd == -1) {
		perror("unable to create socket");
		return 1;
	}

	// attempt to bind to port
	addr.sin_family		=AF_INET;
	addr.sin_port		=htons(PORT);
	addr.sin_addr.s_addr	=INADDR_ANY;

	if(bind(fd, (struct sockaddr *) &addr, sizeof(addr)) == -1){
		perror("Unable to bind to port");
		return 2;
	}

	// declare delay function
	void delay(unsigned int mseconds){
		clock_t goal = mseconds+clock();
		while(goal>clock());
	}


	// retrieve data in packet
	client_addr_len = sizeof(struct sockadddr_storage *);
	while((rlen = recvfrom(fd, buffer, BUFFER_LENGTH, flags, (struct sockaddr *) &client_addr, &client_addr_len)) > 0){
		int i;
		for (i=0; i<rlen; i++){
			printf("%c", buffer[i]);
		}
//		sendto(fd, "ACK %c", BUFFER_LENGTH, flags, (struct sockaddr *)&client_addr, sizeof(client_addr));
		int r = rand() %50;
		//int length = snprintf(NULL,0,"%d",r);
		//char* str = malloc(length+1);
		//snprintf(str, length+1, "%d", r)
		int send = 1;	
		while (send<10){
			sprintf(buffer, "%d", r);
			sendto(fd, buffer, strlen(buffer), flags, (struct sockaddr *)&client_addr, sizeof(client_addr));
			send++;	
		}

		//written = sprintf(randomnumber, "%f \n",r);
//		sendto(fd, randomnumber, written,  flags, (struct sockaddr *)&client_addr, sizeof(client_addr));
		printf(" That's all folks! %s\n", buffer);
	}


		// close socket
	close(fd);

	return 0;
}
