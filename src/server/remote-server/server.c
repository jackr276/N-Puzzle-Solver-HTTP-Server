/**
 * Author: Jack Robbins
 * This file contains the implementation of the functions outlined in server.h
 */  

#include "server.h"
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>

Server create_server(u_int32_t domain, u_int32_t port, u_int32_t service, u_int32_t protocol, u_int32_t backlog, u_int64_t interface){
	//Stack allocate our server(may change this)
	Server server;

	server.domain = domain; 
	server.port = port;
	server.service = service;
	server.backlog = backlog;
	server.interface = interface;

	server.socket_addr.sin_family = domain; 
	server.socket_addr.sin_port = htons(port);
	server.socket_addr.sin_addr.s_addr = htonl(interface);

	//Initialize the socket
	server.socket = socket(domain, service,  protocol);

	//If we couldn't initialize, stop everything
	if(server.socket < 0){
		printf("ERROR: Socket initializiation failed");
		exit(1);
	}

	//Now attempt to bind the socket to the address. If we can't, hard exit
	if(bind(server.socket, (struct sockaddr*)(&server.socket_addr), sizeof(server.socket_addr)) < 0){
		printf("ERROR: Socket failed to bind");
		exit(1);
	}
 
	//Finally attempt to begin listening. If that fails, hard exit
	if(listen(server.socket, server.backlog) < 0){
		printf("ERROR: Socket failed to start listening");
		exit(1);
	}

	//Return our stack allocated server
	return server;
}


void run(Server *server){
	//Allocate our buffer
	char buffer[BUFFER];

	while(1){
		printf("Server active and waiting for connection at Address %d : %d\n", AF_INET, server->port);
		//Grab the length of our socket's address
		int address_length = sizeof(server->socket_addr);
		//Accept a new connection and create a new connected socket
		int new_socket = accept(server->socket, (struct sockaddr*)(&server->socket_addr), (socklen_t*)(&address_length));
		//Read our request into the buffer
		ssize_t bytes_read = read(new_socket, buffer, BUFFER - 1);

		//If we actually read something
		if(bytes_read >= 0){
			//Null terminate the string
			buffer[bytes_read] = '\0';
			//Probably will get rid of this
			puts(buffer);
		} else {
			printf("ERROR: Buffer could not be read\n");
		}

		//Craft the response
        char *response = "HTTP/1.1 200 OK\r\n"
                         "Content-Type: text/html; charset=UTF-8\r\n\r\n"
                         "<!DOCTYPE html>\r\n"
                         "<html>\r\n"
                         "<head>\r\n"
                         "<title>Testing Basic HTTP-SERVER</title>\r\n"
                         "</head>\r\n"
                         "<body>\r\n"
                         "I work\r\n"
                         "</body>\r\n"
                         "</html>\r\n";
		//Write our response to the socket
		write(new_socket, response, strlen(response) );
		//Request is handled, close the new socket
		close(new_socket);
	}
}

