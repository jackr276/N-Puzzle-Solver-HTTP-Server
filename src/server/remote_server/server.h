/**
 * Author: Jack Robbins
 * This is the header file that defines the functionality of the server
 */

#ifndef SERVER_H
#define SERVER_H

//Give ourselves a data buffer, this may be more 
#define BUFFER 20000

//For our socket functionality
#include <netinet/in.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <signal.h>

#include "../response_builder/response_builder.h"
#include "../html_parser/parser.h"
#include "../npuzzle/puzzle/puzzle.h"
#include "../npuzzle//solver//solve.h"

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
 * Thread paramaters for the multithreaded client connection handling
 */
struct server_thread_params{
	struct Server* server;	
	int inbound_socket;
	char buffer[BUFFER];
	ssize_t bytes_read;
	ssize_t bytes_written;
	struct response* response;
	struct request_details* request_details;	
	struct state* initial;
	struct state* goal;
};


/**
 * A constructor for a stack allocated server struct
 */
struct Server create_server(u_int32_t domain, u_int32_t port, u_int32_t service, u_int32_t protocol, u_int32_t backlog, u_int64_t interface);

/**
 * Runs the server on a separate thread
 *
 * THREAD_ENTRY_POINT
 */
void run(struct Server* server);

#endif /* SERVER_H */
