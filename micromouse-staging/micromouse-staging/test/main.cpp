/* xej-Nucleo-F401RE-and-HC05-Bluetooth.cpp
  Test Nucleo-F401RE with HC05 Bluetooth adapter
  
  Wanted to use it for upload of telemetry
  type a command and bt port and it will be echoed back
  
  pin-HC05  Pin-MBed
   TX   --- PA_12
   RX   --- PA_11
   +5V  --- +5 on CN6
               Also worked on 3.3V on CN6
   GND  --- GND on CN6
   STATE--- NC - not connected
   EN   --- NC - not connected
  
   If you cycle power on mbed you may need to close 
   and re-open the port connection which
   is pretty easy when using RealTerm.
  
   tested with Honbay® Wireless Bluetooth Host Serial Transceiver Module   
   Draws about 30mA when starting then drops back
   to 10mA when idle with a jump to about 20 mA 
   when transmitting.
   
   ***
  * By Joseph Ellsworth CTO of A2WH
  * Take a look at A2WH.com Producing Water from Air using Solar Energy
  * March-2016 License: https://developer.mbed.org/handbook/MIT-Licence 
  * Please contact us http://a2wh.com for help with custom design projects.
  ***
  
*/
#include "mbed.h"
#include "commons.h"
#include "multi-serial-command-listener.h"
#include "IR.h"
#include "imu.h"
#include "encoder.h"
#include "motor_commons.h"
#include "actuator.h"
#include "buzzer.h"



    myled= !myled;
char myCommand[SCMD_MAX_CMD_LEN+1];

int char_int =0;
void char_to_int(void){
  char_int = myCommand[0] - '0';
}
Serial bt(PA_11, PA_12);  // This one works
Serial pc(PA_2, PA_3);    // WORKS AFTER CHANGING SOLDER BRIDGES

//Timer t_global ; 

//FUNCTION PROTOTYPES
void commandCallback(char *, void *); 

int main(void) {
  ///*MAIN SETUP BEGINS HERE***********

  pc.baud(9600);
  bt.baud(9600);
  int count_time = 0;
  //encoder_init();
  int i = 1;
  struct SCMD *cmdProc = scMake(&bt, commandCallback, NULL)  ;
  pc.printf("Test HC05 Bluetooth Connection !\r\n");
  imu_setup();
  ///*MAIN SETUP ENDS HERE*     
  //t_global.start();
  while(1) { 
    ///*LOOP CODE BEGINS HERE*
    //IR_module.fire_and_get();
    //IR_module.display_IR();
    
   // bt.printf(" | f_l %3.3f%% | ",front_left_IR );
     if(count_time < 100){
      // r_backward(count_time);
      //l_forward(count_time);
      buzzer_off();   
     }
     /*
    else if((count_time < 22)  && (count_time > 20)){
      r_forward(0);
      l_forward(99);
      buzzer_off();   
    }
     else if(count_time < 30 && count_time > 22
     ){
     r_forward(99);
     l_forward(99);
    // motorbreak();
     buzzer_off();
     }*/
     else{
     r_forward(0);
     l_forward(0);

    // motorbreak();
     //buzzer_on();
       
     }
     
     count_time++;
     //ledtest = 1;
     //disp_enc();
//    printf("The time elapsed is %f \n", t_global.read());
    //read_encoder();
    refresh_imu();
    wait(0.5); 
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
      if (myCommand[0] != 0) {     
          pc.printf("Command Recieved =%s\r\n", myCommand);
          bt.printf("\r\nCommand Recieved =%s\r\n", myCommand);
          if (strcmp(myCommand,"clear") == 0) {
            i = 0;
          }
          myCommand[0] = 0; // clear until we recieve the next command
      }        
    ///*LOOP CODE ENDS HERE*
  }
  return 0;
} 


 

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