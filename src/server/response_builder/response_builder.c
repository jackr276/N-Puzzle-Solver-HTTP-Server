/**
 * Author: Jack Robbins
 * This implementation file implements the response functionality described in the header file
 * of the same name
 */

#include "response_builder.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>


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

	//If we have CSS, free that as well
	if(r.style != NULL){
		free(r.style);
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
	strcat(grid_display, "</div><br><br>\r\n");

	//Once we're done, get rid of this memory
	free(grid_item);

	//Return the pointer to our grid display
	return grid_display;
}


/**
 * A function that will construct the necessary CSS to make our N puzzle show 
 * up as a grid
 */
static char* css_grid_builder(const int N){
	//Reserver space for the css
	char* style = (char*)malloc(1000);

	//Add the initial style in
	sprintf(style, "<style>\r\n"
				   			".grid_container {display: grid; grid-template-columns: ");
             				   
	//We need to adaptively add in the size in pixels each time keyword N times to have an appopriate number of columns
	for(int i = 0; i < N; i++){
		strcat(style, "40px ");	
	}

	//Now we can close up the grid container style
	strcat(style, ";}\r\n");

	//Add in the style for our grid elements
	strcat(style, ".grid_item{"
							 "border: 2px solid rgba(0, 0, 0, 0.8);"
							 "font-size: 30px;"
							 "text-align: center;"
							 "}\r\n");

	//Close the style up
	strcat(style, "</style>\r\n");

	//Return the string
	return style;
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
	sprintf(r.html, "HTTP/2.0 200 OK\r\n"
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
  			 			       "<input type=\"text\" maxlength = \"1\" id=\"N\" name=\"N\" placeholder=\"N\"><br><br>\r\n"
			 				   "<label for = \"complexity\">Enter a value for the complexity of the initial configuration:</label>\r\n"
  							   "<input type=\"text\" maxlength = \"3\" id=\"CMP\" name=\"complexity\" placeholder=\"Complexity\"><br><br>\r\n"
							   "<input type=\"submit\" value=\"Generate Start Configuration and Solve\">\r\n"
			 				   "</form>\r\n"
             				   "</body>\r\n"
        					   "</html>\r\n\r\n");

	//We don't have a grid here, so set it to null to alert the freer method
	r.grid = NULL;
	//We also don't have CSS, so set it to be null to alert the freer method
	r.style = NULL;

	//Give the response back
	return r;
}


/**
 * Construct the initial response that displays for the user the grid after they've entered in N, etc.
 */
struct response initial_config_response(const int N, struct state* state_ptr){ 
	//Stack allocate our response
	struct response r;

	//Generate the grid for our initial response here
	r.grid = construct_grid_display(N, state_ptr);

	//Allocate the space that we need for our html
	r.html = (char*)malloc(RESPONSE_SIZE);


	//Populate the initial HTML
	sprintf(r.html, "HTTP/2.0 200 OK\r\n"
			 				   "Connection: keep-alive\r\n"
             				   "Content-Type: text/html; charset=UTF-8\r\n"
			 				   "Keep-Alive: timeout=15, max=1000\r\n\r\n"
           				 	   "<!DOCTYPE html>\r\n"
             				   "<html>\r\n"
             				   "<head>\r\n");

	//Create our styles
	r.style = css_grid_builder(N);

	//Add in all needed css
	strcat(r.html, r.style);

	//Add the remainder in here
	strcat(r.html,  "<title>N Puzzle Solver</title>\r\n"
             				   "</head>\r\n"
  				               "<body>\r\n"
						       "<h1>N Puzzle Solver</h1>\r\n");
	
	//Add our grid in
	strcat(r.html, r.grid);
		
	//return the response
	return r;
}


struct response solution_response(const int N, struct state* solution_path){
	//Stack allocated response
	struct response r;
	
	//Get some space for our response
	r.html = (char*)malloc(RESPONSE_SIZE);

	//Add in the initial headings
	sprintf(r.html, "<h2>Solution Found!</h2><br>\r\n"
							   "<h2>Solution Path</h2><br>\r\n");	

	r.grid = NULL;

	//Define a cursor to traverse our linked list
	struct state* cursor = solution_path;
	
	//Traverse the solution path
	while(cursor != NULL){
		//If we have a previous grid, we need to free it or else we'll leak
		if(r.grid != NULL){
			free(r.grid);
		}
		//Remake the new grid 
		r.grid = construct_grid_display(N,cursor);

		//Add the grid into our response
		strcat(r.html, r.grid);

		//Advance the linked list
		cursor = cursor->next;
	}

	//Close the entire thing up
	strcat(r.html, "</body>\r\n</html>\r\n\r\n");

	//Give the response back
	return r;
}
