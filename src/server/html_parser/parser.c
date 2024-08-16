/**
 * Author: Jack Robbins
 * This c file contains the implementation for the functions prototyped in 
 * parser.h
 */

#include "parser.h"

/**
 * A non destructive get_next_char method that preserves the original string.
 * Returns the character at the position of current_ptr in the string
 */
static char get_next_char(char* string, unsigned int* current_ptr){
	//Grab the char at location current_ptr
	char next_char = *(string + *current_ptr);

	//Increment the current_ptr for the next call
	(*current_ptr)++;

	//Return the next char
	return next_char;
}


/**
 * Take in a request string and determine what type of request that we have
 */
struct request_details parse_request(char* http_request){
	char c;
	unsigned int current_ptr = 0;
	
	//Stack allocate our request details
	struct request_details details;
	//By default, we are in an error state because we haven't yet parsed
	details.type = R_ERR;
	details.N = -1;
	details.complexity = -1;
	
	//Continuously grab the next character until we hit the end
	while((c = get_next_char(http_request,   &current_ptr)) != '\0'){
		//Switch on the character
		switch(c){
			//We're probably seeing the start of a 'GET' request
			case 'G':
				//If we don't see an E, then we're not in a get request
				if(get_next_char(http_request, &current_ptr) != 'E'){
					//Put back the E
					current_ptr--;
					break;
				}

				//If we saw and E put don't see a 'T', then we're also not in a GET request
				if(get_next_char(http_request, &current_ptr) != 'T'){
					//Put back the 'E' and whatever else we saw
					current_ptr -= 2;
					break;
				}
				
				//If we made it here, we know that we have a GET request so update the details
				details.type = R_GET;
				break;
 
			//If we see this, we could be at the start of a POST request
			case 'P':
				//If we don't see an O, then we aren't in post
				if(get_next_char(http_request, &current_ptr) != 'O'){
					//"put" the char back 
					current_ptr--;
					break;
				}

				//If we don't see an 'S', then we aren't in a post
				if(get_next_char(http_request, &current_ptr) != 'S'){
					//"put" the 2 chars back
					current_ptr -= 2;
					break;
				}

				//If we don't see a "T", then we aren't in a post
				if(get_next_char(http_request, &current_ptr) != 'T'){
					//"put" the three chars back
					current_ptr -= 3;
					break;
				}
				
				//If we do make it here, we know that we have the beginnings of a post request	
				details.type = R_POST;
				break;

		 	//TODO finishme
		}
	}

	return details;
}
