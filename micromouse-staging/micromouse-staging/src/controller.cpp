//DOES PID AND STUFF


#include"../include/commons.h"

#include "../include/controller.h"
#include "../include/actuator.h"

#define refresh_rate 0.01 // Since period is 10MS
#define x_Kp_crit (0.78e2)
#define w_Kp_crit (0.78e2)


#define x_Kp (46.8e0) //found stability for 0.78e2
#define x_Ki (39e0)
#define x_Kd (9.75e0)
#define w_Kp (46.8e0)
#define w_Ki (39e0)
#define w_Kd (9.75e0) 

//global variables
double current_x = 0;
double current_vel = 0;
double current_x_acc = 0 ;
double current_yaw = 0 ;
double current_w = 0 ;

//PRIVATE VARS
static double vel_pwm = 0;
static double w_pwm = 0;
 
static double vel_Error = 0;
static double w_Error = 0; 

static double x_Error = 0;
static double ang_Error = 0;

static double leftMotorPWM = 0;
static double rightMotorPWM = 0;



void main_controller(double desired_vel, double desired_w ){

    
    #ifdef DEBUG
    pc.printf("delT is %f\n\r", delT);
    #endif // DEBUG
    //current_vel = x_vel;
    //current_w = ang_w;
    
    vel_Error = desired_vel - current_vel;
    w_Error = desired_w - current_w;

    //now clamped. between -100 to 100 value
    if(leftMotorPWM > 100 || leftMotorPWM<(-100) ||  rightMotorPWM > 100 || rightMotorPWM<(-100)){
      //DO NOTHING aka dont integrate passed outer limits
    }
    else if(((leftMotorPWM<100.0) AND (leftMotorPWM>-100.0)) AND ((rightMotorPWM<100.0) AND (rightMotorPWM>-100.0))){
    //PARAMS WITHIN LIMITS
    x_Error += vel_Error*refresh_rate;//Integrating position.
    ang_Error += w_Error*refresh_rate;//Integrating angle.
    }

     //x_PD controller.
    vel_pwm = x_Kp*vel_Error +  x_Ki*x_Error +  x_Kd*vel_Error/refresh_rate  ;
    w_pwm = w_Kp*w_Error + w_Ki*ang_Error + w_Kd*ang_Error/refresh_rate ;
    
    
    leftMotorPWM = vel_pwm - w_pwm;
    rightMotorPWM = vel_pwm + w_pwm;
    if(leftMotorPWM >0.0)
      l_forward(leftMotorPWM);
    else  //If negative
      l_backward(-leftMotorPWM);
    
    if(rightMotorPWM >0.0)
      r_forward(rightMotorPWM);
    else  //If negative
      r_backward(-rightMotorPWM);
}

void kill_motion(void){
  l_forward(0);
  r_forward(0);
}

/*
now = millis();          
  timeChange = (double)(now - lastTime); //* del't' 
  error = position_Error();
  out_Of_Path_Error_Check();
      if((newRPM <75)&&(newRPM > -75)){
      errSum += (error * timeChange);   //*integral is now assumed to be clamped*
      }
      double dErr = (error - lastErr)/timeChange; //* timeChange derivative*
      newRPM = (Kp * error) + (Ki * errSum) + (Kd * dErr);
      lastErr = error;  //*Remember some variables for next time*
      lastTime = now;
      if(newRPM > 75){
        newRPM = 75;
      }
      if(newRPM < -75){
        newRPM = -75;
      }
      return newRPM;
      */