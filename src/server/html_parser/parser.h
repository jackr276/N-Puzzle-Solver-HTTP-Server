/**
 * Author: Jack Robbins
 * This file contains the prototypes and data structures needed for the html parsing engine
 */


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
struct request_details parse_request(char* http_request);
