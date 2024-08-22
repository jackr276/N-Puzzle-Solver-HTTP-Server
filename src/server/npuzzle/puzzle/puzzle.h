/**
 * Author: Jack Robbins
 * This header file contains state information and function prototypes that
 * are implemented in puzzle.c
 */


#ifndef PUZZLE_H
#define PUZZLE_H

#define ARRAY_START_SIZE 5000

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>


/**
* Defines a type of state, which is a structure, that represents a configuration in the gem puzzle game
*/
struct state {
   //Define a dynamic 2D array for the tiles since we have a variable puzzle size
   short* tiles;
   //For A*, define the total_cost, how far the tile has traveled, and heuristic cost int total_cost, current_travel, heuristic_cost;
   int total_cost, current_travel, heuristic_cost;
   //location (row and colum) of blank tile 0
   short zero_row, zero_column;
   //The next state in the linked list(fringe or closed), NOT a successor
   struct state* next;
   //The predecessor of the current state, used for tracing back a solution	
   struct state* predecessor;			
};


/**
 * Define a struct that holds everything that we need for the closed array
 */
struct closed {
	struct state** array;
	int next_closed_index;
	int closed_max_size;
};


/**
 * Define a struct that holds everything that we need for the fringe min heap
 */
struct fringe {
	struct state** heap;
	int next_fringe_index;
	int fringe_max_size;
};


/* Method Protoypes */
void initialize_state(struct state*, const int);
void destroy_state(struct state*);
void cleanup_fringe_closed(struct fringe*, struct closed*, struct state*, const int);
void cleanup_solution_path(struct state*);
void print_state(struct state*, const int, int);
void copy_state(struct state*, struct state*, const int);
void move_down(struct state*, const int);
void move_right(struct state*, const int);
void move_up(struct state*, const int);
void move_left(struct state*, const int);
int states_same(struct state*, struct state*, const int);
void update_prediction_function(struct state*, int);
struct state* initialize_goal(const int);
struct state* generate_start_config(const int, const int);
struct closed* initialize_closed(void);
struct fringe* initialize_fringe(void);
void merge_to_closed(struct closed*, struct state*);
struct state* dequeue(struct fringe*);
struct state* generate_start_config(const int, const int);
int fringe_empty(struct fringe*);
void check_repeating_fringe(struct fringe*, struct state**, const int);
void check_repeating_closed(struct closed*, struct state**, const int);
int merge_to_fringe(struct fringe*, struct state*[4]);

#endif /* PUZZLE_H */
