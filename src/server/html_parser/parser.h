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
	R_INITIAL_LANDING,
	R_PUZZLE_INITIAL
} request_type;


//TODO may need more in here
struct request_params {
	int N;
	int complexity;
};


/**
 * Parse an html request message and return what type it is
 */
request_type parse_request(char* html_request);
