
#include "mbed.h"
#include "commons.h"
#include "multi-serial-command-listener.h"
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

#define DEBUG_VIA_PRINTF
#define DEBUG


char myCommand[SCMD_MAX_CMD_LEN+1];

int char_int =0;
void char_to_int(void){
  char_int = myCommand[0] - '0';
}
Serial bt(PA_11, PA_12);  // This one works
Serial pc(USBTX, USBRX);     // WORKS AFTER CHANGING SOLDER BRIDGES




#ifdef DEBUG
Timer t_global ;
// PRIVATE VARS

static double now = 0;
static double last_time = 0;
#endif // DEBUG

double delT ;  

DigitalOut myled5(LED1);
void toggle_LED2(void){
    myled5 = !myled5;
}

//FUNCTION PROTOTYPES
void commandCallback(char *, void *); 

int main(void) {
  ///*MAIN SETUP BEGINS HERE***********
  //pc.baud(115200);
  //bt.baud(9600);
  startup_phase_1();
  
  /////*MAIN SETUP ENDS HERE*


  #ifdef DEBUG
  #endif // DEBUG     

  while(1) { 
    ///*LOOP CODE BEGINS HERE*

      t_global.start(); //PUT BREAKPOINT HERE
      last_time = t_global.read_us();

    #ifdef DEBUG
    
    //imu_setup();
    //IR_module.fire_and_get();
    //calc_state();
    //refresh_imu();
    //get_relative_yaw();
    //pc.printf("The yaw offset is %lf ,and muly fact is %lf and yaw is %lf and delT is %llf ms \n\r", yaw_offset, imu_calib_factor, corrected_yaw , (delT/1000.0));
     //pc.printf("My current vel is %llf and currrent yaw is %llf w is %llf \n", current_vel,current_yaw,current_w);
    //l_forward(0);
    //r_backward(5);
    //calc_state();
    //feed_enc();
    //pc.printf(" \n\r");
    #endif // DEBUG
    
    now = t_global.read_us();
    t_global.stop();      
    delT = (now - last_time);//1000000.0; //sec //PUT BREAKPOINT HERE 
    //wait_ms(50);
  //printf("Time elapsed is %lf \n", delT/1000000);
    ///*LOOP CODE ENDS HERE*
  }
  return 0;
} 

/*
 // 
   // bt.printf(" | f_l %3.3f%% | ",front_left_IR );
    //buzzer_off();   
  

     r_forward(00);
     l_forward(00);



     //buzzer_on();
           //desired_vel = 10.1;
    //main_controller(desired_vel,0);
    
     //ledtest = 1;
     //disp_enc();
    //printf("The time elapsed is %f \n", t_global.read());
    //read_encoder();
    //refresh_imu();
    //wait(0.5); 
    //pc.printf("right count =\n\r");  
    //buzzer_off();
    //  desired_vel = 5; // 5 m per sec
    //  const_Speed();
    //bt.printf("percentage: %3.3f%% ", rxtest.read()*100.0f);
    //bt.printf("normalized: 0x%04X \r\n", rxtest.read_u16());
    //bt.printf("analog val =  \r \n");
    //rightMotor.backward(100);
    //leftMotor.backward(100);

    //Lenable = 1;
    //Linput1 = 1;
    //Linput2 = 0;

    //r_forward(20);
    //l_backward(20);
 

    //main_controller(10,0);
      //bt.printf("PA_11/PA_12 %d seconds\r\n", i);
      //*bt.printf("front_left_IR value %d \r\n", front_left_IR);
     // myled = !myled;



struct SCMD *cmdProc = scMake(&bt, commandCallback, NULL)  ;
  //pc.printf("Test HC05 Bluetooth Connection !\r\n");
  



  if (myCommand[0] != 0) {     
          pc.printf("Command Recieved =%s\r\n", myCommand);
          bt.printf("\r\nCommand Recieved =%s\r\n", myCommand);
          if (strcmp(myCommand,"clear") == 0) {
            i = 0;
          }
          myCommand[0] = 0; // clear until we recieve the next command
      }        
    

*/
 

//UTILITY FUNCTION
void commandCallback(char *cmdIn, void *extraContext) {
  strcpy(myCommand, cmdIn);
  // all our commands will be recieved async in commandCallback
  // we don't want to do time consuming things since it could
  // block the reader and allow the uart to overflow so we simply 
  // copy it out in the callback and then process it latter. 
  
  // See data_log one of dependants of this library for example 
  // of using *extraContext
}
//*/