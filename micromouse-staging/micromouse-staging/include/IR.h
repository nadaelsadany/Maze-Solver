
#ifndef IR_h
#define IR_h


#include "mbed.h"
//*
class IR{

//	DigitalOut front_l_tx	;
//	DigitalOut front_r_tx	;
//	DigitalOut diag_l_tx	;
//	DigitalOut diag_r_tx	;
//	DigitalOut side_l_tx	;
//	DigitalOut side_r_tx	;
//		
//	AnalogIn front_l_rx		;
//	AnalogIn front_r_rx		;
//	AnalogIn  side_l_rx		;
//	AnalogIn  side_r_rx		;
//	AnalogIn  diag_l_rx		;
//	AnalogIn  diag_r_rx		;
//
//	//DEfined for the class STATIC OR PRIVATE ?
	public:
		IR(PinName front_l_tx_Pin, PinName front_r_tx_Pin, PinName diag_l_tx_Pin, PinName diag_r_tx_Pin, PinName side_l_tx_Pin,PinName side_r_tx_Pin ,PinName front_l_rx_Pin, PinName front_r_rx_Pin, PinName side_l_rx_Pin, PinName side_r_rx_Pin, PinName diag_l_rx_Pin, PinName diag_r_rx_Pin);
		int irArray[3][4];
		float IR_readings[3][2];
		void powerUP_Tx(DigitalOut ledPin);
		void powerDOWN_Tx(DigitalOut ledPin);		
		float get_Rx(AnalogIn rx_Pin);
		void reset_params(void);
		void map_IR(void) ;
		void fire_and_get(void);
		void fire_and_read_front_IRs(void);
		void display_IR(void) ;
//*/
};


extern IR IR_module;
extern int IR_readings[3][2];
void fire_and_read_front_IRs(void);
void display_IMU(void);

// GLOBAL VARS

extern float front_left_IR  ;  
extern float front_right_IR ;  
extern float diag_left_IR   ;  
extern float diag_right_IR  ;  
extern float side_left_IR   ;  
extern float side_right_IR  ;  


//void init_IR(void);
#endif