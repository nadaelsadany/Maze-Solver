#include "commons.h"
#include "sch_isr.h"

#include "stm32f4xx.h"

#include "sen_fusion.h"
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

#define NUM_OF_TASKS 3
#define TICK_MS 10.0
#define CALL_AFTER_MS(time_var) time_var/TICK_MS

uint32_t Task_count_G[NUM_OF_TASKS] = {0};

//TASK VECTOR 
// task 0  : pre startup
// task 1  : calc state
// task 2  : end kill_motors.
// task 3  :
// task 4  :
// task 5  :
// task   :
// task   :
// task   :
// task   :
// task   :
// task   :
// task   :


#define CALC_STATE_PERIOD CALL_AFTER_MS(10)
#define PRE_STARTUP_PERIOD CALL_AFTER_MS(10)
#define KILL_MOTOR_PERIOD CALL_AFTER_MS(1000)

// Global Vars
extern bool startup_seq_status ;

//Function prototypes
void sEOS_Update(void);
void toggle_LED(void);



volatile uint32_t ticks;
extern "C" void SysTick_Handler (void)
 {
//   ticks++;
   sEOS_Update();
 }

void sEOS_Init(void){
  
   SysTick_Config((SystemCoreClock / 1000)*TICK_MS);      /* Configure SysTick to generate an interrupt every millisecond */
  
}

bool test_stage_end = false;

void sEOS_Update(void){
    
    // If waiting for STARTUP SEQ TO END
    if(startup_seq_status == WAITING){
        if(++Task_count_G[0] == PRE_STARTUP_PERIOD){
          refresh_imu();
          Task_count_G[0] = 0;
        }    
    }
    
    // Once startup seq ends..
    else if(startup_seq_status == COMPLETE){
        
        if(++Task_count_G[1] == CALC_STATE_PERIOD){
            calc_state();
            if(test_stage_end == false){
                main_controller(0.0,0.0);
            }
            Task_count_G[1] = 0;
        }

        if(++Task_count_G[2] == KILL_MOTOR_PERIOD){
            kill_motion();
            test_stage_end == true;
            Task_count_G[2] = 0;
        }
    }
}

DigitalOut myled3(LED1);
void toggle_LED(void){
    myled3= !myled3;
}
