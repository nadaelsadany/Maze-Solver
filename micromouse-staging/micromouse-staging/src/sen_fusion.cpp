

#include "../include/commons.h"
#include "../include/sen_fusion.h"
#include "../include/encoder.h"
#include "../include/imu.h"



//#define DEBUG
#define refresh_rate 0.01

//Kalman Parameters
#define calc_var_x 1000UL
#define calc_var_vel 1000UL
#define calc_var_x_acc 1000UL
#define calc_var_yaw D_2_RAD(0.05)
#define calc_var_w 1000UL

#define measure_var_l_enc 1000UL
#define measure_var_r_enc 1000UL
#define measure_var_accelX 1000UL
#define measure_var_yawGyro D_2_RAD(0.05)
#define measure_var_yawAccel D_2_RAD(500000)
#define measure_var_yawMag D_2_RAD(1500000)

#define constVel_speed 0UL
#define pivot_w 0UL
#define pivot_turn_Speed 0UL

//private functions 
static void map_yaw_range(void);


//PRIVATE VARS
static double prev_x = 0;
static double prev_yaw = 0;
static const float alpha_yaw_comp = 0.2; // IE 0.6 for encoders 0.4 for IMU


//MAKES SURE MY ANGLES ARE RESTRICTED from -65 to 65 degs.
static void map_yaw_range(void){
     if(corrected_yaw >65){
         corrected_yaw -= 90;
     }
     else if (corrected_yaw >156){
         corrected_yaw -= 180;
     }
     else if (corrected_yaw < -65){
         corrected_yaw += 90;
     }
     else if (corrected_yaw < -156){
         corrected_yaw += 180;
     }
}


void calc_state(void){
    
feed_enc();
current_x = ((L_enc_position + R_enc_position)/(2.0*ONE_CM_WHEEL_ENC_COUNT)); //DIST IN cm
current_vel = (current_x - prev_x)/(refresh_rate); //IN cm/sec callec every 10ms 

//refresh_imu();
//map_yaw_range();
get_relative_yaw();
double enc_diff = L_enc_position - R_enc_position;
double yaw_encoder = enc_diff/ONE_DEG_YAW_ENC_COUNT;
current_yaw = alpha_yaw_comp*yaw_encoder + (1- alpha_yaw_comp)*corrected_yaw ;
current_w = (current_yaw - prev_yaw)/(refresh_rate);  //degs/sec //called every 10ms

#ifdef DEBUG
pc.printf("Current time diff is %lf ", delT);
pc.printf("My current position is %lf ", current_x  );
pc.printf("My current vel is %lf ", current_vel  );
pc.printf("My current yaw is %lf \n\r",   current_yaw  );
pc.printf("My current w is %lf \n\r",   current_w  );

bt.printf("My current vel is %lf ", current_vel  );
bt.printf("My current w is %lf \n\r",   current_w  );

#endif // DEBUG

prev_x = current_x;
prev_yaw = current_yaw;
}
