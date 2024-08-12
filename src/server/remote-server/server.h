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

/**
 * Define a struct for a server that contains all of the needed information 
 * for transmission
 */
typedef struct {
	u_int32_t domain;
	u_int32_t port;	
	u_int32_t service;
	u_int32_t protocol;
	u_int32_t backlog;
	u_int64_t interface;	

	u_int32_t socket;
	struct sockaddr_in socket_addr;
	
} Server;

//A constructor for our server
Server create_server(u_int32_t domain, u_int32_t port, u_int32_t service, u_int32_t backlog, u_int64_t interface);
//A function that serves to start everything up
void launch(Server* server);

#endif
