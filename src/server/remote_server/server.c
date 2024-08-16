/**
 * Author: Jack Robbins
 * This file contains the implementation of the functions outlined in server.h
 */  

#include "server.h"
#include "../response_builder/response_builder.h"
#include "../html_parser/parser.h"
//For multithreading
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>


/**
 * Stack allocate a server object with the needed parameters
 */
struct Server create_server(u_int32_t domain, u_int32_t port, u_int32_t service, u_int32_t protocol, u_int32_t backlog, u_int64_t interface){
	//Stack allocate our server(may change this)
	struct Server server;

	//Assign all appropriate data
	server.domain = domain; 
	server.port = port;
	server.service = service;
	server.backlog = backlog;
	server.interface = interface;

	//Assign all of these as well
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
	
	printf("Socket Initialized\n");

	//Now attempt to bind the socket to the address. If we can't, hard exit
	if(bind(server.socket, (struct sockaddr*)(&server.socket_addr), sizeof(server.socket_addr)) < 0){
		printf("ERROR: Socket failed to bind");
		exit(1);
	}

	printf("Socket Bound\n");
 
	//Finally attempt to begin listening. If that fails, hard exit
	if(listen(server.socket, server.backlog) < 0){
		printf("ERROR: Socket failed to start listening");
		exit(1);
	}

	printf("Socket Listening\n");

	//Return our stack allocated server
	return server;
}


/**
 * Parse and convert a 32 bit IPv4 address into it's dot(.) notation
 * respresentation
 */
static char* IPv4_addr_parser(const unsigned int inet_addr){ 
	//For our uses in this function only
	typedef unsigned char BYTE;

	//IPv4 address: ###.###.###.###\0 is 16 characters at most  
	char* ip_addr = malloc(sizeof(char) * 16);
 
	//Grab the first byte of data
	BYTE first = (inet_addr >> 24) & 0xFF;

	//Grab the second byte of data
	BYTE second = (inet_addr >> 16) & 0xFF;

	//Grab the third byte of data
	BYTE third = (inet_addr >> 8) & 0xFF;

	//Finally store the fourth byte
	BYTE fourth = inet_addr & 0xFF;

	//Print to the string
	sprintf(ip_addr, "%u.%u.%u.%u", first, second, third, fourth);

	return ip_addr;
}


/**
 * Thread worker method: handles each new request on a separate thread. This allows us to potentially have
 * more than one connection active at a time if we'd like
 */
static void* handle_request(void* server_thread_params){
	//Cast appropriately
	struct server_thread_params* params = (struct server_thread_params*)(server_thread_params);

	//Allocate our buffer
	char buffer[BUFFER];

	//Store the size of what we've read
	ssize_t bytes_read;
	ssize_t bytes_written;
	struct response r;

	//Receive data from a connection
	bytes_read = recv(params->inbound_socket, buffer, BUFFER, 0);
	parse_request(buffer);

	//For debugging
	printf("%s\n", buffer);
	printf("Bytes read: %ld\n", bytes_read);

	//If we didn't read anything, we will leave
	if(bytes_read <= 0){
		printf("No data received from client\n");
		return NULL;
	}

	//Craft our response
	r = initial_landing_response();

	//Send a response
	bytes_written = send(params->inbound_socket, r.html, strlen(r.html), 0);

	//If the client did not get our data, we have an error
	if(bytes_written == -1){
		printf("ERROR: Client did not receive sent data. Connection will be closed.\n");
		return NULL;
	}

	//Shutdown the socket
	shutdown(params->inbound_socket, SHUT_RDWR);

	//Request is handled, close the new socket
	close(params->inbound_socket);

	//Exit the threads
	printf("Request handled successfully.\n");
	pthread_exit(NULL);
}


/**
 * Run the server that is referenced in the parameter. This method serves as a thread entry point for
 * the concurrent server handling that we have
 */
void run(struct Server* server){
	//Grab the ip address
	unsigned int ip = server->socket_addr.sin_addr.s_addr;

	//Translate our current ip to be human readable
	char* ip_addr = IPv4_addr_parser(ip);

	//Let the user know that we are listening
	printf("Server active and waiting for connection at Address %s:%d\n", ip_addr, server->port);
	//Release this from memory
	free(ip_addr);


	//Listen for new connections
	while(1){
		//Grab the length of our socket's address
		int address_length = sizeof(server->socket_addr);
		//Accept a new connection and create a new connected socket
		int new_socket = accept(server->socket, (struct sockaddr*)(&server->socket_addr), (socklen_t*)(&address_length));
	
		//Stack allocate a thread paramater structure
		struct server_thread_params params;
		params.inbound_socket = new_socket;
		params.server = server; 

		//Let the logs know what is happening
		printf("A new connection has been detected and is being handed to a new server thread.\n");

		//Stack allocate a new thread;
		pthread_t request_handler;


		//Create the thread to handle the request
		pthread_create(&request_handler, NULL, handle_request, &params);
	}
}
