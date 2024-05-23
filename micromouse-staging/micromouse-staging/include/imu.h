#ifndef IMU_H
#define IMU_H

//You dont include any other Header files in this.
//Function prototypes
void imu_setup(void) ;
void refresh_imu(void) ;
void get_relative_yaw(void) ;
void reset_IMU_bias(void);

//Global vars////
extern double corrected_yaw;
extern float yaw;
extern double imu_calib_factor; 
extern double yaw_offset ;
#endif // !IMU_H