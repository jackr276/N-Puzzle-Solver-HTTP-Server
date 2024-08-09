/**
 * Author: Jack Robbins
 * This runner file serves as the entry point to start up our server and begin handling requests
 */

//Hook into our server
#include "remote-server/server.h"
#include "npuzzle/puzzle/puzzle.h"
#include <stdio.h>
#include <unistd.h>


int run_command_line(){


	return 0;
}


/**
 * Our main function here will use getopt to split apart our command line
 * arguments. It will be possible to use debug mode to void all of the server
 * funtionality entirely
 */
int main(int argc, char** argv){

	char opt;

	while((opt = getopt(argc, argv, "dr")) != -1){
		switch(opt){
			case 'd':
				run_command_line();
				break;
			case 'r':
				break;
			default:
				printf("Error: Invalid command line arguments entered");
		}
	}
	return 0;
}
