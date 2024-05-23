#include "mbed.h"
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
#include "global.h"

#define FINGER_THOLD 50.0

//Global vars 
bool startup_seq_status = WAITING;

//Private vars

//Private functions
bool check_button_press(void);


DigitalIn push_button(PUSH_BUTTON);


void startup_phase_1(void){
  encoder_init();
  imu_setup();
  self_calib_IMU();
  l_forward(0);
  r_forward(0);
  sEOS_Init();
  while(check_button_press() != (PRESSED) );
  reset_encoders();
  reset_IMU_bias();
  while((front_left_IR < FINGER_THOLD) AND (front_right_IR < FINGER_THOLD)){
    IR_module.fire_and_read_front_IRs();
  }
  wait_ms(500.0);
  startup_seq_status = COMPLETE ;
}


bool check_button_press(void){
    if(push_button == LOW){
        wait_ms(50);
        if(push_button == LOW){
            return PRESSED;
        }
        else{
            return NOT_PRESSED;
        }
    }
    else{
        return NOT_PRESSED;
    }
}

