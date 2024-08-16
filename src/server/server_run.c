/**
 * Author: Jack Robbins
 * This runner file serves as the entry point to start up our server and begin handling requests
 */

//Hook into our and the npuzzle
#include "npuzzle/solver/solve.h"
#include "npuzzle/puzzle/puzzle.h"
#include "remote_server/server.h"
#include <stdio.h>
#include <unistd.h>

/**
 * This function essentially duplicates the functionality 
 * of the program that inspired this one. It is purely on the 
 * command line, so all remote server functionality is ignored
 */
int run_command_line(){
	//Welcome message
	printf("\n\n===========================================================================\n");
	printf("Welcome to the N Puzzle Solver\n");
	printf("===========================================================================\n");
 
	//Grab N from the user
	int N;
	printf("Enter the dimension N: ");
	scanf("%d", &N);

	//Input validation
	if(N < 3){ 
		printf("Error: N-Puzzle dimension must be 3x3 or higher\n");
		exit(1);
	}

	int complexity;
	printf("Enter the complexity of the initial configuration: ");
	scanf("%d", &complexity);
 
	//Generate the starting and goal configuration
	struct state* initial = generate_start_config(complexity, N);
	struct state* goal = initialize_goal(N);

	//Simply make a call to solve and let it go from there
	return solve(N, initial, goal, 1);
}


/**
 * Run the server side methods to make this a truly "remote" N Puzzle Solver
 */
int run_server(){
	struct Server server = create_server(AF_INET, 2025, SOCK_STREAM, 0, 10, INADDR_ANY);
	run(&server);
	close(server.socket);
	return 0;
}

/**
 * Our main function here will use getopt to split apart our command line
 * arguments. It will be possible to use debug mode to void all of the server
 * funtionality entirely
 */
int main(int argc, char** argv){	
	char opt;

	//The user can decide to initialize in remote server mode in command line mode
	while((opt = getopt(argc, argv, "dr")) != -1){
		//Based on our option here
		switch(opt){
			//User wants debug mode
			case 'd':
				//Hand everything off to the above method
				run_command_line();
				break;
			//User wants remote server mode
			case 'r':
				run_server();
				//Server run method
				break;
			//Unknown/default case
			case '?':
			default:
				printf("Error: Invalid command line arguments entered");
				exit(1);
		}
	}

	return 0;
}
