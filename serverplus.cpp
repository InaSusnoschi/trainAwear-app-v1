#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

using namespace std;

void error(char *msg){

	perror(msg);
	exit(EXIT_FAILURE);
}

int main() {
	int  sockfd;
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	struct sockaddr_in serv, client;
	serv.sin_family = AF_INET;
	serv.sin_port = htons(31415);
	serv.sin_addr.s_addr = INADDR_ANY;
	char buffer[256];
	socklen_t l = sizeof(client);

	cout<<"\n binding \n";
	bind(sockfd,(struct sockaddr *)&serv, sizeof(serv));

	cout<<" \n going to receive \n";
	int rc = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client, &l);
	if (rc<0){
		cout<<"ERROR READING FROM SOCKET";
	}

	cout<<"\n the message is: "<<buffer<<endl;
	int rp = sendto(sockfd, "hi", 2, 0, (struct sockaddr *)&client, l);
	if(rp<0){
		cout<<"ERROR WRITING TO SOCKET"<<endl;
	}
}


