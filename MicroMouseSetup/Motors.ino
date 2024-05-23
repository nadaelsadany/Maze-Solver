/*************************************************************************/
/******************************* Motion  *********************************/
/*************************************************************************/
void forward() {
  digitalWrite(MR1, 1);
  digitalWrite(MR2, 0);
  analogWrite(ENR, 90);

  digitalWrite(ML1, 1);
  digitalWrite(ML2, 0);
  analogWrite(ENL, 90);
}

void forward_step() {
  if (pos_l > pos_r) {
    analogWrite(ENL, 80);
    analogWrite(ENR, 50);

    digitalWrite(ML1, 0);
    digitalWrite(ML2, 0);

    digitalWrite(MR1, 1);
    digitalWrite(MR2, 0);
  }
  else if ( pos_l < pos_r) {
    analogWrite(ENL, 80);
    analogWrite(ENR, 50);

    digitalWrite(ML1, 1);
    digitalWrite(ML2, 0);

    digitalWrite(MR1, 0);
    digitalWrite(MR2, 0);
  }
  else {
    analogWrite(ENL, 80);
    analogWrite(ENR, 50);

    digitalWrite(ML1, 1);
    digitalWrite(ML2, 0);

    digitalWrite(MR1, 1);
    digitalWrite(MR2, 0);
  }
}

void backward(void)
{
  digitalWrite(MR1, 0);
  digitalWrite(MR2, 1);
  analogWrite(ENR, 80);

  digitalWrite(ML1, 0);
  digitalWrite(ML2, 1);
  analogWrite(ENL, 80);
}
void left(void)
{
  digitalWrite(MR1, 1);
  digitalWrite(MR2, 0);
  analogWrite(ENR, 60);

  digitalWrite(ML1, 0);
  digitalWrite(ML2, 1);
  analogWrite(ENL, 60);
}
void right(void)
{
  digitalWrite(MR1, 0);
  digitalWrite(MR2, 1);
  analogWrite(ENR, 60);

  digitalWrite(ML1, 1);
  digitalWrite(ML2, 0);
  analogWrite(ENL, 60);
}
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
/********************* Rigth with Angle 90  ******************************/
/*************************************************************************/
void right_90(void)
{
  while (abs(pos_l) < ROTATE_RIGHT_VALUE && abs(pos_r) < ROTATE_RIGHT_VALUE)
  {
    right();
    Serial.print(pos_l);
    Serial.print("  ");
    Serial.println(pos_r);
  }
  STOP();
}

/*************************************************************************/
/********************* Left with Angle 90  ******************************/
/*************************************************************************/
void left_90(void)
{
  while (abs(pos_l) < ROTATE_LEFT_VALUE && abs(pos_r) < ROTATE_LEFT_VALUE)
  {
    left();
    Serial.print(pos_l);
    Serial.print("  ");
    Serial.println(pos_r);
  }
  STOP();
}
