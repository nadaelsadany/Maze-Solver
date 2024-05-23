#include "commons.h"
#include "imu.h"
#include "buzzer.h"
#include "actuator.h"
#include "encoder.h"
#include "Utils.h"

//do not remove this debug or modify pf statements they slow down to a consirable range.
//#define DEBUG

#define Stable_VARIANCE_YAW 100.0
#define STABLE_BUF_SIZE 150

// GLobal VARS
double CALIB_MOTOR_SPEED = 16.0;

// Static vars 
static float new_angle = 0.0;

//PRIVATE FUNCTIONS 
int variance(int [], int); 
static double left_motor_speed = 0;
static double right_motor_speed = 0;


// PRIVATE FUNCTIONS 
//static int variance(int [], int ) ;

int variance(int a[], int n) 
{ 
    // Compute mean (average of elements) 
    int sum = 0; 
    for (int i = 0; i < n; i++) 
        sum += a[i]; 
    double mean = (double)sum /  
                  (double)n; 
  
    // Compute sum squared  
    // differences with mean. 
    double sqDiff = 0; 
    for (int i = 0; i < n; i++)  
        sqDiff += (a[i] - mean) *  
                  (a[i] - mean); 
    return sqDiff / n; 
} 

void self_calib_IMU(void){
   
    //Check if stable readings ?
    wait(2);
    int value_buffer[STABLE_BUF_SIZE] = {0};
    for (int i = 0; i < STABLE_BUF_SIZE; i++){
        refresh_imu();
        value_buffer[i] = yaw;
    }
    #ifdef DEBUG
    int test_variance = variance(value_buffer, STABLE_BUF_SIZE);
    pc.printf("the current variance is %d", variance(value_buffer, 10));
    #endif // DEBUG
    //Get the variance if stable move on.

    if(variance(value_buffer, STABLE_BUF_SIZE) < Stable_VARIANCE_YAW){
        buzzer_off();
        yaw_offset = yaw;
        bool turn_90degs = false;
        while (turn_90degs != true )
        {
        wait_ms(100);
        feed_enc();
        refresh_imu();    
        //Turn the bot by +90degs.
        if(L_enc_position < 45*ONE_DEG_YAW_ENC_COUNT){
            //ramp_LX(left_motor_speed,l_forward,RAMP_UP);
            ramp_X(left_motor_speed,l_forward,RAMP_UP);

        }

        else if(L_enc_position > 45*ONE_DEG_YAW_ENC_COUNT)
        {
          ramp_X(left_motor_speed,l_forward,RAMP_DOWN);
        }
        if((-R_enc_position) < (45*ONE_DEG_YAW_ENC_COUNT)){
            ramp_X(right_motor_speed,r_backward,RAMP_UP);     
        }
        else if((-R_enc_position) > (45*ONE_DEG_YAW_ENC_COUNT))
        {
            ramp_X(right_motor_speed,r_backward,RAMP_DOWN);
        }
        if ((L_enc_position >= 45*ONE_DEG_YAW_ENC_COUNT) && (R_enc_position <= (-45*ONE_DEG_YAW_ENC_COUNT)))
        {
            turn_90degs = true;
        }
        }
        // Now it has turned 90 degs
        refresh_imu();
        imu_calib_factor = (90)/(yaw - yaw_offset);
        #ifdef DEBUG
        pc.printf("The mult factor is %lf bias is %lf , yaw is %lf", imu_calib_factor,yaw_offset , yaw);
        #endif // DEBUG
        
        
        //Now turn it back to -180 degs
        bool turn_back_180degs = false;
        while (turn_back_180degs != true )
        {   
        wait_ms(75);
        feed_enc();
        refresh_imu();
        if(L_enc_position > (-45*ONE_DEG_YAW_ENC_COUNT)){
            ramp_X(left_motor_speed,l_backward,RAMP_UP);
            //l_backward(CALIB_MOTOR_SPEED);
        }
        else
        {
            ramp_X(left_motor_speed,l_backward,RAMP_DOWN);
        }
        if(R_enc_position < (45*ONE_DEG_YAW_ENC_COUNT)){
            ramp_X(right_motor_speed,r_forward,RAMP_UP);
            //r_forward(CALIB_MOTOR_SPEED);   
        }
        else
        {
            ramp_X(right_motor_speed,r_forward,RAMP_DOWN);
        }
        if ((L_enc_position <= (-45*ONE_DEG_YAW_ENC_COUNT)) && (R_enc_position >= (45*ONE_DEG_YAW_ENC_COUNT)))
        {
            turn_back_180degs = true;
        }

        }
    
        // Has turned backed 180 degs 
        //Recheck if calib factor gives correct readings 
        // ie if calibration is successful 
        refresh_imu();
        new_angle = imu_calib_factor*(yaw - yaw_offset); 
        if ((new_angle < -65) && (new_angle > -115))
        {
            //victory beep
            buzzer_on();
            wait_ms(500);
            buzzer_off();
            wait_ms(500);

            buzzer_on();
            wait_ms(500);
            buzzer_off();
            wait_ms(500);
        
        }
        
        bool turn_back_0degs = false;
        while (turn_back_0degs != true )
        {   
            wait_ms(75);
            feed_enc();
            refresh_imu();
            if(L_enc_position < (-5*ONE_DEG_YAW_ENC_COUNT)){
                ramp_X(left_motor_speed,l_forward,RAMP_UP);
            }
            else if(L_enc_position > (-5*ONE_DEG_YAW_ENC_COUNT))
            {
                ramp_X(left_motor_speed,l_forward,RAMP_DOWN);
            }
            if((-R_enc_position) < (5*ONE_DEG_YAW_ENC_COUNT)){
                ramp_X(right_motor_speed,r_backward,RAMP_UP);   
            }
            else if(((-R_enc_position) < (5*ONE_DEG_YAW_ENC_COUNT)))
            {
                ramp_X(right_motor_speed,r_backward,RAMP_DOWN);
            }
            if ((L_enc_position >= (-5*ONE_DEG_YAW_ENC_COUNT)) && (5*ONE_DEG_YAW_ENC_COUNT))
            {
                turn_back_0degs = true;
                ramp_X(left_motor_speed,l_forward,RAMP_DOWN);
                ramp_X(right_motor_speed,r_backward,RAMP_DOWN);
        
            }

            #ifdef DEBUG
          
            #endif // DEBUG
        }
        
        /*
        bool correct_to_zero = false;
        while (correct_to_zero != true){   
            refresh_imu();
            feed_enc();
            if(L_enc_position > -5*ONE_DEG_YAW_ENC_COUNT){
                ramp_X(left_motor_speed,l_backward,RAMP_UP);
            }
            else if(L_enc_position < -5*ONE_DEG_YAW_ENC_COUNT){
                ramp_X(left_motor_speed,l_backward,RAMP_DOWN);    
            }
            if(R_enc_position < 5*ONE_DEG_YAW_ENC_COUNT){
                ramp_X(right_motor_speed,r_forward,RAMP_UP);
            }
            else if(R_enc_position > 5*ONE_DEG_YAW_ENC_COUNT){
                ramp_X(right_motor_speed,r_forward,RAMP_DOWN);    
            }
            if((L_enc_position < -5*ONE_DEG_YAW_ENC_COUNT) AND (R_enc_position > 5*ONE_DEG_YAW_ENC_COUNT)){
                correct_to_zero = true;
            }
        }        */
        refresh_imu();
        new_angle = imu_calib_factor*(yaw - yaw_offset); 
        if((new_angle < 15) && (new_angle > -15))
        {
            //victory beep
            buzzer_on();
            wait_ms(500);
            buzzer_off();
            wait_ms(500);

            buzzer_on();
            wait_ms(500);
            buzzer_off();
            wait_ms(500);

            l_forward(0);
            r_forward(0);
        }
        else{
            buzzer_on();
            // Manually set it in range.
        }
        while ((new_angle > 15) OR (new_angle < -15)){
                l_forward(0);
                r_forward(0);
                refresh_imu();
                new_angle = imu_calib_factor*(yaw - yaw_offset);
                buzzer_on();
            }
            buzzer_off();
            l_forward(0);
            r_forward(0);

    }
    else
    {
        buzzer_on();
        self_calib_IMU();
    }
    
    
}









