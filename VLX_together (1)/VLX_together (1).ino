//VLX
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
#define left_val 100 //7cm
#define front_val 60 //5cm
#define right_val 95 //7cm
VL53L0X_RangingMeasurementData_t measure1;
VL53L0X_RangingMeasurementData_t measure2;
VL53L0X_RangingMeasurementData_t measure3;

int leftVlx, centreVlx, rightVlx;
void seeVlx(int* reading1, int* reading2, int* reading3);
void setID();
void Creating_array (int leftVlx ,int centreVlx ,int rightVlx);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(SHT_LOX1, OUTPUT);
  pinMode(SHT_LOX2, OUTPUT);
  pinMode(SHT_LOX3, OUTPUT);
  digitalWrite(SHT_LOX1, LOW);
  digitalWrite(SHT_LOX2, LOW);
  digitalWrite(SHT_LOX3, LOW);
  setID();
}
void Creating_array (int leftVlx ,int centreVlx ,int rightVlx)
{

  if (leftVlx < left_val && centreVlx > front_val && rightVlx < right_val)//Straight path or using pid
    {
     Serial.print(F("just forward"));
    }

  if (leftVlx > left_val && centreVlx < front_val && rightVlx < right_val)//Left turn or using pid
    {
     Serial.print(F("just left"));

    }

  if (leftVlx < left_val && centreVlx < front_val && rightVlx > right_val)//Right Turn or using pid
    {
      Serial.print(F("just right"));

    }

  if (leftVlx > left_val && centreVlx < front_val && rightVlx > right_val)//T Intersection
    {
    Serial.print(F("left & right"));
    }

  if (leftVlx > left_val && centreVlx > front_val && rightVlx < right_val)//Left T Intersection
    {
     Serial.print(F("left & forward"));
    }

  if (leftVlx < left_val && centreVlx > front_val && rightVlx > right_val)//Right T Tntersection
    {
     Serial.print(F("forward & right"));
    }

  if (leftVlx < left_val && centreVlx < front_val && rightVlx < right_val)//Dead End
    {
     Serial.print(F("return"));
    }

  if (leftVlx > left_val && centreVlx > front_val && rightVlx > right_val)//4 Lane intersection
    {
     Serial.print(F("left & right & forward"));
    }
}

void loop() {
  // put your main code here, to run repeatedly:
seeVlx(&leftVlx,&centreVlx,&rightVlx);
Creating_array ( leftVlx , centreVlx , rightVlx);
//if (centreVlx < 170 )
//{
//  Serial.println("centre!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
//}
//
//if (leftVlx < 170 )
//{
//  Serial.println("left!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
//}
//
//if (rightVlx < 170 )
//{
//  Serial.println("right!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
//}
}

void setID(void)
{
  // all reset
  digitalWrite(SHT_LOX1, LOW);
  digitalWrite(SHT_LOX2, LOW);
  digitalWrite(SHT_LOX3, LOW);
  delay(10);
  // all unreset
  digitalWrite(SHT_LOX1, HIGH);
  digitalWrite(SHT_LOX2, HIGH);
  digitalWrite(SHT_LOX3, HIGH);
  delay(10);

  // activating LOX1 and reseting LOX2
  digitalWrite(SHT_LOX1, HIGH);
  digitalWrite(SHT_LOX2, LOW);
  digitalWrite(SHT_LOX3, LOW);

  // initing LOX1
  if (!lox1.begin(LOX1_ADDRESS)) {
    Serial.println(F("Failed to boot first VL53L0X"));
    while (1);
  }
  delay(10);

  // activating LOX2
  digitalWrite(SHT_LOX2, HIGH);
  delay(10);

  //initing LOX2
  if (!lox2.begin(LOX2_ADDRESS)) {
    Serial.println(F("Failed to boot second VL53L0X"));
    while (1);
  }

  // activating LOX3
  digitalWrite(SHT_LOX3, HIGH);
  delay(10);

  //initing LOX3
  if (!lox3.begin(LOX3_ADDRESS)) {
    Serial.println(F("Failed to boot third VL53L0X"));
    while (1);
  }
}

void seeVlx(int* reading1, int* reading2, int* reading3)
{
  lox1.rangingTest(&measure1, false); // pass in 'true' to get debug data printout!
  lox2.rangingTest(&measure2, false); // pass in 'true' to get debug data printout!
  lox3.rangingTest(&measure3, false); // pass in 'true' to get debug data printout!

  // print sensor one reading
  Serial.print(F("1: "));
  if (measure1.RangeStatus != 4) {    // if not out of range
    Serial.print(measure1.RangeMilliMeter);
    *reading1 = measure1.RangeMilliMeter;
  } else {
    Serial.print(F("Out of range"));
  }

  Serial.print(F(" "));

  // print sensor two reading
  Serial.print(F("2: "));
  if (measure2.RangeStatus != 4) {
    Serial.print(measure2.RangeMilliMeter);
    *reading2 = measure2.RangeMilliMeter;
  } else {
    Serial.print(F("Out of range"));
  }

  Serial.print(F(" "));

  // print sensor two reading
  Serial.print(F("3: "));
  if (measure3.RangeStatus != 4) {    // if not out of range
    Serial.print(measure3.RangeMilliMeter);
    *reading3 = measure3.RangeMilliMeter;
  } else {
    Serial.print(F("Out of range"));
  }

  Serial.println();
}