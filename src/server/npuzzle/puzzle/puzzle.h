/**
 * Author: Jack Robbins
 * This header file contains state information and function prototypes that
 * are implemented in puzzle.c
 */


#ifndef PUZZLE_H
#define PUZZLE_H

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


/* Method Protoypes */
void initialize_state(struct state*, const int);
void destroy_state(struct state*);
void cleanup_fringe_closed(struct state*, const int);
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
void initialize_closed(void);
void initialize_fringe(void);
void merge_to_closed(struct state*);
void priority_queue_insert(struct state*);
struct state* dequeue();
struct state* generate_start_config(const int, const int);
int fringe_empty();
void check_repeating_fringe(struct state**, const int);
void check_repeating_closed(struct state**, const int);
int merge_to_fringe(struct state*[4]);

#endif /* PUZZLE_H */
