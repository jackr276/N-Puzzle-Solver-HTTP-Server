/**
 * Author: Jack Robbins
 * This header file contains the prototypes of functions used in crafting the HTML response for our server
 */

#ifndef RESPONSE_BUILDER_H
#define RESPONSE_BUILDER_H

#define RESPONSE_SIZE 50000

#include "../npuzzle/puzzle/puzzle.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>


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
struct response {
	//The response mainly contains the HTML code that we want to serve up
	char* html;
	char* grid;
	char* style;
	response_type type;
};

/**
 * Serve up the initial landing page response
 */
struct response* initial_landing_response();

/**
 * Serve up the response that has the initial grid displayed
 */
struct response* initial_config_response(const int N, struct state* state_ptr);

/**
 * Serve up the response that shows the entire solution after it's 
 * been found
 */
struct response* solution_response(const int N, struct state* solution);

/**
 * Teardown any dynamically allocated memory components in the response
 */
void teardown_response(struct response* response);

#endif /* RESPONSE_BUILDER_H */
