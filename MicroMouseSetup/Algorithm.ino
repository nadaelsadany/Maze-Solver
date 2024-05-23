///***********************************************************************************************************************/
///**********************   The First Function save the direction of the mouse in the maze  ******************************/
///**********************   According to the Dip Switch the Strategy changes:               ******************************/
///**********************                     0 --> Most Right                            ********************************/
///**********************                     1 --> Most Left                             ********************************/
///***********************************************************************************************************************/
//void Creating_array (char MAZE_ARRAY[], int *SIZE_OF_ARRAY, int leftVlx , int centreVlx , int rightVlx)
//{
//  
//  if (leftVlx < LEFT_VLX_VALUE && centreVlx > FORWARD_VLX_VALUE && rightVlx < RIGHT_VLX_VALUE)  //Straight path or using pid
//  {
//    //Forward();
//  }
//
//  if (leftVlx > LEFT_VLX_VALUE && centreVlx < FORWARD_VLX_VALUE && rightVlx < RIGHT_VLX_VALUE)  //Left turn or using pid
//  {
//    left_90();
//  }
//
//  if (leftVlx < LEFT_VLX_VALUE && centreVlx < FORWARD_VLX_VALUE && rightVlx > RIGHT_VLX_VALUE)  //Right Turn or using pid
//  {
//    right_90();
//
//  }
//
//  if (leftVlx > LEFT_VLX_VALUE && centreVlx < FORWARD_VLX_VALUE && rightVlx > RIGHT_VLX_VALUE)  //T Intersection
//  {
//    if (push_button == 1) //most left
//    {
//     // Left(); // As Left is possible
//      MAZE_ARRAY[size] = 'L';
//    }
//    else //most right
//    {
//     // Right();
//      MAZE_ARRAY[size] = 'R';
//
//    }
//
//    size++;
//  }
//
//  if (leftVlx > LEFT_VLX_VALUE && centreVlx > FORWARD_VLX_VALUE && rightVlx < RIGHT_VLX_VALUE)//Left T Intersection
//  {
//    if (push_button == 1) //most left
//    {
//     // Left(); // As Left is possible
//      MAZE_ARRAY[size] = 'L';
//    }
//    else //most right
//    {
//     // Forward();//As Straight path is possible
//      MAZE_ARRAY[size] = 'S';
//
//    }
//    size++;
//  }
//
//  if (leftVlx < LEFT_VLX_VALUE && centreVlx > FORWARD_VLX_VALUE && rightVlx > RIGHT_VLX_VALUE)//Right T Tntersection
//  {
//    if (push_button == 1) //most left
//    {
//    //  Forward();//As Straight path is possible
//      MAZE_ARRAY[size] = 'S';
//    }
//    else //most right
//    {
//    //  Right();
//      MAZE_ARRAY[size] = 'R';
//    }
//    size++;
//  }
//
//  if (leftVlx < LEFT_VLX_VALUE && centreVlx < FORWARD_VLX_VALUE && rightVlx < RIGHT_VLX_VALUE)//Dead End
//  {
//   // Backward(); //As no other direction is possible
//    MAZE_ARRAY[size] = 'B';
//    size++;
//  }
//
//  if (leftVlx > LEFT_VLX_VALUE && centreVlx > FORWARD_VLX_VALUE && rightVlx > RIGHT_VLX_VALUE)//4 Lane intersection
//  {
//    if (push_button == 1) //most left
//    {
//    //  Left(); // As Left is possible
//      MAZE_ARRAY[size] = 'L';
//    }
//    else //most right
//    {
//      //Right();
//      MAZE_ARRAY[size] = 'R';
//
//    }
//    size++;
//  }
//}
//
////just clean nonuseful char from the array after calling shortest path
//void minimize_array(char MAZE_ARRAY[], int *SIZE_OF_ARRAY, int x, int y)
//{
//  MAZE_ARRAY[x] = MAZE_ARRAY[y];
//  for (int i = y; i <= *SIZE_OF_ARRAY - 2 ; i++)
//  {
//    MAZE_ARRAY[i] = MAZE_ARRAY[i + 2];
//  }
//  *SIZE_OF_ARRAY = *SIZE_OF_ARRAY - 2;
//}
//
//
//// creat a shortest path by removing any back
//void SHORTEST_PATH(char MAZE_ARRAY[], int *SIZE_OF_ARRAY)
//{
//  /*
//    LBL = S
//    LBR = B
//    LBS = R
//    RBL = B
//    SBL = R
//    SBS = B */
//
//  char ACTION;
//
//  for (int i = 0; i <= *SIZE_OF_ARRAY - 2; i++)
//  {
//    ACTION = MAZE_ARRAY[i];
//
//    if (ACTION == 'B')
//    {
//      if (MAZE_ARRAY[i - 1] == 'L' && MAZE_ARRAY[i + 1] == 'R')
//      {
//        MAZE_ARRAY[i] = 'B';
//        minimize_array(MAZE_ARRAY, SIZE_OF_ARRAY, i - 1, i);
//      }
//
//      if (MAZE_ARRAY[i - 1] == 'L' && MAZE_ARRAY[i + 1] == 'S')
//      {
//        MAZE_ARRAY[i] = 'R';
//        minimize_array(MAZE_ARRAY, SIZE_OF_ARRAY, i - 1, i);
//      }
//
//      if (MAZE_ARRAY[i - 1] == 'R' && MAZE_ARRAY[i + 1] == 'L')
//      {
//        MAZE_ARRAY[i] = 'B';
//        minimize_array(MAZE_ARRAY, SIZE_OF_ARRAY, i - 1, i);
//      }
//
//      if (MAZE_ARRAY[i - 1] == 'S' && MAZE_ARRAY[i + 1] == 'L')
//      {
//        MAZE_ARRAY[i] = 'R';
//        minimize_array(MAZE_ARRAY, SIZE_OF_ARRAY, i - 1, i);
//      }
//
//      if (MAZE_ARRAY[i - 1] == 'S' && MAZE_ARRAY[i + 1] == 'S')
//      {
//        MAZE_ARRAY[i] = 'B';
//        minimize_array(MAZE_ARRAY, SIZE_OF_ARRAY, i - 1, i);
//      }
//
//      if (MAZE_ARRAY[i - 1] == 'L' && MAZE_ARRAY[i + 1] == 'L')
//      {
//        MAZE_ARRAY[i] = 'S';
//        minimize_array(MAZE_ARRAY, SIZE_OF_ARRAY, i - 1, i);
//      }
//
//      i = -1;
//    }
//
//  }
//}
//
//// the final path and the shortest one we call it back when the flag =1
//
//void Second_trail(char MAZE_ARRAY[], int *SIZE_OF_ARRAY, int leftVlx , int centreVlx , int rightVlx)
//{
//  for (int i = 0; i <= size_array ;)
//  {
//    if (leftVlx < left_val && centreVlx > front_val && rightVlx < right_val)//Straight path or using pid
//    {
////Forward();
//    }
//
//    if (leftVlx > left_val && centreVlx < front_val && rightVlx < right_val)//Left turn or using pid
//    {
//    //  Left();
//
//    }
//
//    if (leftVlx < left_val && centreVlx < front_val && rightVlx > right_val)//Right Turn or using pid
//    {
//     // Right();
//
//    }
//    if ((leftVlx > left_val && centreVlx > front_val && rightVlx > right_val) ||
//        (leftVlx < left_val && centreVlx > front_val && rightVlx > right_val) ||
//        (leftVlx > left_val && centreVlx > front_val && rightVlx < right_val) ||
//        (leftVlx > left_val && centreVlx < front_val && rightVlx > right_val))
//    {
//      if (MAZE_ARRAY[i] == 'R')
//      {
//    //    Right();
//        i++;
//      }
//      if (MAZE_ARRAY[i] == 'L')
//      {
// //       Left();
//        i++;
//      }
//      if (MAZE_ARRAY[i] == 'S')
//      {
////        Forward();
//        i++;
//      }
//    }
//
//
//  }
//
//
//}
