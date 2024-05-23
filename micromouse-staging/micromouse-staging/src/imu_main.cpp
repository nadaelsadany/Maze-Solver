/* MPU9250 Basic Example Code
 by: Kris Winer
 date: April 1, 2014
 license: Beerware - Use this code however you'd like. If you 
 find it useful you can buy me a beer some time.
 
 Demonstrate basic MPU-9250 functionality including parameterizing the register addresses, initializing the sensor, 
 getting properly scaled accelerometer, gyroscope, and magnetometer data out. Added display functions to 
 allow display to on breadboard monitor. Addition of 9 DoF sensor fusion using open source Madgwick and 
 Mahony filter algorithms. Sketch runs on the 3.3 V 8 MHz Pro Mini and the Teensy 3.1.
 
 SDA and SCL should have external pull-up resistors (to 3.3V).
 10k resistors are on the EMSENSR-9250 breakout board.
 
 Hardware setup:
 MPU9250 Breakout --------- Arduino
 VDD ---------------------- 3.3V
 VDDI --------------------- 3.3V
 SDA ----------------------- A4
 SCL ----------------------- A5
 GND ---------------------- GND
 
 Note: The MPU9250 is an I2C sensor and uses the Arduino Wire library. 
 Because the sensor is not 5V tolerant, we are using a 3.3 V 8 MHz Pro Mini or a 3.3 V Teensy 3.1.
 We have disabled the internal pull-ups used by the Wire library in the Wire.h/twi.c utility file.
 We are also using the 400 kHz fast I2C mode by setting the TWI_FREQ  to 400000L /twi.h utility file.
 */
 
//#include "ST_F401_84MHZ.h" 
//F401_init84 myinit(0);
#include "commons.h"
#include "../lib/MPU9250.h"
#include "imu.h"

//#define DEBUG
// int imu_count = 0;
//#include "N5110.h"

// Using NOKIA 5110 monochrome 84 x 48 pixel display
// pin 9 - Serial clock out (SCLK)
// pin 8 - Serial data out (DIN)
// pin 7 - Data/Command select (D/C)
// pin 5 - LCD chip select (CS)
// pin 6 - LCD reset (RST)
//Adafruit_PCD8544 display = Adafruit_PCD8544(9, 8, 7, 5, 6);

//imu_setup()
//display_imu()
//refresh_imu()

//GLOBAL VARS 
double yaw_offset = 0;
double imu_calib_factor = 0;
double corrected_yaw = 0;

//Private vars
static float sum = 0;
static uint32_t sumCount = 0;
static char buffer[14];
static double yaw_bias_final = 0;


   MPU9250 mpu9250;
   
   Timer t;
  
//int imu_count = 0;

void imu_setup(void){
  
  //Set up I2C
  i2c.frequency(400000);  // use fast (400 kHz) I2C  
  
  bt.printf("CPU SystemCoreClock is %d Hz\r\n", SystemCoreClock);   
  
  t.start();        
  
  uint8_t whoami = mpu9250.readByte(MPU9250_ADDRESS, WHO_AM_I_MPU9250);  // Read WHO_AM_I register for MPU-9250
  bt.printf("I AM 0x%x\n\r", whoami); bt.printf("I SHOULD BE 0x71\n\r");


  // Read the WHO_AM_I register, this is a good test of communication
  
  if (whoami == 0x71) // WHO_AM_I should always be 0x68
  {  
    bt.printf("MPU9250 WHO_AM_I is 0x%x\n\r", whoami);
    bt.printf("MPU9250 is online...\n\r");
    wait(1);
    
    mpu9250.resetMPU9250(); // Reset registers to default in preparation for device calibration
    mpu9250.MPU9250SelfTest(SelfTest); // Start by performing self test and reporting values
    
    bt.printf("x-axis self test: acceleration trim within : %f % of factory value\n\r", SelfTest[0]);  
    bt.printf("y-axis self test: acceleration trim within : %f % of factory value\n\r", SelfTest[1]);  
    bt.printf("z-axis self test: acceleration trim within : %f % of factory value\n\r", SelfTest[2]);  
    bt.printf("x-axis self test: gyration trim within : %f % of factory value\n\r", SelfTest[3]);  
    bt.printf("y-axis self test: gyration trim within : %f % of factory value\n\r", SelfTest[4]);  
    bt.printf("z-axis self test: gyration trim within : %f % of factory value\n\r", SelfTest[5]);  
    mpu9250.calibrateMPU9250(gyroBias, accelBias); // Calibrate gyro and accelerometers, load biases in bias registers  
    bt.printf("x gyro bias = %f\n\r", gyroBias[0]);
    bt.printf("y gyro bias = %f\n\r", gyroBias[1]);
    bt.printf("z gyro bias = %f\n\r", gyroBias[2]);
    bt.printf("x accel bias = %f\n\r", accelBias[0]);
    bt.printf("y accel bias = %f\n\r", accelBias[1]);
    bt.printf("z accel bias = %f\n\r", accelBias[2]);
    wait(2);
    mpu9250.initMPU9250(); 
    bt.printf("MPU9250 initialized for active data mode....\n\r"); // Initialize device for active mode read of acclerometer, gyroscope, and temperature
    mpu9250.initAK8963(magCalibration);
    bt.printf("AK8963 initialized for active data mode....\n\r"); // Initialize device for active mode read of magnetometer
    bt.printf("Accelerometer full-scale range = %f  g\n\r", 2.0f*(float)(1<<Ascale));
    bt.printf("Gyroscope full-scale range = %f  deg/s\n\r", 250.0f*(float)(1<<Gscale));
    if(Mscale == 0) bt.printf("Magnetometer resolution = 14  bits\n\r");
    if(Mscale == 1) bt.printf("Magnetometer resolution = 16  bits\n\r");
    if(Mmode == 2) bt.printf("Magnetometer ODR = 8 Hz\n\r");
    if(Mmode == 6) bt.printf("Magnetometer ODR = 100 Hz\n\r");
    wait(1);
   }
   else
   {
    bt.printf("Could not connect to MPU9250: \n\r");
    bt.printf("%#x \n",  whoami);
 
    while(1) ; // Loop forever if communication doesn't happen
    }

}   
int Countt = 0;
double yawGyro_rads =0;

void refresh_imu(void)
{
    mpu9250.getAres(); // Get accelerometer sensitivity
    mpu9250.getGres(); // Get gyro sensitivity
    mpu9250.getMres(); // Get magnetometer sensitivity
    //bt.printf("Accelerometer sensitivity is %f LSB/g \n\r", 1.0f/aRes);
    //bt.printf("Gyroscope sensitivity is %f LSB/deg/s \n\r", 1.0f/gRes);
    //bt.printf("Magnetometer sensitivity is %f LSB/G \n\r", 1.0f/mRes);
    magbias[0] = +470.;  // User environmental x-axis correction in milliGauss, should be automatically calculated
    magbias[1] = +120.;  // User environmental x-axis correction in milliGauss
    magbias[2] = +125.;  // User environmental x-axis correction in milliGauss



  
  // If intPin goes high, all data registers have new data
  if(mpu9250.readByte(MPU9250_ADDRESS, INT_STATUS) & 0x01) {  // On interrupt, check if data ready interrupt

    mpu9250.readAccelData(accelCount);  // Read the x/y/z adc values   
    // Now we'll calculate the accleration value into actual g's
    ax = (float)accelCount[0]*aRes - accelBias[0];  // get actual g value, this depends on scale being set
    ay = (float)accelCount[1]*aRes - accelBias[1];   
    az = (float)accelCount[2]*aRes - accelBias[2];  
   
    mpu9250.readGyroData(gyroCount);  // Read the x/y/z adc values
    // Calculate the gyro value into actual degrees per second
    gx = (float)gyroCount[0]*gRes - gyroBias[0];  // get actual gyro value, this depends on scale being set
    gy = (float)gyroCount[1]*gRes - gyroBias[1];  
    gz = (float)gyroCount[2]*gRes - gyroBias[2];   
  
    mpu9250.readMagData(magCount);  // Read the x/y/z adc values   
    // Calculate the magnetometer values in milliGauss
    // Include factory calibration per data sheet and user environmental corrections
    mx = (float)magCount[0]*mRes*magCalibration[0] - magbias[0];  // get actual magnetometer value, this depends on scale being set
    my = (float)magCount[1]*mRes*magCalibration[1] - magbias[1];  
    mz = (float)magCount[2]*mRes*magCalibration[2] - magbias[2];   
  }
   
    Now = t.read_us();
    deltat = (float)((Now - lastUpdate)/1000000.0f) ; // set integration time by time elapsed since last filter update
    lastUpdate = Now;
    
    sum += deltat;
    sumCount++;
    
    //    if(lastUpdate - firstUpdate > 10000000.0f) {
    //     beta = 0.04;  // decrease filter gain after stabilized
    //     zeta = 0.015; // increasey bias drift gain after stabilized
    //   }
    
    // Pass gyro rate as rad/s
    //  mpu9250.MadgwickQuaternionUpdate(ax, ay, az, gx*PI/180.0f, gy*PI/180.0f, gz*PI/180.0f,  my,  mx, mz);
    mpu9250.MahonyQuaternionUpdate(ax, ay, az, gx*PI/180.0f, gy*PI/180.0f, gz*PI/180.0f, my, mx, mz);

    // Serial print and/or display at 0.5 s rate independent of data rates
    delt_t = t.read_ms() - imu_count;
    //if (delt_t > 500) { // update LCD once per half-second independent of read rate

    //bt.printf("ax = %f", 1000*ax); 
    //bt.printf(" ay = %f", 1000*ay); 
    //bt.printf(" az = %f  mg\n\r", 1000*az); 

    //bt.printf("gx = %f", gx); 
    //bt.printf(" gy = %f", gy); 
    //bt.printf(" gz = %f  deg/s\n\r", gz); 
    
    //bt.printf("gx = %f", mx); 
    //bt.printf(" gy = %f", my); 
    //bt.printf(" gz = %f  mG\n\r", mz); 
    
    tempCount = mpu9250.readTempData();  // Read the adc values
    temperature = ((float) tempCount) / 333.87f + 21.0f; // Temperature in degrees Centigrade
    //bt.printf(" temperature = %f  C\n\r", temperature); 
    
    //bt.printf("q0 = %f\n\r", q[0]);
    //bt.printf("q1 = %f\n\r", q[1]);
    //bt.printf("q2 = %f\n\r", q[2]);
    //bt.printf("q3 = %f\n\r", q[3]);      
    
/*    lcd.clear();
    lcd.printString("MPU9250", 0, 0);
    lcd.printString("x   y   z", 0, 1);
    sprintf(buffer, "%d %d %d mg", (int)(1000.0f*ax), (int)(1000.0f*ay), (int)(1000.0f*az));
    lcd.printString(buffer, 0, 2);
    sprintf(buffer, "%d %d %d deg/s", (int)gx, (int)gy, (int)gz);
    lcd.printString(buffer, 0, 3);
    sprintf(buffer, "%d %d %d mG", (int)mx, (int)my, (int)mz);
    lcd.printString(buffer, 0, 4); 
 */  
  // Define output variables from updated quaternion---these are Tait-Bryan angles, commonly used in aircraft orientation.
  // In this coordinate system, the positive z-axis is down toward Earth. 
  // Yaw is the angle between Sensor x-axis and Earth magnetic North (or true North if corrected for local declination, looking down on the sensor positive yaw is counterclockwise.
  // Pitch is angle between sensor x-axis and Earth ground plane, toward the Earth is positive, up toward the sky is negative.
  // Roll is angle between sensor y-axis and Earth ground plane, y-axis up is positive roll.
  // These arise from the definition of the homogeneous rotation matrix constructed from quaternions.
  // Tait-Bryan angles as well as Euler angles are non-commutative; that is, the get the correct orientation the rotations must be
  // applied in the correct order which for this configuration is yaw, pitch, and then roll.
  // For more see http://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles which has additional links.
    yaw   = atan2(2.0f * (q[1] * q[2] + q[0] * q[3]), q[0] * q[0] + q[1] * q[1] - q[2] * q[2] - q[3] * q[3]);   

    //yawGyro_rads +=  (gz*(50000) - angleSumFiftyK)/50000;
    pitch = -asin(2.0f * (q[1] * q[3] - q[0] * q[2]));
    roll  = atan2(2.0f * (q[0] * q[1] + q[2] * q[3]), q[0] * q[0] - q[1] * q[1] - q[2] * q[2] + q[3] * q[3]);
    pitch *= 180.0f / PI;
    yaw   *= 180.0f / PI; 
    //yaw   -= 13.8f; // Declination at Danville, California is 13 degrees 48 minutes and 47 seconds on 2014-04-04 (magnetic degs)
    // MUMBAI IT IS 0deg 3mins    
    roll  *= 180.0f / PI;

    //countt
    #ifdef DEBUG
    pc.printf(" i = %d , Yaw: %f , yaw GYRO:%f  \n\r",Countt, yaw, yawGyro_rads);
    #endif // DEBUG
    //bt.printf("average rate = %f\n\r", (float) sumCount/sum);
//    sprintf(buffer, "YPR: %f %f %f", yaw, pitch, roll);
//    lcd.printString(buffer, 0, 4);
//    sprintf(buffer, "rate = %f", (float) sumCount/sum);
//    lcd.printString(buffer, 0, 5);
    
    myled= !myled;
    imu_count = t.read_ms(); 

    if(imu_count > 1<<21) {
        t.start(); // start the timer over again if ~30 minutes has passed
        imu_count = 0;
        deltat= 0;
        lastUpdate = t.read_us();
    }
    sum = 0;
    sumCount = 0; 
  //}

 
}

void get_relative_yaw(void){
  refresh_imu();
  corrected_yaw = (imu_calib_factor*(yaw - yaw_offset)) - yaw_bias_final ;
}

void reset_IMU_bias(void){
  refresh_imu();
  yaw_bias_final = (imu_calib_factor*(yaw - yaw_offset));
}