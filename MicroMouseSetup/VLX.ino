/*************************************************************************/
/********************* Initialize Vlx Settings  **************************/
/*************************************************************************/
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

/*************************************************************************/
/*********************** Take Vlx Readinga *******************************/
/*************************************************************************/
void seeVlx(int* reading1, int* reading2, int* reading3)
{
  lox1.rangingTest(&measure1, false); // pass in 'true' to get debug data printout!
  lox2.rangingTest(&measure2, false); // pass in 'true' to get debug data printout!
  lox3.rangingTest(&measure3, false); // pass in 'true' to get debug data printout!

  // print sensor one reading
  //Serial.print(F("1: "));
  if (measure1.RangeStatus != 4) {    // if not out of range
    //Serial.print(measure1.RangeMilliMeter);
    *reading1 = measure1.RangeMilliMeter;
  } else {
    //Serial.print(F("Out of range"));
  }

  //Serial.print(F(" "));

  // print sensor two reading
  //Serial.print(F("2: "));
  if (measure2.RangeStatus != 4) {
    //Serial.print(measure2.RangeMilliMeter);
    *reading2 = measure2.RangeMilliMeter;
  } else {
    //Serial.print(F("Out of range"));
  }

  //Serial.print(F(" "));

  // print sensor two reading
  //Serial.print(F("3: "));
  if (measure3.RangeStatus != 4) {    // if not out of range
    //Serial.print(measure3.RangeMilliMeter);
    *reading3 = measure3.RangeMilliMeter;
  } else {
    //Serial.print(F("Out of range"));
  }

  //Serial.println();
}
