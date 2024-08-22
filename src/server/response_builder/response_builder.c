/**
 * Author: Jack Robbins
 * This implementation file implements the response functionality described in the header file
 * of the same name
 */

#include "response_builder.h"
#include <stdlib.h>

/**
 * Perform any needed teardowns on the heap allocated components of the response, namely
 * the response that is malloced
 * 
 * May not need this but it could be nice
 */
void teardown_response(struct response* r){
	//Avoid a bad free attempt
	if(r == NULL){
		return;
	}

	//Free the html
	if(r->html != NULL){
		free(r->html);
	}

	//If we have a grid, free that too
	if(r->grid != NULL){
		free(r->grid);
	}

	//If we have CSS, free that as well
	if(r->style != NULL){
		free(r->style);
	}

	//Free the response struct itself
	free(r);
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
struct response* initial_landing_response(){
	//Allocate our response
	struct response* response = (struct response*)malloc(sizeof(struct response));

	//Save the type in here for later
	response->type = RSP_INITIAL;

	//Allocate the space that we need for our initial html
	response->html = (char*)malloc(RESPONSE_SIZE);

	//Populate the initial HTML
	sprintf(response->html, "HTTP/2.0 200 OK\r\n"
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
	response->grid = NULL;
	//We also don't have CSS, so set it to be null to alert the freer method
	response->style = NULL;

	//Give the response back
	return response;
}


/**
 * Construct the initial response that displays for the user the grid after they've entered in N, etc.
 */
struct response* initial_config_response(const int N, struct state* state_ptr){ 
	//Allocate our response
	struct response* response = (struct response*)malloc(sizeof(struct response));

	//Generate the grid for our initial response here
	response->grid = construct_grid_display(N, state_ptr);

	//Allocate the space that we need for our html
	response->html = (char*)malloc(RESPONSE_SIZE);


	//Populate the initial HTML
	sprintf(response->html, "HTTP/2.0 200 OK\r\n"
			 				   "Connection: keep-alive\r\n"
             				   "Content-Type: text/html; charset=UTF-8\r\n"
			 				   "Keep-Alive: timeout=15, max=1000\r\n\r\n"
           				 	   "<!DOCTYPE html>\r\n"
             				   "<html>\r\n"
             				   "<head>\r\n");

	//Create our styles
	response->style = css_grid_builder(N);

	//Add in all needed css
	strcat(response->html, response->style);

	//Add the remainder in here
	strcat(response->html,  "<title>N Puzzle Solver</title>\r\n"
             				   "</head>\r\n"
  				               "<body>\r\n"
						       "<h1>N Puzzle Solver</h1>\r\n");
	
	//Add our grid in
	strcat(response->html, response->grid);
		
	//return the response
	return response;
}


/**
 * Construct the response that shows the full solution path
 */
struct response* solution_response(const int N, struct state* solution_path){
	//Allocated response
	struct response* response = (struct response*)malloc(sizeof(struct response));
	
	//Get some space for our response
	response->html = (char*)malloc(RESPONSE_SIZE);

	//Add in the initial headings
	sprintf(response->html, "<h2>Solution Found!</h2><br>\r\n"
							   "<h2>Solution Path</h2><br>\r\n");	

	//Set these as warnings to the resonse deconstructor
	response->grid = NULL;
	response->style = NULL;

	//Define a cursor to traverse our linked list
	struct state* cursor = solution_path;
	
	//Traverse the solution path
	while(cursor != NULL){
		//If we have a previous grid, we need to free it or else we'll leak
		if(response->grid != NULL){
			free(response->grid);
		}
		//Remake the new grid 
		response->grid = construct_grid_display(N,cursor);

		//Add the grid into our response
		strcat(response->html, response->grid);

		//Advance the linked list
		cursor = cursor->next;
	}

	//Close the entire thing up
	strcat(response->html, "</body>\r\n</html>\r\n\r\n");

	//Cleanup the solution path
	cleanup_solution_path(solution_path);

	//Give the response back
	return response;
}
