/**
 * Author: Jack Robbins
 * This is the header file that defines the functionality of the server
 */

#ifndef SERVER_H
#define SERVER_H

//Give ourselves a data buffer
#define BUFFER 20000

//For our socket functionality
#include <netinet/in.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>

/**
 * Define a struct for a server that contains all of the needed information 
 * for transmission
 */
struct Server{
	u_int32_t domain;
	u_int32_t port;	
	u_int32_t service;
	u_int32_t protocol;
	u_int32_t backlog;
	u_int64_t interface;	

	int socket;
	struct sockaddr_in socket_addr;
};

/**
 */
struct server_thread_params{
	struct Server* server;	
	int inbound_socket;
};


	
//A constructor for our server
struct Server create_server(u_int32_t domain, u_int32_t port, u_int32_t service, u_int32_t protocol, u_int32_t backlog, u_int64_t interface);
//A function that serves to start everything up
void run(struct Server* server);

#endif
