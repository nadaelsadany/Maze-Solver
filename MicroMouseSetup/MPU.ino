/*************************************************************************/
/************************* Gyro Z axis Raw  ******************************/
/*************************************************************************/
inline int16_t gyroZ_raw()
{
  int16_t data = 0;
  Wire.beginTransmission(MPU_ADDRESS); // Orders MPU to communicate to send data
  Wire.write(GYRO_ZOUT_H);             // Sends Register Address to MPU, then the MPU responds.
  Wire.endTransmission(false);         // Doesn't send a stop bit at the end of the transmission

  Wire.requestFrom(MPU_ADDRESS, 2); // Request 2 bytes from MPU. GYRO_ZOUT_L
  // The MPU will send the data in GYRO_ZOUT_H then GYRO_ZOUT_L
  data = Wire.read() << 8 | Wire.read();

  return data;
}

/*************************************************************************/
/******************** Writing on Mpu Registers  **************************/
/*************************************************************************/
void MPU_reg_write(byte address, byte data) // inline means the function is inserted at the call place when compiled,
{                                           //  and not sent to this function address as normal functions.
  Wire.beginTransmission(MPU_ADDRESS);      // Start communication with MPU6050 // MPU=0x68
  Wire.write(address);                      // Talk to the register 6B
  Wire.write(data);                         // Make reset - place a 0 into the 6B register
  Wire.endTransmission(true);               // end the transmission
}

/*************************************************************************/
/*************************** Reading Yaw angle  **************************/
/*************************************************************************/
float readYaw()
{ // gyroZ angle
  milliOld_mpu = milliNew_mpu;
  milliNew_mpu = millis();
  dt_mpu = (milliOld_mpu - milliNew_mpu) / 1000;
  float reading = (float)gyroZ_raw();
  static float yaw = 0;
  reading /= converter;
  yaw += reading * (dt_mpu);
  return yaw * 2;
}

/*************************************************************************/
/***************** Rotating with 90 Degree with MPU  *********************/
/*************************************************************************/
void rotate_90(float yaw1, char dir)
{
  // start rotation in a certian direction
  if (dir == 'l')
  {
   left();
    while (readYaw() <= yaw1 + MPU_TUNE)
    {
      seeVlx(&leftVlx,&centreVlx,&rightVlx);
      Serial.print("left: ");Serial.print(leftVlx);Serial.print("\tcentreVlx: ");Serial.print(centreVlx);Serial.print("\trightVlx: ");Serial.print(rightVlx);
        Serial.print("\tAngle:");Serial.println(readYaw());

    }
  }
  else if (dir == 'r')
  {
    right();
    while (readYaw() >= yaw1 - MPU_TUNE)
    {
      Serial.print("\tAngle:");Serial.println(readYaw());
    }
  }
  // spins until the following condition is false
  
  STOP(); // end rotation
  delay(1000);
}
