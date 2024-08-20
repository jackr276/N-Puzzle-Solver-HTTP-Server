/**
 * Author: Jack Robbins
 * This c file contains the implementation for the functions prototyped in 
 * parser.h
 */

#include "parser.h"
#include <stdio.h>

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

			//If we see this and we are in a POST request, this is our N value
			case 'N':
				//If we didn't already find a post request then we don't care
				if(details.type != R_POST){
					break;
				}

				//If we don't see the equals sign, then we don't have our N value
				if(get_next_char(http_request, &current_ptr) != '='){
					current_ptr--;
					break;
				}

				//We should see the number for N here
				char num = get_next_char(http_request, &current_ptr);

				//Verify that we actually have a number in here
				if(num - '0' < 0 || num - '9' > 9){
					printf("ERROR: Invalid input for N");
					//We need to put back the 2 characters we've consumed
					current_ptr -= 2;
					break;
				}
				
				//We found our N
				details.N = num - '0';
				break;

			//If we see this and we are in a post request, then this is our complexity value
			case 'c':
				//If we are not in a post request, then we don't care
				if(details.type != R_POST){
					break;
				}

				//We need to see an 'o' next in the request, if we don't put the char back and break
				if(get_next_char(http_request, &current_ptr) != 'o'){
					current_ptr--;
					break;
				}

				//We need to see an 'm' next in the request, if we don't put the chars back and break
				if(get_next_char(http_request, &current_ptr) != 'm'){
					current_ptr -= 2;
					break;
				}

				//We need to see a 'p' next in the request, if we don't put the chars back and break
				if(get_next_char(http_request, &current_ptr) != 'p'){
					current_ptr -= 3;
					break;
				}

				//We need to see an 'l' next in the request, if we don't put the chars back and break
				if(get_next_char(http_request, &current_ptr) != 'l'){
					current_ptr -= 4;
					break;
				}

				//We need to see an 'e' next in the request, if we don't put the chars back and break
				if(get_next_char(http_request, &current_ptr) != 'e'){
					current_ptr -= 5;
					break;
				}

				//We need to see an 'x' next in the request, if we don't put the chars back and break
				if(get_next_char(http_request, &current_ptr) != 'x'){
					current_ptr -= 6;
					break;
				}

				//We need to see an 'i' next in the request, if we don't put the chars back and break
				if(get_next_char(http_request, &current_ptr) != 'i'){
					current_ptr -= 7;
					break;
				}

				//We need to see a 't' next in the request, if we don't put the chars back and break
				if(get_next_char(http_request, &current_ptr) != 't'){
					current_ptr -= 8;
					break;
				}

				//We need to see a 'y' next in the request, if we don't put the chars back and break
				if(get_next_char(http_request, &current_ptr) != 'y'){
					current_ptr -= 9;
					break;
				}

				//If we don't see the equals sign, then we don't have our CMP value
				if(get_next_char(http_request, &current_ptr) != '='){
					current_ptr -= 10;
					break;
				}

				//Now we have to parse whatever number is in here
				//Remember -- the maximum value is 3 digits, but we may not have 3 digits
				short hundreds = 0;
				short tens = 0;
				short ones = 0;

				char ch;
				//Grab the first number. This may be the ones, tens or hundreds place, we currently don't know
				if((ch = get_next_char(http_request, &current_ptr)) != '\0'){
					if(ch - '0' < 0 || ch - '9' > 9){
						//Put it back
						current_ptr--;
						//Put our details into an error state
						details.type = R_ERR;
						break;
					}

					//Store this value in the ones for now
					ones = ch - '0';

					//Currently, our complexity is just the ones
					details.complexity = ones;

				//If we get here, we say an equals with no value, so we have a bad request
				} else {
					details.type = R_ERR;
					return details;
				}

				//Grab the second number, if there even is one
				if((ch = get_next_char(http_request, &current_ptr)) != '\0'){
					if(ch - '0' < 0 || ch - '9' > 9){
						//Throw it back into the stream
						current_ptr--;
						break;
					} 

					//Swap the ones and tens
					tens = ones;
					//Store this value in the ones
					ones = ch - '0';

					//Now currently our complexity is the ones and tens;
					details.complexity = tens * 10 + ones;
				
				//If we do get here, it means that we saw a null terminator, so the request is over
				} else {
					return details;
				}

				//Grab the third number, if there is one
				if((ch = get_next_char(http_request, &current_ptr)) != '\0'){
					if(ch - '0' < 0 || ch - '9' > 9){
						//Throw it back into the stream
						current_ptr--;
						break;
					}

					//If we get here, the tens become hundres, ones becomes tens, and this new number becomes the ones
					hundreds = tens;
					tens = ones; 
					ones = ch - '0';

					//Now our comlexity is this 
					details.complexity = hundreds * 100 + tens * 10 + ones;

				//If we do get here, it means that we found the null terminator, so the request is over
				} else {
					return details;
				}
		}
	}

	return details;
}
