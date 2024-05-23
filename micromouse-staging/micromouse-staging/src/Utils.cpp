
#include "../include/Utils.h"
#include "calibration.h"
#include <string.h>
#include "../include/commons.h"


void ramp_UP_X( double & var, double x){
  if(var <= x){
    ++var;
  }
}
//This function will specifically ramp l/r forward/backward vals to the desired val from current val.
// More specifically 
// 0 to max_turn and max_turn to 0.
/*
void ramp_LX( double &(x_val), void (*ramp_func)(float), bool ramp_dir){
  
  if((ramp_dir == RAMP_DOWN) AND (x_val != 0) ){
  //RAMP DOWN
   --x_val;
  }
  else if((ramp_dir == RAMP_UP) AND (x_val != CALIB_MOTOR_SPEED)){
    //It was zero and is now being added faster
    ++x_val;
  }
  ramp_func(x_val);
  //if()
}
*/
void ramp_X(double &cur_L_vel, void (*L_func)(float), bool ramp_dir){
  if((ramp_dir == RAMP_DOWN) AND (cur_L_vel != 0)){
    --cur_L_vel;
  }
  else if((ramp_dir == RAMP_UP) AND (cur_L_vel != CALIB_MOTOR_SPEED)){
    ++cur_L_vel;
  }
  L_func(cur_L_vel);
}




void ramp_DOWN_X( double & var, double x){
 if (var >= x){
    --var;
 }  
}



void ramp_DOWN_X_RESET( double & var, double x){
 if (var >= x){
    --var;
 }  
}

//#include "../include/multi-serial-command-listener.h"

/*
char myCommand[SCMD_MAX_CMD_LEN+1];

//------------------------------------
// RealTerm or Teraterm config
// 9600 bauds, 8-bit data, no parity
//------------------------------------

//Serial hc05(D1, D0); // PA_2, PA_3 This one does not work because redirected to USBTX, USBRX
//                     //  can be fixed by changing solder bridges                      
//Serial hc052(D10,D2); // PB_6, PA_10 This one works

void commandCallback(char *cmdIn, void *extraContext) {
  strcpy(myCommand, cmdIn);
  // all our commands will be recieved async in commandCallback
  // we don't want to do time consuming things since it could
  // block the reader and allow the uart to overflow so we simply 
  // copy it out in the callback and then process it latter. 
  
  // See data_log one of dependants of this library for example 
  // of using *extraContext
}
 
void bluetooth_setup(void){
  Serial bt(PA_11, PA_12);  // This one works
  Serial pc(USBTX, USBRX); 
 

  pc.baud(9600);
  //hc05.baud(9600);
  //hc052.baud(9600);
  bt.baud(9600);
  struct SCMD *cmdProc = scMake(&bt, commandCallback, NULL)  ;
  pc.printf("Test HC05 Bluetooth Connection !\r\n");

}

void bt_nline(void){
  bt.printf(" \n ");
}



void wait_for_G_press(void){

    Serial.begin(115200);
    while(true) // remain here until told to break

{
  if(Serial.available() > 0) // did something come in?
    if(Serial.read() == 'G') // is that something the char G?
      break;
}
}
*/