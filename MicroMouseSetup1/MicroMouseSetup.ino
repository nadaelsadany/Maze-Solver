/********************************************************************/
/******************** VLX Variables and Pins ************************/
/********************************************************************/
#include "Adafruit_VL53L0X.h"
Adafruit_VL53L0X lox1 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox2 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox3 = Adafruit_VL53L0X();
#define LOX1_ADDRESS 0x30
#define LOX2_ADDRESS 0x31
#define LOX3_ADDRESS 0x32
#define SHT_LOX1 PA8
#define SHT_LOX2 PA10
#define SHT_LOX3 PA9
VL53L0X_RangingMeasurementData_t measure1;
VL53L0X_RangingMeasurementData_t measure2;
VL53L0X_RangingMeasurementData_t measure3;
int leftVlx, centreVlx, rightVlx;

/********************************************************************/
/******************** MPU Variables and Pins ************************/
/********************************************************************/
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include "MPU_biasing.hpp"
#define FILTER_DEFAULT MPU6050_BAND_21_HZ
Adafruit_MPU6050 mpu;
sensors_event_t a, g, temp;
Position P;
float time2;
float ax_avg = 0, ay_avg = 0, gz_avg = 0;


/***********************************************************/
/******************** Motors Pins  *************************/
/***********************************************************/
#define MR1 PA0
#define MR2 PC15
#define ML1 PC14
#define ML2 PC13
#define ENR PA2
#define ENL PA3

/*************************************************************************/
/******************** Encoders Variables and Pins  ***********************/
/*************************************************************************/
#define ENCA PB9 // YELLOW
#define ENCB PB8 // WHITE
#define ENCA1 PB0 // YELLOW
#define ENCB1 PB1 // WHITE
int pos_l = 0;
int ReadingL;
int pos_r = 0;
int ReadingR;

/*************************************************************************/
/**************************** PID Variables  *****************************/
/*************************************************************************/
#include <util/atomic.h> // For the ATOMIC_BLOCK macro
int spd = 60;
int error;
volatile int posi = 0; // specify posi as volatile: https://www.arduino.cc/reference/en/language/variables/variable-scope-qualifiers/volatile/
long prevT = 0;
float eprev = 0;
float eintegral = 0;
// PID
float kp = 1.5;
float ki = 0;
float kd = 0;

/******************************************************************************/
/******************** Algorithm Macros and Variables  *************************/
/******************************************************************************/
#define LEFT_VLX_VALUE  120 //7cm
#define FORWARD_VLX_VALUE  100 //5cm
#define RIGHT_VLX_VALUE  95 //7cm
#define push_button   1 // if =1 .....most left //if =0 .....most right
#define push_button2 // the 2ed push button at 0 creat the array ....at 1 make the shortest round (not completed )
char directions[100]  ;
int pathSize = 0 ;

void setup()
{
  Serial.begin(9600);
  /***********************************************************/
  /******************** Motors Setup  ************************/
  /***********************************************************/
  pinMode( MR1 , OUTPUT);
  pinMode( MR2 , OUTPUT);
  pinMode( ML1 , OUTPUT);
  pinMode( ML2 , OUTPUT);
  pinMode( ENR , OUTPUT);
  pinMode( ENL , OUTPUT);

  /***********************************************************/
  /******************** Encoders Setup  **********************/
  /***********************************************************/
  pinMode(ENCA, INPUT);
  pinMode(ENCB, INPUT);
  pinMode(ENCA1, INPUT);
  pinMode(ENCB1, INPUT);
  attachInterrupt(digitalPinToInterrupt(ENCA), readEncoderR, RISING);
  attachInterrupt(digitalPinToInterrupt(ENCA1), readEncoderL, RISING);

  /***********************************************************/
  /****************       VLX Setup      *********************/
  /***********************************************************/
  pinMode(SHT_LOX1, OUTPUT);
  pinMode(SHT_LOX2, OUTPUT);
  pinMode(SHT_LOX3, OUTPUT);
  digitalWrite(SHT_LOX1, LOW);
  digitalWrite(SHT_LOX2, LOW);
  digitalWrite(SHT_LOX3, LOW);
  setID();

  /***********************************************************/
  /****************       MPU Setup      *********************/
  /***********************************************************/
  pinMode(PB6, INPUT_PULLUP);
  pinMode(PB7, INPUT_PULLUP);
  // Try to initialize!
  mpu.begin();
  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);

  //getting the most accurate DC Value for the filter
  setOffset();
}

void loop()
{
  while (push_button2==0)
  {
    Creating_array ();
  }
  else {
     if (push_button==0)
     {
     SHORTEST_PATH_right();
     }
     else
     {
     SHORTEST_PATH_left();
     }
     Second_trail();

  }
}
