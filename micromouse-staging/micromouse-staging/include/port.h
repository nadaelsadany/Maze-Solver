/**
 *   FOR STLINK CONNECTIONS(CN4) to NuCLEO BOARD (left side CN7) 
 *      PIN1 - 3.3 V
 *      PIN2 - PIN15
 *      PIN3 - PIN19
 *      PIN4 - PIN13
 *      PIN5 - PIN14 
 *      PIN6 - PIN31(CN10) (OPTIONAL) 
 *      RX(CN3) - PIN35(CN10)
 *      TX(CN3) - PIN37(CN10)
 * 
 */



#ifndef PORT_H
#define PORT_H

#define OR || 
#define AND &&
#define builtin_led_Pin D13
#define PUSH_BUTTON PC_13 




//Need analog pin
#define battery_Pin PC_4
#define buzzer_Pin PB_13
 
//Left motor Pin_Nos 
#define rme PC_8
#define rmi1 PB_6
#define rmi2 PA_7       

//Right motor Pin_Nos
#define lme PC_9
#define lmi1 PB_12
#define lmi2 PC_7


//IR Pin_NoS will be fed to IR object

#define front_l_tx_Pin_No PB_2
#define front_r_tx_Pin_No PB_4

#define diag_l_tx_Pin_No PB_1
#define diag_r_tx_Pin_No PB_5

#define side_l_tx_Pin_No PB_15
#define side_r_tx_Pin_No PA_10

#define front_l_rx_Pin_No PC_2
#define diag_l_rx_Pin_No PC_0
#define side_l_rx_Pin_No PA_4

#define side_r_rx_Pin_No PB_0
#define diag_r_rx_Pin_No PC_1
#define front_r_rx_Pin_No PC_3

//*/

#endif // !PORT_H
