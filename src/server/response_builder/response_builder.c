/**
 * Author: Jack Robbins
 * This implementation file implements the response functionality described in the header file
 * of the same name
 */

#include "response_builder.h"
#include <stdio.h>
#include <stdlib.h>


/**
 * Construct the HTML necessary for the grid display that we need on the screen
 */
static char* construct_grid_display(const int N, struct state* state_ptr){
	char* grid_display = (char*)malloc(2000);

	//First print in the start of the grid
	sprintf(grid_display, "<div class = \"wrapper\">");


	return grid_display;
}



/**
 * Construct the response that the user initially gets on the landing page
 */
struct response initial_landing_response(){
	//Stack allocate our response
	struct response r;

	//Allocate the space that we need for our initial html
	r.html = (char*)malloc(RESPONSE_SIZE);

	//Populate the initial HTML
	sprintf(r.html, "HTTP/1.1 200 OK\r\n"
			 				   "Connection: keep-alive\r\n"
             				   "Content-Type: text/html; charset=UTF-8\r\n"
			 				   "Keep-Alive: timeout=15, max=1000\r\n\r\n"
           				 	   "<!DOCTYPE html>\r\n"
             				   "<html>\r\n"
             				   "<head>\r\n"
             				   "<title>N Puzzle Solver</title>\r\n"
             "</head>\r\n"
             "<body>\r\n"
			 "<h1>N Puzzle Solver</h1>\r\n"
			 "<form method=\"POST\">\r\n"
  			 "<label for = \"N\">Enter a value for N:</label>\r\n"
  			 "<input type=\"text\" maxlength = \"1\" id=\"N\" name=\"N\"><br><br>\r\n"
			 "<label for = \"complexity\">Enter a value for the complexity of the initial configuration:</label>\r\n"
  			 "<input type=\"text\" maxlength = \"3\" id=\"CMP\" name=\"complexity\"><br><br>\r\n"
		     "<input type=\"submit\" value=\"Generate Start Config\">\r\n"
			 "</form>\r\n"
             "</body>\r\n"
             "</html>\r\n\r\n");

	//Save the type in here for later
	r.type = RSP_INITIAL;

	return r;
}


struct response initial_config_response(const int N, struct state* state_ptr){ 
	//Stack allocate our response
	struct response r;


	return r;
}



