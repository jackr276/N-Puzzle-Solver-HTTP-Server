/**
 * Author: Jack Robbins
 * This implementation file implements the response functionality described in the header file
 * of the same name
 */

#include "response_builder.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/**
 * Perform any needed teardowns on the heap allocated components of the response, namely
 * the response that is malloced
 * 
 * May not need this but it could be nice
 */
void teardown_response(struct response r){
	//Free the html
	free(r.html);

	//If we have a grid, free that too
	if(r.grid != NULL){
		free(r.grid);
	}
}


/**
 * Construct the HTML necessary for the grid display that we need on the screen
 */
static char* construct_grid_display(const int N, struct state* state_ptr){
	//We need a pointer to a grid display
	char* grid_display = (char*)malloc(5000);

	char* grid_item = (char*)malloc(50);

	//First print in the start of the grid
	sprintf(grid_display, "<div class = \"grid_container\">\r\n");

	for(int i = 0; i < N*N; i++){
		//Wipe the grid item string
		memset(grid_item, 0, 50);
		//Print the div item into it
		sprintf(grid_item, "<div class=\"grid_item\">%d</div>\r\n", state_ptr->tiles[i]);

		//Concatenate the new grid item onto the grid string
		strcat(grid_display, grid_item); 
	}

	//Attach the closing tag for the wrapper
	strcat(grid_display, "</div>\r\n");

	//Once we're done, get rid of this memory
	free(grid_item);

	//Return the pointer to our grid display
	return grid_display;
}


/**
 * Construct the response that the user initially gets on the landing page
 */
struct response initial_landing_response(){
	//Stack allocate our response
	struct response r;

	//Save the type in here for later
	r.type = RSP_INITIAL;

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

	//We don't have a grid here, so set it to null to alert the freer method
	r.grid = NULL;

	//Give the response back
	return r;
}


/**
 * Construct the initial response that displays for the user the grid after they've entered in N, etc.
 */
struct response initial_config_response(const int N, struct state* state_ptr){ 
	//Stack allocate our response
	struct response r;

	return r;
}
