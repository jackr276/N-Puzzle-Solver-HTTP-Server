/**
 * Author: Jack Robbins
 * This file contains the implementation of the functions outlined in server.h
 */  

#include "server.h"
//For multithreading
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netdb.h>

//Global variable that holds our socket here
int server_socket;


static void teardown_thread_params(struct server_thread_params* params){
	cleanup_request_details(params->request_details);
	teardown_response(params->response);
	free(params);
}


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
		printf("ERROR: Socket initializiation failed\n");
		exit(1);
	}

	//Now attempt to bind the socket to the address. If we can't, hard exit
	if(bind(server.socket, (struct sockaddr*)(&server.socket_addr), sizeof(server.socket_addr)) < 0){
		printf("ERROR: Socket failed to bind\n");
		exit(1);
	}

 
	//Finally attempt to begin listening. If that fails, hard exit
	if(listen(server.socket, server.backlog) < 0){
		printf("ERROR: Socket failed to start listening\n");
		exit(1);
	}

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
	sprintf(ip_addr, "%u.%u.%u.%u", fourth, third, second, first);

	return ip_addr;
}


/**
 * Thread worker method: handles each new request on a separate thread. This allows us to potentially have
 * more than one connection active at a time if we'd like
 */
static void* handle_request(void* server_thread_params){
	//Cast appropriately
	struct server_thread_params* params = (struct server_thread_params*)(server_thread_params);

	//Receive data from a connection
	params->bytes_read = recv(params->inbound_socket, params->buffer, BUFFER, 0);
	
	//If we didn't read anything, we will close the socket and leave
	if(params->bytes_read < 0){
		printf("No data received from client\n");

		//Shutdown the socket
		shutdown(params->inbound_socket, SHUT_RDWR);

		//Request is handled, close the new socket
		close(params->inbound_socket);	

		//Param cleanup
		teardown_thread_params(params);

		return NULL;
	}

	printf("%s\n", params->buffer);
	//If we get here, we know that we got a response that needs to be parsed
	params->request_details = parse_request(params->buffer);

	//What kind of request that we have determines the response
	switch(params->request_details->type){
		//If we receive a GET request, that means that the user wants to see the landing page
		case R_GET: 
			printf("Received a GET request\n");
			//Craft our response
			params->response = initial_landing_response();

			//Send a response
			params->bytes_written = send(params->inbound_socket, params->response->html, strlen(params->response->html), 0);

			//If the client did not get our data, we have an error
			if(params->bytes_written == -1){
				printf("ERROR: Client did not receive sent data. Connection will be closed.\n");
				//Param cleanup
				teardown_thread_params(params);
				return NULL;
			}

			break;

		//A post request means that we want to solve the entire puzzle
		case R_POST:
			printf("Received a POST request\n");
			printf("N: %d Complexity: %d \n", params->request_details->N, params->request_details->complexity);
		
			//For our initial and goal configs
			struct state* initial;
			struct state* goal;

			//Generate the initial starting config
			initial = generate_start_config(params->request_details->complexity, params->request_details->N);
			//Generate the goal config too
			goal = initialize_goal(params->request_details->N);


			//Generate the config response
			params->response = initial_config_response(params->request_details->N, initial);

			//Send a response
			params->bytes_written = send(params->inbound_socket, params->response->html, strlen(params->response->html), 0);

			//Free up our response here
			teardown_response(params->response);

			//If the client did not get our data, we have an error
			if(params->bytes_written == -1){
				printf("ERROR: Client did not receive sent data. Connection will be closed.\n");
				//Parameter cleanup
				teardown_thread_params(params);

				return NULL;
			}

			//Attempt to solve the puzzle
			struct state* solution_path = solve(params->request_details->N, initial, goal, 0);

			//Construct the solution path
			params->response = solution_response(params->request_details->N, solution_path);

			//Send the final response
			params->bytes_written = send(params->inbound_socket, params->response->html, strlen(params->response->html), 0);

			break;
			
		//A default break otherwise
		default:
			break;
	}

	//Shutdown the socket
	shutdown(params->inbound_socket, SHUT_RDWR);

	//Request is handled, close the new socket
	close(params->inbound_socket);
	
	//Parameter cleanup
	teardown_thread_params(params);

	//Exit the thread
	printf("Request handled successfully.\n");
	pthread_exit(NULL);
}


/**
 * Signal interrupt handler to enable a graceful exit on CTRL-C
 */
static void sigint_handler(const int sig_num){
	//Let the user know what is happening
	printf("\nServer closing on <CTRL-C>(Signal Interrupt %d)\nAll sockets closing\n", sig_num);

	//Shutdown and close the socket
	shutdown(server_socket, SHUT_RDWR);
	close(server_socket);

	//Exit the program with code 0
	exit(0);
}


/**
 * A simple function that prints out our hostname and address
 */
void print_initial_message(struct Server* server){

	//Declare a buffer for the hostname
	char host_name[256];

	//Grab the hostname and store it into the host buffer
	if(gethostname(host_name, 256) == -1){
		//We really should never end up here
		printf("ERROR: Your computer appears to have no hostname. Server functionality is therefore impossible\n");
		exit(1);
	}

	//Pointer to our host entry
	struct hostent* host_entry;

	//Grab the ip addresses of the host
	if((host_entry = gethostbyname(host_name)) == NULL){
		//We also really shouldn't end up here
		printf("ERROR: There appears to be no IP addresses associated with your computers hostname. Server functionality is therefore impossible\n");
		exit(1);
	}	

	//Grab the list of addresses
	struct in_addr** address_list = (struct in_addr**)host_entry->h_addr_list;
	char* ip_addr;

	//Go through these addresses until we find the non null one	
	for(int i = 0; address_list[i] != NULL; i++){
		ip_addr = IPv4_addr_parser(address_list[i]->s_addr);	
	}

	printf("=============================================================================\n");
	//Let the user know that we are good to go
	printf("Server active on host machine: %s\n", host_name);
	printf("Waiting for inbound connections at address http://%s:%d\n", ip_addr, server->port);
	printf("=============================================================================\n\n");
	//For some reason we wouldn't get a display unless we flush to stdout
	fflush(stdout);

	//Release this from memory
	free(ip_addr);
}


/**
 * Run the server that is referenced in the parameter. This method serves as a thread entry point for
 * the concurrent server handling that we have
 */
void run(struct Server* server){
	//Assign the server socket global variable for our shutdown
	server_socket = server->socket;

	//Display the initial message for our user
	print_initial_message(server);

	//Listen for a <CTRL-C> signal and use the handler to perform graceful shutdown
	signal(SIGINT, sigint_handler);

	//Listen for new connections
 	while(1){
		//Grab the length of our socket's address
		int address_length = sizeof(server->socket_addr);
		//Accept a new connection and create a new connected socket
		int new_socket = accept(server->socket, (struct sockaddr*)(&server->socket_addr), (socklen_t*)(&address_length));
	
		//Stack allocate a thread paramater structure
 		struct server_thread_params* params = (struct server_thread_params*)malloc(sizeof(struct server_thread_params));
		params->inbound_socket = new_socket;
		params->server = server; 

		//Let the logs know what is happening
		printf("A new connection has been detected and is being handed to a new server thread.\n");

		//Stack allocate a new thread;
		pthread_t request_handler;

		//Create the thread to handle the request
		pthread_create(&request_handler, NULL, handle_request, params);
	}
}
