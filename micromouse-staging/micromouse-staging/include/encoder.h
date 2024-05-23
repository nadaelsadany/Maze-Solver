#ifndef ENCODER_H
#define ENCODER_H

//GLOBAL DEFS 
#define ONE_DEG_YAW_ENC_COUNT 14.269
#define ONE_CM_WHEEL_ENC_COUNT 50.74

void feed_enc(void);
void encoder_init(void);
void reset_encoders(void);


//GLOBAL VARS
extern int64_t L_enc_position;
extern int64_t R_enc_position;


#endif // !1