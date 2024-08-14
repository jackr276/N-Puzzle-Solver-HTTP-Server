/**
 * Author: Jack Robbins
 * This file contains the implementation of the functions outlined in server.h
 */  

#include "server.h"
#include "../response_builder/response_builder.h"
//For multithreading
#include <pthread.h>


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


/**
 * Thread worker method: handles each new request on a separate thread. This allows us to potentially have
 * more than one connection active at a time if we'd like
 */
static void* handle_request(void* server_thread_params){
	//Cast appropriately
	struct server_thread_params* params = (struct server_thread_params*)(server_thread_params);

	//Allocate our buffer
	char buffer[BUFFER];

	//Read our request into the buffer
	ssize_t bytes_read = read(params->inbound_socket, buffer, BUFFER - 1);

	//If we actually read something
	if(bytes_read >= 0){
		//Null terminate the string
		buffer[bytes_read] = '\0';
		//Probably will get rid of this
		puts(buffer);
	} else {
		printf("ERROR: Buffer could not be read\n");
	}

	//Stack allocate our intial respone
	struct response r;
	r = initial_landing_response();

	//Write our response to the socket
	write(params->inbound_socket, r.html, strlen(r.html) );

	sleep(15);
	//Request is handled, close the new socket
	close(params->inbound_socket);

	return NULL;
}


/**
 * Run the server that is referenced in the parameter. This method serves as a thread entry point for
 * the concurrent server handling that we have
 */
void run(struct Server* server){
	while(1){
		printf("Server active and waiting for connection at Address %d : %d\n", AF_INET, server->port);
		//Grab the length of our socket's address
		int address_length = sizeof(server->socket_addr);
		//Accept a new connection and create a new connected socket
		int new_socket = accept(server->socket, (struct sockaddr*)(&server->socket_addr), (socklen_t*)(&address_length));
		
		//Stack allocate a thread paramater structure
		struct server_thread_params params;
		params.inbound_socket = new_socket;
		params.server = server; 

		//Stack allocate a new thread;
		pthread_t request_handler;

		//Create the thread to handle the request
		pthread_create(&request_handler, NULL, handle_request, &params);
	}
}

