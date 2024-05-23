#ifndef SPEED_PROFILES_H
#define SPEED_PROFILES_H

#define MAX_W_ACLKWISE -4*M_PI //rads per sec
#define MAX_W_CLKWISE 4*M_PI //rads per sec

//Function proototypes
void const_Speed(void);
void pivot_left(void);
void pivot_right(void);
void curved_turn_left(void);
void curved_turn_right(void);
void motorbreak(void);
void move_single_cell(void);


#endif // !SPEED_PROFILES_H
