
#include "Maze.c"
#include "Maze.h"
#include "Stack.c"


#include "commons.h"
#include "IR.h"
#include "imu.h"
#include "encoder.h"
#include "motor_commons.h"
#include "actuator.h"
#include "buzzer.h"
#include "../include/sen_fusion.h"
#include "calibration.h"
#include "sch_isr.h"
#include "startup_seq.h"

#include "path_planner.h"


#define BACKWARD 0
#define FORWARD 1


/**** maze solving variables ****/
struct Maze * my_maze; //maze that keeps track of flood fill values and walls
struct Stack * my_stack; //stack used for flood fill
struct Node * temp; //node used for in-between start->goal, goal->start transition
short found_dest; //flag if goal is reached
short direction; //direction that the mouse is facing
short x, y; //current coordinates of the mouse in the maze



short frontwallflag; //global flag that is set when PID finds a front wall
short noWall;  //global flag to detect scenario with no walls


short goal_x, goal_y; //goal coordinates once found

void floodfill_setup(void){

  /**maze solving setup**/
  my_maze = new_Maze(); //initialize new maze
  my_stack = new_Stack(); //initialize new stack
  x = START_X;
  y = START_Y;
  direction = NORTH;
  found_dest = FALSE;
  
  frontwallflag = 0;
  noWall = 0;

}

//  while the center destination hasn't been reached

void run_floodfill(void){


  while(!found_dest){
    //Serial.println("start"); 
     //readSensor();          // read sensors

     visit_next_node(my_maze,my_stack,x,y,TRUE); //visit node in front to detect LEFT/RIGHTwalls
     visit_node(my_maze,my_stack,x,y,TRUE); //visit current node to detect FRONT walls
     //delay(100);
     
     change_dir(my_maze,&x,&y,&direction); //turns the mouse to face the direction of best path. updates location (x,y)
     //delay(100); 
     move_single_cell();   // move a single cell forward in direction chosen by change_dir
     

     check_goal_reached(&x, &y, &found_dest); //neccesary to know if mouse should stop, will set found_dest flag
     
    // lcd_display_location(); //Debugging: display current location of mouse
    // print_map_serial(my_maze); //Debugging: prints out flood values of the maze to Serial port
  }

  ////redo floodfill

  ////speed runs?

}
