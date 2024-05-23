/*************************************************************************/
/******************** Filtering values of MPU  ***************************/
/*************************************************************************/
void setOffset()
{
  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
  for (int i=0; i<2000; i++){
    mpu.getEvent(&a, &g, &temp);
    ax_avg= (ax_avg + a.acceleration.x)/2;
    ay_avg= (ay_avg + a.acceleration.y)/2;
    gz_avg= (gz_avg + g.gyro.z)/2; 
  }
  mpu.setFilterBandwidth(FILTER_DEFAULT);
}

/*************************************************************************/
/************************* MPU Values Update  ****************************/
/*************************************************************************/
void MpuUpdate(){
  mpu.getEvent(&a, &g, &temp);
  time2=millis();
  float ax = a.acceleration.x -ax_avg;
  float ay = a.acceleration.y -ay_avg;
  float gz = a.gyro.y -gz_avg;
  
  if (fabs(ax) <= 0.05) (ax=0);
  if (fabs(ay) <= 0.05) (ay=0);
  if (fabs(gz) <= 0.03) (gz=0);
  
  P.update(time2,ax,ay,g.gyro.z);
}
