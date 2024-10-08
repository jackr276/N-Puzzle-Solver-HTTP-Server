/**
 * Author: Jack Robbins
 * This file contains the prototypes and data structures needed for the html parsing engine
 */


#ifndef PARSER_H
#define PARSER_H 

#include <stdio.h>
#include <stdlib.h>

/**
 * The type of HTTP request
 */
typedef enum {
	R_GET,
	R_POST,
	R_PUZZLE_INITIAL,
	R_PUZZLE_SOLVE,
	R_ERR
} request_type;


/**
 * All necessary details of a request. 
 * NOTE: not all requests have values for all fields, it is the caller's responsibility to know 
 * which field is filled for which request type
 */
struct request_details{
	request_type type;
	int N;
	int complexity;
};

/**
 * Parse an html request message and return what request type it is 
 */
struct request_details* parse_request(char* request);

/**
 * Clean up a response by deallocating all memory
 */
void cleanup_request_details(struct request_details* request_details);

#endif /* PARSER_H */
