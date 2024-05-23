#include"../include/commons.h"

#include"../include/actuator.h"

short int enable;
short int input1;
short int input2;

#include "mbed.h" 

    PwmOut r_enable(rme);
    //DigitalOut r_enable(rme);
    DigitalOut r_input1(rmi2);
    DigitalOut r_input2(rmi1);

    PwmOut l_enable(lme);
    DigitalOut l_input1(lmi2);
    DigitalOut l_input2(lmi1);


    void r_backward(float percent_PWM){
    r_enable.period_ms(40.0f);     
    r_enable = percent_PWM/100.0f;
    r_input2 = HIGH;
    r_input1 = LOW;
    }

    void r_forward(float percent_PWM){

    r_enable.period_ms(40.0f);     
    r_enable = percent_PWM/100.0f;
    r_input2 = LOW;
    r_input1 = HIGH;
    }


    void l_forward(float percent_PWM){
    
    l_enable.period_ms(40.0f);  
    l_enable = percent_PWM/100.0f;
    l_input1 = LOW;
    l_input2 = HIGH;
    
    
    }


    void l_backward(float percent_PWM){
    
    l_enable.period_ms(40.0f);
    l_enable = percent_PWM/100.0f;
    l_input1 = HIGH;
    l_input2 = LOW;
    }


//Simple actuator file to move motor ahead.
Motor::Motor(PinName enablePinNo,PinName input1PinNo,PinName input2PinNo){ //constructor
    
    PwmOut enable(enablePinNo);
    enable.period_ms(20);
    DigitalOut input1(input1PinNo);
    DigitalOut input2(input2PinNo);

}
 
  void Motor::forward(float percent_PWM){   //moves the bot forward

    enable = percent_PWM;
    input1 = HIGH;
    input2 = LOW;
  
  }


  void Motor::backward(float percent_PWM){  //*moves the bot in reverse useful for hardBrake*
    
    enable = percent_PWM;
    input1 = LOW;
    input2 = HIGH;

  }
//#define DEBUG
#ifdef DEBUG
#endif // DEBUG


/**************END_OF_FILE*****************/

/*
class Motor{
  short int enable;   //the enable pin  
  short int input1;   //the first or vcc of motor
  short int input2;   //the 2nd input or gnd of motor
  //unsigned  int PWM;
  public:
  Motor(short int enablePinNo,short int input1PinNo,short int input2PinNo){ //constructor
     enable = enablePinNo;
     input1 = input1PinNo;
     input2 = input2PinNo;
     }
  void motorSetup(){   //Goes in the setup function
    pinMode(enable, OUTPUT);
    pinMode(input1, OUTPUT);
    pinMode(input2, OUTPUT);
  }
  void forward(unsigned int PWM){   //moves the bot forward
    analogWrite(enable, PWM);
    digitalWrite(input1, HIGH);
    digitalWrite(input2, LOW);
  }
  void backward(unsigned int PWM){  //*moves the bot in reverse useful for hardBrake*
    analogWrite(enable, PWM);
    digitalWrite(input1, LOW);
    digitalWrite(input2, HIGH);
  }
  void brake(unsigned long PWM){    //*stops the bot PWM given if you want to keep any minimum bare value*
    analogWrite(enable, PWM);
    digitalWrite(input1, LOW);
    digitalWrite(input2, LOW);
  }
};

Motor rightMotor(brme,brmi1,brmi2);
Motor leftMotor(blme,blmi1,blmi2);

/**************END_OF_FILE*****************/

