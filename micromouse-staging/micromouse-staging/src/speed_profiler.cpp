// DOES DIRECTION CONTROL WAS BUILT FOR CURVED TURNS

#include"../include/commons.h"

#include "../include/speed_profiler.h"
#include "../include/controller.h"

/*
***Find min operating voltage of a battery.

***30% above that voltage should be my min motor speed OR my motors should not stall/Just stall.

Let the trial run speed be 70% of final/max motor speed.
And only of pivot turns ??
For pivot turn I would be decreasing to 50% and increasing to 90% capacities of their final values.

For curve turns need explicit parameters.

*/
//wSpeed (rads / sec) YEP
//x_Speed (m/s)    YEP



//Global
double desired_vel = 0;
double desired_w = 0 ;
double desired_x_acc = 0 ;

//Local
static double x_Speed = 0 ;
static double yaw_Speed = 0 ;

void const_Speed(void){
    
    if(current_vel < desired_vel){
        current_vel += desired_x_acc*delT;
    }

    x_Speed = desired_vel;
    yaw_Speed = desired_w;
    main_controller(x_Speed,yaw_Speed);
}
void curved_turn_left(void){

}

void curved_turn_right(void){

}

void motorbreak(void){
    kill_motion();
}





//*/