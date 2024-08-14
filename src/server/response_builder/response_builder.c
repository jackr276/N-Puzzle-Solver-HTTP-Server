/**
 * Author: Jack Robbins
 * This implementation file implements the response functionality described in the header file
 * of the same name
 */

#include "response_builder.h"

/**
 * Construct the response that the user initially gets on the landing page
 */
struct response initial_landing_response(){
	//Stack allocate our response
	struct response r;

	//Populate the initial HTML
	r.html = "HTTP/1.1 200 OK\r\n"
             "Content-Type: text/html; charset=UTF-8\r\n\r\n"
             "<!DOCTYPE html>\r\n"
             "<html>\r\n"
             "<head>\r\n"
             "<title>N Puzzle Solver</title>\r\n"
             "</head>\r\n"
             "<body>\r\n"
			 "<h1>N Puzzle Solver</h1>\r\n"
			 "<form>\r\n"
  			 "<label for = \"N\">First name:</label>\r\n"
  			 "<input type=\"text\" id=\"N\" name=\"N\"><br><br>\r\n"
		     "<input type=\"submit\" value=\"Solve\">\r\n"
			 "</form>\r\n"
             "</body>\r\n"
             "</html>\r\n";

	return r;
}
