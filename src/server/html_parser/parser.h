/**
 * Author: Jack Robbins
 * This file contains the prototypes and data structures needed for the html parsing engine
 */

//TODO need more in here
typedef enum {
	L_ANGLE,
	R_ANGLE
} token;

//TODO need more in here
typedef enum {
	R_GET,
	R_POST,
	R_PUZZLE_INITIAL,
	R_PUZZLE_SOLVE,
	R_ERR
} request_type;

//Maybe need this but don't know
struct request_details{
	request_type type;
	int N;
	int complexity;
};

/**
 * Parse an html request message and return what request type it is 
 */
struct request_details parse_request(char* html_request);
