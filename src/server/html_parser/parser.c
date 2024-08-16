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
request_type parse_request(char* html_request){
	char c;
	unsigned int current_ptr = 0;
	
	//Continuously grab the next character until we hit the end
	while((c = get_next_char(html_request,   &current_ptr)) != '\0'){
		//Switch on the character
		switch(c){
			//We're probably seeing the start of a 'GET' request
			case 'G':
				
				break;
			case 'P':
				break;

		}
	}

	return R_INITIAL_LANDING;
}
