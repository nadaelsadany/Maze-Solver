
#include "../include/commons.h"
#include "mbed.h"
#include "../include/IR.h"

#define DEBUG

//*

unsigned long last_time = 0;
unsigned long now_time = 0;
unsigned long time_diff = 0;

    bool front_l_tx_fired_before ;  
    bool front_r_tx_fired_before ;
    bool diag_l_tx_fired_before  ;
    bool diag_r_tx_fired_before ;
    bool side_l_tx_fired_before ;
    bool side_r_tx_fired_before ;
    bool front_r_rx_read_before ;
    bool front_l_rx_read_before ;
    bool side_l_rx_read_before ;
    bool side_r_rx_read_before ;
    bool diag_r_rx_read_before ;
    bool diag_l_rx_read_before ;

  //Private function prototype
  DigitalOut front_l_tx(front_l_tx_Pin_No );  
  DigitalOut front_r_tx(front_r_tx_Pin_No) ;  
  DigitalOut diag_l_tx(diag_l_tx_Pin_No )  ;
  DigitalOut diag_r_tx(diag_r_tx_Pin_No )  ;
  DigitalOut side_l_tx(side_l_tx_Pin_No )  ;
  DigitalOut side_r_tx(side_r_tx_Pin_No )  ;
    
  AnalogIn front_l_rx(front_l_rx_Pin_No) ;  
  AnalogIn front_r_rx(front_r_rx_Pin_No) ;  
  AnalogIn  side_l_rx( side_l_rx_Pin_No) ;
  AnalogIn  side_r_rx( side_r_rx_Pin_No) ;
  AnalogIn  diag_l_rx( diag_l_rx_Pin_No) ;
  AnalogIn  diag_r_rx( diag_r_rx_Pin_No) ;


//constructor
//First front LTX diag LTX side 
IR::IR(PinName front_l_tx_Pin, PinName front_r_tx_Pin, PinName diag_l_tx_Pin, PinName diag_r_tx_Pin, PinName side_l_tx_Pin,PinName side_r_tx_Pin ,PinName front_l_rx_Pin, PinName front_r_rx_Pin, PinName side_l_rx_Pin, PinName side_r_rx_Pin, PinName diag_l_rx_Pin, PinName diag_r_rx_Pin)
{
  
   // front_l_tx =front_l_tx_Pin ;  
   // front_r_tx =front_r_tx_Pin ;  
   //  diag_l_tx = diag_l_tx_Pin ;  
   //  diag_r_tx = diag_r_tx_Pin ; 
   //  side_l_tx = side_l_tx_Pin ; 
   //  side_r_tx = side_r_tx_Pin ;   
   
   // front_l_rx =front_l_rx_Pin ;  
   // front_r_rx =front_r_rx_Pin ;  
   //  side_l_rx = side_l_rx_Pin ; 
   //  side_r_rx = side_r_rx_Pin ; 
   //  diag_l_rx = diag_l_rx_Pin ; 
   //  diag_r_rx = diag_r_rx_Pin ; 
//
  






    /*
    pinMode(front_l_tx, OUTPUT);
    pinMode(front_r_tx, OUTPUT);

    pinMode(diag_l_tx, OUTPUT);
    pinMode(diag_r_tx, OUTPUT);

    pinMode(side_l_tx, OUTPUT);
    pinMode(side_r_tx, OUTPUT);

    pinMode( front_r_rx, INPUT );
    pinMode( front_l_rx, INPUT );
    pinMode( side_l_rx, INPUT );
    pinMode( side_r_rx, INPUT );
    pinMode( diag_r_rx, INPUT );
    pinMode( diag_l_rx, INPUT );


/*
     int irArray[3][4]={
	  {front_l_tx,  front_r_tx,  front_l_rx,   front_r_rx},
	  {diag_l_tx,   diag_r_tx,   diag_l_rx,    diag_r_rx},
	  {side_l_tx,   side_r_tx ,  side_l_rx,    side_r_rx}
	};
//*/
}

/*
void IR::reset_params(void){

  now_time = micros();
  last_time = now_time;
  
  front_l_tx_fired_before = false ;  
  front_r_tx_fired_before = false ;
  diag_l_tx_fired_before  = false  ;
  diag_r_tx_fired_before  = false ;
  side_l_tx_fired_before  = false ;
  side_r_tx_fired_before  = false ;
  front_r_rx_read_before  = false ;
  front_l_rx_read_before  = false ;
  side_l_rx_read_before   = false;
  side_r_rx_read_before   = false;
  diag_r_rx_read_before   = false;
  diag_l_rx_read_before   = false;

}
*/

//ARRAY TO STORE LEFT SENSOR values
//int IR_readings[3][2];

void IR::powerUP_Tx(DigitalOut ledPin){
    ledPin = HIGH;
    //digitalWrite(ledPin, HIGH);
}
// SINCE PNP transistors are used
void IR::powerDOWN_Tx(DigitalOut ledPin){
    ledPin = LOW;
}
float IR::get_Rx(AnalogIn rx_pin_value){
  return rx_pin_value.read()*100.0f;
}


float front_left_IR    = 0;
float front_right_IR   = 0; 
float diag_left_IR     = 0;
float diag_right_IR    = 0;
float side_left_IR     = 0;
float side_right_IR    = 0;

void IR::map_IR(void){
  front_left_IR    = IR_readings[0][0];
  front_right_IR   = IR_readings[0][1]; 
  diag_left_IR     = IR_readings[1][0]; 
  diag_right_IR    = IR_readings[1][1]; 
  side_left_IR     = IR_readings[2][0]; 
  side_right_IR    = IR_readings[2][1]; 
}


void IR::fire_and_get(void){
  #ifndef DEBUG //RUN NORMALLY
  //at t=0
  powerUP_Tx(front_l_tx);
  wait_us(60);
  front_left_IR = get_Rx(front_l_rx);
  powerDOWN_Tx(front_l_tx); 
  wait_us(80);

  powerUP_Tx(front_r_tx);
  wait_us(60);
  front_right_IR = get_Rx(front_r_rx);
  powerDOWN_Tx(front_r_tx);
  wait_us(80);

  powerUP_Tx(diag_l_tx);
  wait_us(60);
  diag_left_IR =  get_Rx(diag_l_rx);
  powerDOWN_Tx(diag_l_tx);    
  wait_us(80);

  powerUP_Tx(diag_r_tx);
  wait_us(60);
  diag_right_IR  =  get_Rx(diag_r_rx);
  powerDOWN_Tx(diag_r_tx);
  wait_us(80);

  powerUP_Tx(side_l_tx);
  wait_us(60);
  side_left_IR =  get_Rx(side_l_rx);
  powerDOWN_Tx(side_l_tx);    
  wait_us(80);

  powerUP_Tx(side_r_tx);
  wait_us(60);
  side_right_IR =  get_Rx(side_r_rx);
  powerDOWN_Tx(side_r_tx);    
  wait_us(80);
  #endif
  //map_IR();
  #ifdef DEBUG
  powerDOWN_Tx(front_l_tx);
  powerDOWN_Tx(front_r_tx);
  powerDOWN_Tx(diag_l_tx);
  powerDOWN_Tx(diag_r_tx);    
  //powerDOWN_Tx(side_l_tx);   
  //powerDOWN_Tx(side_r_tx);    

  //powerUP_Tx(front_l_tx);
  //powerUP_Tx(front_r_tx);
  //powerUP_Tx(diag_l_tx);
  //powerUP_Tx(diag_r_tx);    
  powerUP_Tx(side_l_tx);   
  powerUP_Tx(side_r_tx);    
  side_left_IR =  get_Rx(side_l_rx);

  #endif
}

// To begin the run
 void IR::fire_and_read_front_IRs(void){
   powerUP_Tx(front_l_tx);
   powerUP_Tx(front_r_tx);
   powerUP_Tx(diag_l_tx);
   powerUP_Tx(diag_r_tx);    
  
  front_left_IR = get_Rx(front_l_rx);
  //diag_left_IR =  get_Rx(diag_l_rx);
  //diag_right_IR  =  get_Rx(diag_r_rx);
  front_right_IR = get_Rx(front_r_rx);
  side_left_IR =  get_Rx(side_l_rx);
 } 


  void IR::display_IR(void){
  
  //bt.printf(" | f_l %3.3f%% | ",IR_readings[0][0] );
  //bt.printf(" | f_r %3.3f%% | ",IR_readings[0][1] );
  //bt.printf(" | d_l %3.3f%% | ",IR_readings[1][0] );
  //bt.printf(" | d_r %3.3f%% | ",IR_readings[1][1] );
  //bt.printf(" | s_l %3.3f%% | ",IR_readings[2][0] );
  //bt.printf(" | s_r %3.3f%% | \r\n",IR_readings[2][1] );
  

  printf(" | f_l %3.3f%% | ", front_left_IR  );
  printf(" | f_r %3.3f%% | ", front_right_IR  );
  printf(" | d_l %3.3f%% | ", diag_left_IR  );
  printf(" | d_r %3.3f%% | ", diag_right_IR  );
  printf(" | s_l %3.3f%% | ", side_left_IR  );
  printf(" | s_r %3.3f%% | \n\r", side_right_IR  );
  


  //  
  //Serial.print("  front left  ");
  //Serial.print(analogRead(A0));
//
//
  //Serial.print("  front right  ");
  //Serial.print(IR_readings[0][1]);
  //
//
  //Serial.print("  diag left ");
  //Serial.print(IR_readings[1][0]);
//
//
  //Serial.print("  diag right  ");
  //Serial.print(IR_readings[1][1]);
  //
//
  //Serial.print("  side left ");
  //Serial.print(IR_readings[2][0]);
//
//
  //Serial.print("  side right  ");
  //Serial.print(IR_readings[2][1]);
  //
  //Serial.println("");
////*
  }
//*/
IR IR_module( front_l_tx_Pin_No,  front_r_tx_Pin_No,  diag_l_tx_Pin_No,  diag_r_tx_Pin_No,  side_l_tx_Pin_No, side_r_tx_Pin_No , front_l_rx_Pin_No,  front_r_rx_Pin_No,  side_l_rx_Pin_No, side_r_rx_Pin_No,  diag_l_rx_Pin_No,  diag_r_rx_Pin_No);





/*

  now_time = micros();
    

      Serial.print(" time ");
      Serial.print(now_time);

      Serial.print("  last time ");
      Serial.print(last_time);

      
  time_diff = (now_time - last_time);

      Serial.print("  time diff ");
      Serial.print(time_diff);



      Serial.println(" ");

  //firing front sensors
  
  //Fire left at t = 0. //  5 US buffer
  if(time_diff > 5 AND time_diff < 60){
    if(front_l_tx_fired_before == false){
      
      Serial.print("  Fired LTX at ");
      Serial.print(time_diff);
      
      powerUP_Tx(front_l_tx);
      front_l_tx_fired_before =true;
    }
    
  }
  
  //Measure / get rx op
  else if(time_diff > 60 AND time_diff < 140){
    if(front_l_rx_read_before == false){
      
      Serial.print("  Read LRX at ");
      Serial.print(time_diff);
      
      
      IR_readings[0][0] = get_Rx(front_l_rx);
      front_l_rx_read_before = true;
      powerDOWN_Tx(front_l_tx);    
    }
  }
  
  //80US delay

  //Firing right sensors
  else if(time_diff > 140 AND time_diff < 200){
    if(front_r_tx_fired_before == false){
      powerUP_Tx(front_r_tx);
      front_r_tx_fired_before =true;
    }
  }
  else if(time_diff > 200 AND time_diff < 280){
  if(front_r_rx_read_before == false){
      IR_readings[0][1] = get_Rx(front_r_rx);
      front_r_rx_read_before = true;
      powerDOWN_Tx(front_r_tx);
    }
  }


  //Firing diagonals Left 

  else if(time_diff > 280 AND time_diff < 340){
    if(diag_l_tx_fired_before == false){    
      powerUP_Tx(diag_l_tx);
      diag_l_tx_fired_before = true;
    }
  }

  else if(time_diff > 340 AND time_diff < 420){
  if(diag_l_rx_read_before == false){
      IR_readings[1][0] =  get_Rx(diag_l_rx);
      diag_l_rx_read_before = true;
      powerDOWN_Tx(diag_l_tx);    
    }
  }
  
  //Firing diag right

  else if(time_diff > 420 AND time_diff < 480){
        if(diag_r_tx_fired_before == false){
        powerUP_Tx(diag_r_tx);
        diag_r_tx_fired_before = true;
        }
  }

  else if(time_diff > 480 AND time_diff < 560){
    if(diag_r_rx_read_before == false){
      IR_readings[1][1] =  get_Rx(diag_r_rx);
      diag_r_rx_read_before = true;
        powerDOWN_Tx(diag_r_tx);

    }
  }

  
  //Side sensors

  //Firing diagonals Left 

  else if(time_diff > 560 AND time_diff < 620){
     if(side_l_tx_fired_before == false){    
        powerUP_Tx(side_l_tx);
        side_l_tx_fired_before = true; 
     }
  }
  else if(time_diff > 620 AND time_diff < 700){
    if(side_l_rx_read_before == false){
    IR_readings[2][0] =  get_Rx(side_l_rx);
    side_l_rx_read_before = true;
    powerDOWN_Tx(side_l_tx);    
    }
  }
  
  //Firing side right

  else if(time_diff > 700 AND time_diff < 760){
    if(side_r_tx_fired_before == false){
      powerUP_Tx(side_r_tx);
      side_r_tx_fired_before = true;
    }

  }
  else if(time_diff > 760){
    IR_readings[2][1] =  get_Rx(side_r_rx);
      powerDOWN_Tx(side_r_tx);    
    //reset_params();
  }//*/
  



//Create an obj for IR ARRAY

//change this to get the sensor value
  //int sensorReading = readSensor(4, side_l_rx);
  //Serial.println(sensorReading);
  //
 //pinMode(4, OUTPUT );
 //digitalWrite(4, HIGH);
  //*/
