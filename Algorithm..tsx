/*******************************************************************************************/
/******************** Function to save direction of robot in array  ************************/
/*******************************************************************************************/
void Creating_array ()
{
  seeVlx(&leftVlx, &centreVlx, &rightVlx);
  if (leftVlx < LEFT_VLX_VALUE && centreVlx > FORWARD_VLX_VALUE && rightVlx < RIGHT_VLX_VALUE)//Straight path or using pid
  {
    stepNextCell();
    delay(500);
  }

  if (leftVlx > LEFT_VLX_VALUE && centreVlx < FORWARD_VLX_VALUE && rightVlx < RIGHT_VLX_VALUE)//Left turn or using pid
  {
    left_90();
  }

  if (leftVlx < LEFT_VLX_VALUE && centreVlx < FORWARD_VLX_VALUE && rightVlx > RIGHT_VLX_VALUE)//Right Turn or using pid
  {
    right_90();

  }

  if (leftVlx > LEFT_VLX_VALUE && centreVlx < FORWARD_VLX_VALUE && rightVlx > RIGHT_VLX_VALUE)//T Intersection
  {
    if (push_button == 1) //most left
    {
      left_90(); // As Left is possible
      directions[pathSize] = 'L';
    }
    else //most right
    {
      right_90();
      directions[pathSize] = 'R';
    }

    pathSize++;
  }

  if (leftVlx > LEFT_VLX_VALUE && centreVlx > FORWARD_VLX_VALUE && rightVlx < RIGHT_VLX_VALUE)//Left T Intersection
  {
    if (push_button == 1) //most left
    {
      left_90(); // As Left is possible
      directions[pathSize] = 'L';
    }
    else //most right
    {
      stepNextCell();
      delay(500);//As Straight path is possible
      directions[pathSize] = 'S';

    }
    pathSize++;
  }

  if (leftVlx < LEFT_VLX_VALUE && centreVlx > FORWARD_VLX_VALUE && rightVlx > RIGHT_VLX_VALUE)//Right T Tntersection
  {
    if (push_button == 1) //most left
    {
      stepNextCell();
      delay(500);//As Straight path is possible
      directions[pathSize] = 'S';
    }
    else //most right
    {
      right_90();
      directions[pathSize] = 'R';
    }
    pathSize++;
  }

  if (leftVlx < LEFT_VLX_VALUE && centreVlx < FORWARD_VLX_VALUE && rightVlx < RIGHT_VLX_VALUE)//Dead End
  {
    right_180(); //As no other direction is possible
    directions[pathSize] = 'B';
    pathSize++;
  }

  if (leftVlx > LEFT_VLX_VALUE && centreVlx > FORWARD_VLX_VALUE && rightVlx > RIGHT_VLX_VALUE)//4 Lane intersection
  {
    if (push_button == 1) //most left
    {
      left_90(); // As Left is possible
      directions[pathSize] = 'L';
    }
    else //most right
    {
      right_90();
      directions[pathSize] = 'R';
    }
    pathSize++;
  }
}

/***********************************************************************************************************/
/*********************** Minimzing the array used in the shortest path functions ***************************/
/***********************************************************************************************************/
void minimize_array( int x, int y)
{
  directions[x] = directions[y];
  for (int i = y; i <= pathSize - 2 ; i++)
  {
    directions[i] = directions[i + 2];
  }
  pathSize = pathSize - 2;
}

/*********************************************************************************************************/
/*********************** Optimizing the array to the shortest path (Most Left) ***************************/
/*********************************************************************************************************/
void SHORTEST_PATH_left()
{
  /*
    LBL = S
    LBR = B
    LBS = R
    RBL = B
    SBL = R
    SBS = B */
  directions[pathSize] = '\0';
  pathSize++;
  char ACTION;

  for (int i = 0; i <= pathSize - 2; i++)
  {
    ACTION = directions[i];

    if (ACTION == 'B')
    {
      if (directions[i - 1] == 'L' && directions[i + 1] == 'R')
      {
        directions[i] = 'B';
        minimize_array( i - 1, i);
      }

      if (directions[i - 1] == 'L' && directions[i + 1] == 'S')
      {
        directions[i] = 'R';
        minimize_array( i - 1, i);
      }

      if (directions[i - 1] == 'R' && directions[i + 1] == 'L')
      {
        directions[i] = 'B';
        minimize_array( i - 1, i);
      }

      if (directions[i - 1] == 'S' && directions[i + 1] == 'L')
      {
        directions[i] = 'R';
        minimize_array( i - 1, i);
      }

      if (directions[i - 1] == 'S' && directions[i + 1] == 'S')
      {
        directions[i] = 'B';
        minimize_array( i - 1, i);
      }

      if (directions[i - 1] == 'L' && directions[i + 1] == 'L')
      {
        directions[i] = 'S';
        minimize_array( i - 1, i);
      }
      i = -1;
    }
  }
}

/**********************************************************************************************************/
/*********************** Optimizing the array to the shortest path (Most Right) ***************************/
/**********************************************************************************************************/
void SHORTEST_PATH_right()
{
  /*
    RBR = S
    RBL = B
    RBS = L
    LBR = B
    SBR = L
    SBS = B
  */
  directions[pathSize] = '\0';
  pathSize++;
  char ACTION;

  for (int i = 0; i <= pathSize - 2; i++)
  {
    ACTION = directions[i];

    if (ACTION == 'B')
    {
      if (directions[i - 1] == 'L' && directions[i + 1] == 'R')
      {
        directions[i] = 'B';
        minimize_array( i - 1, i);
      }

      if (directions[i - 1] == 'R' && directions[i + 1] == 'S')
      {
        directions[i] = 'L';
        minimize_array( i - 1, i);
      }

      if (directions[i - 1] == 'R' && directions[i + 1] == 'L')
      {
        directions[i] = 'B';
        minimize_array( i - 1, i);
      }

      if (directions[i - 1] == 'S' && directions[i + 1] == 'R')
      {
        directions[i] = 'L';
        minimize_array( i - 1, i);
      }

      if (directions[i - 1] == 'S' && directions[i + 1] == 'S')
      {
        directions[i] = 'B';
        minimize_array( i - 1, i);
      }

      if (directions[i - 1] == 'R' && directions[i + 1] == 'R')
      {
        directions[i] = 'S';
        minimize_array( i - 1, i);
      }

      i = -1;
    }

  }
}

/*********************************************************************************************************/
/************************************** Run the shortest path ********************************************/
/*********************************************************************************************************/
void Second_trail()
{
  delay(15000); //the delay befor the second trail till take the robot to start again
  seeVlx(&leftVlx, &centreVlx, &rightVlx);
  for (int i = 0; i <= pathSize ;)
  {
    if (leftVlx < LEFT_VLX_VALUE && centreVlx > FORWARD_VLX_VALUE && rightVlx < RIGHT_VLX_VALUE)//Straight path or using pid
    {
      stepNextCell();
      delay(500);
    }

    if (leftVlx > LEFT_VLX_VALUE && centreVlx < FORWARD_VLX_VALUE && rightVlx < RIGHT_VLX_VALUE)//Left turn or using pid
    {
      left_90();
    }

    if (leftVlx < LEFT_VLX_VALUE && centreVlx < FORWARD_VLX_VALUE && rightVlx > RIGHT_VLX_VALUE)//Right Turn or using pid
    {
      right_90();

    }
    if (((leftVlx > LEFT_VLX_VALUE && centreVlx > FORWARD_VLX_VALUE && rightVlx > RIGHT_VLX_VALUE) ||
        (leftVlx < LEFT_VLX_VALUE && centreVlx > FORWARD_VLX_VALUE && rightVlx > RIGHT_VLX_VALUE) ||
        (leftVlx > LEFT_VLX_VALUE && centreVlx > FORWARD_VLX_VALUE && rightVlx < RIGHT_VLX_VALUE) ||
        (leftVlx > LEFT_VLX_VALUE && centreVlx < FORWARD_VLX_VALUE && rightVlx > RIGHT_VLX_VALUE))&&(i<pathSize))
    {
      if (directions[i] == 'R')
      {
        right_90();
        i++;
      }
      if (directions[i] == 'L')
      {
        left_90();
        i++;
      }
      if (directions[i] == 'S')
      {
        stepNextCell();
        delay(500);
        i++;
      }
    }
    else {
      stopModified();
    }

  }
}
