/**
 * Author: Jack Robbins
 * This file represents the header that will point to the solve_multi_threaded file
 */

#ifndef SOLVER_H
#define SOLVER_H

#include "../puzzle/puzzle.h"


/**
 * Define a structure for holding all of our thread parameters
 */
struct thread_params{
	//The predecessor state
	struct state* predecessor;
	//0 = leftMove, 1 = rightMove, 2 = downMove, 3 = upMove
	int option;
	//The size of the N puzzle
	int N;
	//The successors array that we will store the states in
	struct state** successors;
};


//The solve function
int solve(int N, struct state* start_state, struct state* goal_state);


#endif
