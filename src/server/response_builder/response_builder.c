/**
 * Author: Jack Robbins
 * This implementation file implements the response functionality described in the header file
 * of the same name
 */

#include "response_builder.h"



struct response initial_landing_response(){
	struct response r;


	r.html = "HTTP/1.1 200 OK\r\n"
             "Content-Type: text/html; charset=UTF-8\r\n\r\n"
             "<!DOCTYPE html>\r\n"
             "<html>\r\n"
             "<head>\r\n"
             "<title>N Puzzle Solver</title>\r\n"
             "</head>\r\n"
             "<body>\r\n"
             "I work\r\n"
             "</body>\r\n"
             "</html>\r\n";


	return r;
}
