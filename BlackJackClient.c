#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

#include "BlackJackHeader.h"

#define BUFFER_SIZE 1

int main(int argc, char* argv[]) {
	int _socket, n, port;
	socklen_t client_length;
	char buffer[BUFFER_SIZE];
	
	struct sockaddr_in server, client;
	
	// check the number of command line arguments
	if (argc < 3) {
		printf("Please enter a port number!\n");
		return 1;
	} else if (argc < 2) {
		printf("Please give an I.P. and port number to conenct to!\n");
		return 1;
	}
	
	// open the socket
	_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket < 0) {
		printf("I couldn't open a new socket!\n");
		return 2;
	}
	
	bzero(&server, sizeof(server));
	
	port = atoi(argv[2]);
	if (port == 0) {
		printf("\"%s\" should be a port number!\n", argv[2]);
		return 1;
	}
	
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(argv[1]);
	server.sin_port = htons(port);
			
	////////////// BlackJack /////////////////
	
	srand(time(NULL));
	
	int server_sum = 0, client_sum = 0;
	unsigned short cards_used = 0;  // each bit represents whether or not the card has been used
	
	while (client_sum <= 21 && server_sum <= 21) {
		// pick an unused random card
		unsigned char card;
		do
			card = (unsigned char) (rand() % 13 + 1);
		while ((cards_used >> card) & 1 == 1);
		
		// post the client's picked card
		printf("You got a %c!\n", getCardSymbol(card));
		
		// mark the card as chosen
		cards_used |= 1 << (16 - card);
		
		// update the client_sum
		client_sum += card;
		if (client_sum > 21)
			break;
		
		// send the server the chosen card
		buffer[0] = getCardSymbol(card);
		sendto(socket, buffer, 1, 0, (struct sockaddr*) &server, sizeof(server));
		
		// wait for the server's response
		int length = sizeof(server);
		recvfrom(socket, buffer, BUFFER_SIZE, 0, (struct sockaddr*) &server, &length);
		
		// post the server's picked card
		printf("The dealer got a %c!\n", buffer[0]);
		
		// when the server responds, update the server_sum
		server_sum += card = getCardValue(buffer[0]);
		
		// mark the card as chosen
		cards_used |= 1 << (16 - card);
	}
	
	// when the game ends, print out a message giving the result
	if (server_sum > 21)
		printf("The dealer busted with %d!\nYOU WIN!\n", server_sum);
	else
		printf("You busted with %d!\nYou lose. :(\n", client_sum);
	
	////////////// END BlackJack ///////////////
	
	close(socket);
}
