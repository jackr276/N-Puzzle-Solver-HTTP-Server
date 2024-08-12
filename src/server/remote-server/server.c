/**
 * Author: Jack Robbins
 * This file contains the implementation of the functions outlined in server.h
 */  

#include "server.h"
#include <stdlib.h>
#include <stdio.h>


Server create_server(u_int32_t domain, u_int32_t port, u_int32_t service, u_int32_t protocol, u_int32_t backlog, u_int64_t interface){
	Server server;

	server.domain = domain; 
	server.port = port;
	server.service = service;
	server.backlog = backlog;
	server.interface = interface;

	server.socket_addr.sin_family = domain; 
	server.socket_addr.sin_port = htons(port);
	server.socket_addr.sin_addr.s_addr = htonl(interface);

	server.socket = socket(domain, service,  protocol);

	if(server.socket < 0){
		printf("ERROR: Socket initializiation failed");
		exit(1);
	}


	return server;
}


void run(Server *server){

}

