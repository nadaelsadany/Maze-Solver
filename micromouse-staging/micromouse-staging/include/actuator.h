#ifndef ACTUATOR_H
#define ACTUATOR_H
#include "../include/commons.h"

void r_forward(float); //PASS PERCENT PWM
void l_forward(float);  //PASS PERCENT PWM

void r_backward(float);//PASS PERCENT PWM
void l_backward(float);//PASS PERCENT PWM


class Motor{
  public:
    Motor(PinName enable,PinName input1,PinName input2); //constructor
    void forward(float);
    void backward(float);

};

extern Motor rightMotor;
extern Motor leftMotor;




#endif //!ACTUATOR_H;
