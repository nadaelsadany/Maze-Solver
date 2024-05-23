#ifndef PATH_PLANNER_H
#define PATH_PLANNER_H

#include "maze_commons.h"

bool check_left_wall(void);
bool check_right_wall(void);

bool check_front_wall(void);
void move_single_cell(void);

static void turn_around(void);

void maintain_pos_and_orient(void);

void change_dir ( Maze *, short *, short *, short *);

void visit_node(Maze * , Stack * , short , short , short );


void visit_next_node(Maze * , Stack * , short , short , short );

void check_goal_reached (short * , short * , short * ) ;

void set_center_walls(short , short );

void reflood_from_goal(void);

#endif // !PATH_PLANNER_H
