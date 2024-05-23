#include "../include/commons.h"

#include "../include/path_planner.h"
#include "../include/sensor_commons.h"
#include "../include/motor_commons.h"

#include "../include/maze_commons.h"


#define TURN_RIGHT_COUNT 1900      //1900 for fake maze
#define TURN_LEFT_COUNT 1850      //1850 for fake maze +/- 50
#define TURN_AROUND_COUNT 3000    //3000 for fake maze
#define ONECELL 17//  M          //7200 for fake maze


//constants for wall sensing left/right/front
#define hasLeftWall 0.7//% //190 for fake maze
#define hasRightWall 10.0 //  //220 for fake maze; 120 for real maze
#define hasFrontWall 7  //300 for fake maze; 
//IR THRESHOLD min dist before stopping
#define PIDSTOP 600    //600 for fake maze;
#define LEFTPID 200    //200 for fake maze
#define RIGHTPID 350  //350 for fake maze
//maybe define new ones for wall sensing for the floodfill




bool check_left_wall(void) {
  if ((side_left_IR > hasLeftWall)){
    return TRUE;
  }
  return FALSE;
}
bool check_right_wall(void) {
  if ((side_right_IR > hasRightWall)){//need to adjust value  
    
    return TRUE;
  }
  return FALSE;
}

bool check_front_wall(void) {
  if ((front_right_IR > hasFrontWall && front_left_IR > hasFrontWall) ){ 

    return TRUE;
  }
  return FALSE;
}

static bool leftWall  = 0;
static bool rightWall = 0;

static float current_yaw_checkpoint;
static float prev_yaw_checkpoint;

static void turn_around(void){
  current_yaw_checkpoint = current_yaw;
  prev_yaw_checkpoint = current_yaw_checkpoint;
  float diff;
  diff = current_yaw_checkpoint - prev_yaw_checkpoint;
  while( diff <= D_2_RAD(180)){
    main_controller(0,MAX_W_ACLKWISE);
  }
}


float current_cell_position;
float previous_cell_checkpoint;

void move_single_cell(void) {
  //initialize encoder values to 0
 
  frontwallflag = 0;
  

  previous_cell_checkpoint = current_cell_position;

  //while the left encoder has not moved 1 "full" cell yet
  while(current_cell_position - previous_cell_checkpoint <= ONECELL && !frontwallflag)
  {
    current_cell_position = current_x;
    
    //IR_module.fire_and_get();    //read in sensors to update sensor readings
    //maintain_pos_and_orient();           //call pid to make sure it is going straight based on sensor readings
  }
  previous_cell_checkpoint = current_cell_position;

  //stop fucntions after finished moving 1 cell
  motorbreak();
  //*/
}



void maintain_pos_and_orient(void){
   leftWall  = check_left_wall();
   rightWall = check_right_wall();

  if ((front_right_IR > PIDSTOP && front_left_IR > PIDSTOP)) //prevents mouse from crashing into wall due to errors.
  {                                        //if this is called, its a BAD sign....
    motorbreak();
    //main_controller(0,0);//STOP EVERYTHING
    frontwallflag = 1;
    return;
  }

  if(leftWall && rightWall) {
  //    Serial.println("in the middle");  
  }
  else if(leftWall){
//    Serial.println("only left wall"); 
       // change in error
  }
  // sensor only reads in right wall
  else if(rightWall){
//    Serial.println("only right wall"); 
       // change in error
  }
  
}//*/


/** Function: change_dir ****ONLY FUNCTION THAT TOUCHES THE MAIN PROGRAM*****
 * Parameters: this_maze - the maze with flood values
 * x,y - current mouse coordinates
 * dir - current direction mouse is facing.
 * Description: makes the mouse face new direction. updates the new coordinates of the mouse.
 */

void change_dir ( Maze * this_maze, short * x, short * y, short * dir){
  
  Node * this_node;  //initialize a node
  short next_dir;    //new direction to face after performing floodfill calculation

  this_node = this_maze->map[(*x)][(*y)];
  next_dir = get_smallest_neighbor_dir(this_node, *dir);

  /* update the appropriate location value x or y */
  if (next_dir == NORTH) 
    (*x) = (*x) - 1;
  else if (next_dir == EAST) 
    (*y) = (*y) + 1;
  else if (next_dir == SOUTH) 
    (*x) = (*x) + 1;
  else if (next_dir == WEST) 
    (*y) = (*y) - 1;

  // Turn the actual mouse in the optimal direction
  if (*dir == NORTH) {
    if (next_dir == WEST)
      pivot_left();
    else if (next_dir == EAST)
      pivot_right();
    else if (next_dir == SOUTH)
      turn_around();
  }

  else if (*dir == EAST) {
    if (next_dir == NORTH)
      pivot_left();
    else if (next_dir == SOUTH)
      pivot_right();
    else if (next_dir == WEST)
      turn_around();
  }

  else if (*dir == SOUTH) {
    if (next_dir == EAST)
      pivot_left();
    else if (next_dir == WEST)
      pivot_right();
    else if (next_dir == NORTH)
      turn_around();
  }

  else if (*dir == WEST) {
    if (next_dir == SOUTH)
      pivot_left();
    else if (next_dir == NORTH)
      pivot_right();
    else if (next_dir == EAST)
      turn_around();
  }

  /* update the direction */
  (*dir) = next_dir;
  wait_ms(200);
}//end change_dir


/** Function: visit_node
 * Parameters: this_maze - maze with flood values
 * this_stack - stack for flood fill
 * x,y - coordinates to be visited
 * flag - whether to update goal cells or not
 * Description: visits the cell, checks for walls, and updates flood values
 */
void visit_node(Maze * this_maze, Stack * this_stack, short x, short y, short flag) {
 
  IR_module.fire_and_get();
  Node * this_node;                                  //initialize a node
  short northwall, eastwall, southwall, westwall;    //boolean values of wall on each side

  this_node = this_maze->map[x][y];                  //initialize node to the node we want to go to
  northwall = eastwall = southwall = westwall = FALSE;

  //correspondingly update the walls based on which direction we are facing
  if (direction == NORTH) {    //check if direction is currently facing NORTH
    if (check_front_wall()) {    //there is a front wall
      set_wall(this_node, NORTH);
      northwall = TRUE;
    }
  }
  else if (direction == EAST){  //check if direction is currently facing EAST
    if (check_front_wall()) {
      set_wall(this_node, EAST);
      eastwall = TRUE;
    }
  }
  else if (direction == SOUTH) {
    if (check_front_wall()) {
      set_wall(this_node, SOUTH);
      southwall = TRUE;
    }
  }
  else {//direction we are facing is WEST
    if (check_front_wall()) {
      set_wall(this_node, WEST);
      westwall = TRUE;
    }
  }

//  Serial.print("Row"); 
//  Serial.println(ROW); 
//  Serial.print("Col"); 
//  Serial.println(COL); 

  /* If there is a wall -> do a series of checks and pushes onto the stack */
  if (northwall) {
    if (this_node->row != 0)
      push (this_stack, MAP[ROW-1][COL]);
    set_wall(this_node, NORTH);
  }
  if (eastwall) {
    if (this_node->column != SIZE-1)
      push (this_stack, MAP[ROW][COL+1]);
    set_wall(this_node, EAST);
  }
  if (southwall) {
    if (this_node->row != SIZE-1)
      push (this_stack, MAP[ROW+1][COL]);
    set_wall(this_node, SOUTH);
  }
  if (westwall) {
    if (this_node->column != 0)
      push (this_stack, MAP[ROW][COL-1]);
    set_wall(this_node, WEST);
  }
  /* push this node itself, as it was updated */
  push(this_stack, this_node);

  /* pop until the stack is empty, and call flood_fill on that node */
  while (!is_empty_Stack(this_stack)) {
    pop(this_stack, &this_node);
    /* NOTE: the flag parameter determines wheter to update goal cells or not */
    flood_fill(this_node, this_stack, flag);
  }
  set_visited (this_node);
}//end visit_node


/** Function: visit_node
 * Parameters: this_maze - maze with flood values
 * this_stack - stack for flood fill
 * x,y - coordinates to be visited
 * flag - whether to update goal cells or not
 * Description: visits the cell, checks for walls, and updates flood values
 */
void visit_next_node(Maze * this_maze, Stack * this_stack, short x, short y, short flag) {
  IR_module.fire_and_get();
  if(check_front_wall()) //return if there is a wall, meaning that we cannot determine wall info about the
    return;              //next node in front of this one.
  Node * next_node = NULL;//node one cell ahead of direction faced.
  short northwall, eastwall, southwall, westwall;    //boolean values of wall on each side

  northwall = eastwall = southwall = westwall = FALSE;

  if( direction == NORTH && (y > 0) )
    next_node = this_maze->map[x][y-1];
  if( direction == EAST && (x < (SIZE - 1)) )
    next_node = this_maze->map[x+1][y];
  if( direction == SOUTH && (y < (SIZE-1)) )
    next_node = this_maze->map[x][y+1];
  if( direction == WEST && (x > 0) )
    next_node = this_maze->map[x-1][y];
    
  if( next_node == NULL ) //return if we are at the boundry of the maze.
    return;

  //correspondingly update the walls based on which direction we are facing
  if (direction == NORTH) {    //check if direction is currently facing NORTH
    if (check_left_wall()) {      //there is a left wall
      set_wall(next_node, WEST);  //set wall on the left
      westwall = TRUE;             
    }
    if (check_right_wall()) {        //there is a right wall
      set_wall(next_node, EAST); 
      eastwall = TRUE;
    }  
  }
  else if (direction == EAST){  //check if direction is currently facing EAST
    if (check_left_wall()) {
      set_wall(next_node, NORTH);
      northwall = TRUE; 
    }
    if (check_right_wall()) {
      set_wall(next_node, SOUTH);
      southwall = TRUE;
    }
  }
  else if (direction == SOUTH) {
    if (check_left_wall()) {
      set_wall(next_node, EAST);
      eastwall = TRUE; 
    }
    if (check_right_wall()) {
      set_wall(next_node, WEST); 
      westwall = TRUE;
    }
  }
  else {    //direction we are facing is WEST
    if (check_left_wall()) {
      set_wall(next_node, SOUTH);
      southwall = TRUE; 
    }
    if (check_right_wall()) {
      set_wall(next_node, NORTH); 
      northwall = TRUE;
    }
  }

  /* If there is a wall -> do a series of checks and pushes onto the stack */
  if (northwall) {
    if (next_node->row != 0)
      push (this_stack, MAP[(next_node->row)-1][next_node->column]);
    set_wall(next_node, NORTH);
  }
  if (eastwall) {
    if (next_node->column != SIZE-1)
      push (this_stack, MAP[next_node->row][(next_node->column)+1]);
    set_wall(next_node, EAST);
  }
  if (southwall) {
    if (next_node->row != SIZE-1)
      push (this_stack, MAP[(next_node->row)+1][next_node->column]);
    set_wall(next_node, SOUTH);
  }
  if (westwall) {
    if (next_node->column != 0)
      push (this_stack, MAP[next_node->row][(next_node->column)-1]);
    set_wall(next_node, WEST);
  }
  /* push this node itself, as it was updated */
  push(this_stack, next_node);

  /* pop until the stack is empty, and call flood_fill on that node */
  while (!is_empty_Stack(this_stack)) {
    pop(this_stack, &next_node);
    /* NOTE: the flag parameter determines wheter to update goal cells or not */
    flood_fill(next_node, this_stack, flag);
  }
  //set_visited (this_node);
}//end visit_next_node



/** Function: check_goal_reached
 * Parameters: x,y - coordinate to be checked
 * found_goal - flag if goal cell was found or not
 * Description: updates flag for whether goal cell was reached
 */
void check_goal_reached (short * x, short * y, short * found_goal) {
  //if the mouse is in the center of the maze -> it found it's goal
  if (*x == SIZE / 2 || *x == SIZE / 2 - 1) {  
    if (*y == SIZE / 2 || *y == SIZE / 2 - 1) {
      *(found_goal) = TRUE;
    }
  }
}

///* update flag for whether goal cell was reached */
//void check_start_reached (short * x, short * y, short * found_start) {
//
//  if (*x == START_X && *y == START_Y) {
//    *(found_start) = TRUE;
//    //printf("Start Coorinates Reached: %d, %d\n", *x, *y);
//  }
//}


/**Function: set_center_walls
   Description: fills in the wall values of the centers based on where
                you discovered the goal from.
*/
void set_center_walls(short entered_x, short entered_y) {

  // 8, 8 : NORTH or WEST
  if (entered_x == SIZE/2 && entered_y == SIZE/2) {

    set_wall(my_maze->map[SIZE/2 - 1][SIZE/2 - 1], NORTH);
    set_wall(my_maze->map[SIZE/2 - 1][SIZE/2 - 1], WEST);
    set_wall(my_maze->map[SIZE/2 - 1][SIZE/2],     NORTH);
    set_wall(my_maze->map[SIZE/2 - 1][SIZE/2],     EAST);
    set_wall(my_maze->map[SIZE/2][SIZE/2 - 1], SOUTH);
    set_wall(my_maze->map[SIZE/2][SIZE/2 - 1], WEST);
  }

  // 8, 7 : NORTH or EAST
  if (entered_x == SIZE/2 && entered_y == SIZE/2 - 1) {

    set_wall(my_maze->map[SIZE/2 - 1][SIZE/2 - 1], NORTH);
    set_wall(my_maze->map[SIZE/2 - 1][SIZE/2 - 1], WEST);
    set_wall(my_maze->map[SIZE/2 - 1][SIZE/2],     NORTH);
    set_wall(my_maze->map[SIZE/2 - 1][SIZE/2],     EAST);
    set_wall(my_maze->map[SIZE/2][SIZE/2], SOUTH);
    set_wall(my_maze->map[SIZE/2][SIZE/2], EAST);
  }

  // 7, 7 : SOUTH or EAST
  if (entered_x == SIZE/2 - 1 && entered_y == SIZE/2 - 1) {

    set_wall(my_maze->map[SIZE/2][SIZE/2 - 1], SOUTH);
    set_wall(my_maze->map[SIZE/2][SIZE/2 - 1], WEST);
    set_wall(my_maze->map[SIZE/2 - 1][SIZE/2],     NORTH);
    set_wall(my_maze->map[SIZE/2 - 1][SIZE/2],     EAST);
    set_wall(my_maze->map[SIZE/2][SIZE/2], SOUTH);
    set_wall(my_maze->map[SIZE/2][SIZE/2], EAST);
  }


  // 7, 8 : SOUTH or WEST
  if (entered_x == SIZE/2 - 1 && entered_y == SIZE/2) {

    set_wall(my_maze->map[SIZE/2][SIZE/2 - 1], SOUTH);
    set_wall(my_maze->map[SIZE/2][SIZE/2 - 1], WEST);
    set_wall(my_maze->map[SIZE/2 - 1][SIZE/2 - 1], NORTH);
    set_wall(my_maze->map[SIZE/2 - 1][SIZE/2 - 1], WEST);
    set_wall(my_maze->map[SIZE/2][SIZE/2], SOUTH);
    set_wall(my_maze->map[SIZE/2][SIZE/2], EAST);
  }

}



/** Function: reflood_from_goal
    Description: Resets all the flood values so that the goal is now the start without modifying wall values.
*/
void reflood_from_goal(void) {
  for (int i = 0; i < SIZE; i++) 
      for (int j = 0; j < SIZE; j++)
        my_maze->map[i][j]->floodval = LARGEVAL;
      
    // set the start value to zero 
    set_value(my_maze->map[START_X][START_Y], 0);

    /* push the neighbors of start cell to stack 
       then pop everything until all cells updated*/
    push_open_neighbors(my_maze->map[START_X][START_Y], my_stack);
    while(!is_empty_Stack(my_stack)) {
      pop(my_stack, &temp);
      if (!(temp->row == 15 && temp->column == 0))
        flood_fill(temp, my_stack, TRUE);
    }
}


