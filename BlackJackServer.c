#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>

#include "BlackJackHeader.h"

#define BUFFER_SIZE 1

int main(int argc, char* argv[]) {
	int _socket, n, port;
	
	struct sockaddr_in server, client;
	
	char buffer[BUFFER_SIZE];
	
	port = atoi(argv[1]);
	if (port == 0) {
		printf("\"%s\" should be a port number!\n", argv[1]);
		return 1;
	}
	
	_socket = socket(AF_INET, SOCK_STREAM, 0);
	
	bzero(&server, sizeof(server));
	
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(port);
	
	bind(socket, (struct sockaddr*) &server, sizeof(server));
	
	////////////// BlackJack /////////////////
	
	srand(time(NULL));
	
	int server_sum = 0, client_sum = 0;
	unsigned short cards_used = 0;  // each bit represents whether or not the card has been used
	
	while (client_sum <= 21 && server_sum <= 21) {
		// wait for the client's response
		int length = sizeof(client);
		recvfrom(socket, buffer, BUFFER_SIZE, 0, (struct sockaddr*) &client, &length);
		
		// when the client responds, update the client_sum
		unsigned char card;
		client_sum += card = getCardValue(buffer[0]);
		if (client_sum > 21)
			break;
		
		// mark the card as chosen
		cards_used |= 1 << (16 - card);
		
		// pick an unused random card
		do
			card = (unsigned char) (rand() % 13 + 1);
		while ((cards_used >> card) & 1 == 1);
		
		// mark the card as chosen
		cards_used |= 1 << (16 - card);
		
		// update the server_sum
		server_sum += card;
		
		// send the server the chosen card
		buffer[0] = getCardSymbol(card);
		sendto(socket, buffer, BUFFER_SIZE, 0, (struct sockaddr*) &server, sizeof(server));
	}
	
	////////////// END BlackJack ///////////////
	
	close(socket);
}
