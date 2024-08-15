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



request_type parse_request();
