/********************************************************************************/
/******************************* Move Straight  *********************************/
/********************************************************************************/
void forward_step() {
  if (pos_l > pos_r) {
    analogWrite(ENL, spd);
    analogWrite(ENR, spd);

    digitalWrite(ML1, 0);
    digitalWrite(ML2, 0);

    digitalWrite(MR1, 1);
    digitalWrite(MR2, 0);
  }
  else if ( pos_l < pos_r) {
    analogWrite(ENL, spd);
    analogWrite(ENR, spd);

    digitalWrite(ML1, 1);
    digitalWrite(ML2, 0);

    digitalWrite(MR1, 0);
    digitalWrite(MR2, 0);
  }
  else {
    analogWrite(ENL, spd);
    analogWrite(ENR, spd);

    digitalWrite(ML1, 1);
    digitalWrite(ML2, 0);

    digitalWrite(MR1, 1);
    digitalWrite(MR2, 0);
  }
}

/********************************************************************************/
/********************************* Turn Left  ***********************************/
/********************************************************************************/
void left(void)
{
  digitalWrite(MR1, 1);
  digitalWrite(MR2, 0);
  analogWrite(ENR, 60);

  digitalWrite(ML1, 0);
  digitalWrite(ML2, 1);
  analogWrite(ENL, 60);
}

/*********************************************************************************/
/********************************* Turn Right  ***********************************/
/*********************************************************************************/
void right(void)
{
  digitalWrite(MR1, 0);
  digitalWrite(MR2, 1);

  digitalWrite(ML1, 1);
  digitalWrite(ML2, 0);

  analogWrite(ENR, 60);
  analogWrite(ENL, 60);
}

/**********************************************************************************/
/********************************* Stop Motors  ***********************************/
/**********************************************************************************/
void STOP(void)
{
  digitalWrite(MR1, 0);
  digitalWrite(MR2, 0);
  analogWrite(ENR, 0);

  digitalWrite(ML1, 0);
  digitalWrite(ML2, 0);
  analogWrite(ENL, 0);
}

/*************************************************************************/
/****************** Step to center of the cell  **************************/
/*************************************************************************/
void stepNextCell()
{
  while (pos_r != 540)
  {
    forward_step();
    Serial.print(pos_l); Serial.print("  "); Serial.println(pos_r);
  }
  stopModified();
  
  STOP();
  pos_r = 0;
  pos_l = 0;
}


/*************************************************************************/
/********************* Rigth with Angle 90  ******************************/
/*************************************************************************/
void right_90()
{
  P.clearAng();
  pos_r = 0;
  pos_l = 0;
  int step = 0;
  MpuUpdate();
  float flag = P.angZ;
  float Yaw = P.angZ;
  Serial.print("flag=");
  Serial.println(flag);
  Serial.print("yaw out");
  Serial.println(Yaw);


  while (Yaw != 90 ) {
    MpuUpdate();
    Yaw = P.angZ;
    Serial.print("yaw in");
    Serial.println(Yaw);

    if (Yaw < flag + 85) {
      motor_l(step++);
      motor_r(-step);
    }
    else  if (Yaw > flag + 93) {
      motor_l(-step);
      motor_r(step++);
    }
    else {
      stopModified();
      if (Yaw <= 92 || Yaw >= 88) {
        P.clearAng();
        pos_r = 0;
        pos_l = 0;
        break;
      }
    }
    MpuUpdate();
    Yaw = P.angZ;
  }
}
/*************************************************************************/
/********************* Left with Angle 90  ******************************/
/*************************************************************************/
void left_90()
{
  P.clearAng();
  pos_r = 0;
  pos_l = 0;
  int step = 0;
  MpuUpdate();
  float flag = P.angZ;
  float Yaw = P.angZ;
  Serial.print("flag=");
  Serial.println(flag);
  Serial.print("yaw out");
  Serial.println(Yaw);


  while (Yaw != -90 ) {
    MpuUpdate();
    Yaw = P.angZ;
    Serial.print("yaw in");
    Serial.println(Yaw);
    if (Yaw > flag - 85) {
      motor_l(-step);
      motor_r(step++);
    }
    else  if (Yaw < flag - 93) {
      motor_l(step);
      motor_r(-step++);
    }
    else {
      stopModified();
      if (Yaw <= -92 || Yaw >= -88) {
        P.clearAng();
        pos_r = 0;
        pos_l = 0;
        break;
      }
    }
    MpuUpdate();
    Yaw = P.angZ;
  }
}
/*************************************************************************/
/********************* Rigth with Angle 180  *****************************/
/*************************************************************************/
void right_180()
{
  P.clearAng();
  pos_r = 0;
  pos_l = 0;
  int step = 0;
  MpuUpdate();
  float flag = P.angZ;
  float Yaw = P.angZ;
  Serial.print("flag=");
  Serial.println(flag);
  Serial.print("yaw out");
  Serial.println(Yaw);


  while (Yaw != 180 ) {
    MpuUpdate();
    Yaw = P.angZ;
    Serial.print("yaw in");
    Serial.println(Yaw);

    if (Yaw < flag + 175) {
      motor_l(step++);
      motor_r(-step);
    }
    else  if (Yaw > flag + 183) {
      motor_l(-step);
      motor_r(step++);
    }
    else {
      stopModified();
      if (Yaw <= 182 || Yaw >= 178) {
        P.clearAng();
        pos_r = 0;
        pos_l = 0;
        break;
      }
    }
    MpuUpdate();
    Yaw = P.angZ;
  }
}

/******************************************************************************************/
/*********************************** Modify using PID  ************************************/
/******************************************************************************************/
void stopModified()
{
  int stop_atR = pos_r;
  int stop_atL = pos_l;
  while (pos_r > stop_atR || pos_l > stop_atL) 
  {
    if (pos_r > stop_atR)
    {
      motor_r(stop_atR);
    }
    if (pos_l > stop_atL)
    {
      motor_l(stop_atL);
    }
  }
  digitalWrite(ML1, 0);
  digitalWrite(ML2, 0);

  digitalWrite(MR1, 0);
  digitalWrite(MR2, 0);
}
