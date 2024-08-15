/**
 * Author: Jack Robbins
 * This file contains the prototypes and data structures needed for the html parsing engine
 */

//TODO need more in here
enum token{
	L_ANGLE,
	R_ANGLE
};


//TODO need more in here
enum request_type{
	R_INITIAL,
	R_PUZZLE_INITIAL
};



request_type parse_request();
