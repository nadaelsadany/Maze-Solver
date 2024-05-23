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
#define MPU_ADDRESS 0b1101000
// Gyroscope modes
#define pm250 0b00000
#define pm500 0b01000
#define pm1000 0b10000
#define pm2000 0b11000

#define gyroMode pm1000

#if gyroMode == pm2000
#define converter 131
#elif gyroMode == pm1000
#define converter 65.5
#elif gyroMode == pm500
#define converter 32.8
#else
#define converter 16.4
#endif

// needed Registers
#define GYRO_ZOUT_H 0x47
#define GYRO_ZOUT_L 0x48
#define GYRO_CONFIG 0x1B

// Other Registers
#define MPU_POWER 0x6B
//Variables for Mpu Functions
float milliOld_mpu;
float milliNew_mpu;
float dt_mpu;
#define MPU_TUNE  70

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
#define ROTATE_RIGHT_VALUE    140
#define ROTATE_LEFT_VALUE     125
int pos_l = 0;
int ReadingL;
int pos_r = 0;
int ReadingR;
int stepDone = 0;
int stepForward = 0;

/******************************************************************************/
/******************** Algorithm Macros and Variables  *************************/
/******************************************************************************/
#define LEFT_VLX_VALUE  100 //7cm
#define FORWARD_VLX_VALUE  60 //5cm
#define RIGHT_VLX_VALUE  95 //7cm
#define push_button   1 // if =1 .....most left //if =0 .....most right
#define FLAG // the 2ed push button at 0 creat the array ....at 1 make the shortest round (not completed )
char directions[] = {'L', 'B', 'L', 'L', 'B', 'L', 'S', 'B', 'L', 'L', 'L', 'B', 'S', 'B', 'L', 'L', '\0'};
int pathSize = sizeof(directions) / sizeof(char) - 1;

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
  Wire.begin();
  MPU_reg_write(MPU_POWER, 0x00); // MPU Reset
  MPU_reg_write(GYRO_CONFIG, gyroMode);
}

void loop()
{
  if (stepDone == 0)
  {
 forward_step();
  }
  else
  {
    stepDone = 0;
    STOP();
    delay(1000);
  }

  Serial.print(pos_l);
  Serial.print("  ");
  Serial.println(pos_r);
}
