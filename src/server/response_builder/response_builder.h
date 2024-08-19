/**
 * Author: Jack Robbins
 * This header file contains the prototypes of functions used in crafting the HTML response for our server
 */

#ifndef RESPONSE_BUILDER_H
#define RESPONSE_BUILDER_H

#define RESPONSE_SIZE 20000

#include <string.h>
#include "../npuzzle/puzzle/puzzle.h"

/**
 * The type of HTML response given
 */
typedef enum {
	RSP_INITIAL,
	RSP_INITIAL_CONF,
	RSP_SOLUTION,

} response_type;

/**
 * A response struct that holds the various things needed with our html string
 */
struct response{
	//The response mainly contains the HTML code that we want to serve up
	char* html;
	char* grid;
	response_type type;
};

/**
 * Serve up the initial landing page response
 */
struct response initial_landing_response();

/**
 * Serve up the response that has the initial grid displayed
 */
struct response initial_config_response(const int, struct state*);

/**
 * Teardown any dynamically allocated memory components in the response
 */
void teardown_response(struct response);
#endif

