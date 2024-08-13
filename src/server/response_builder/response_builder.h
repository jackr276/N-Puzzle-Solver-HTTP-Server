/**
 * Author: Jack Robbins
 * This header file contains the prototypes of functions used in crafting the HTML response for our server
 */

#ifndef RESPONSE_BUILDER_H
#define RESPONSE_BUILDER_H

struct response{
	//The response mainly contains the HTML code that we want to serve up
	char* html;
};

/**
 * Server up the initial landing page response
 */
struct response initial_landing_response();



#endif

