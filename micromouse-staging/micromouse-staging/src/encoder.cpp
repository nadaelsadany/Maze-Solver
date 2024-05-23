
#include "../include/commons.h"
#include "../include/encoder.h"


//#define DEBUG

//GLOBAL VARS
int64_t L_enc_position = 0;
int64_t R_enc_position = 0;

// Private vars 
int16_t EncoderPosition_TIM1 = 0;
int16_t EncoderPosition_TIM2 = 0;

static int8_t encoder_1_cycle_count = 0;
static int8_t encoder_2_cycle_count = 0;
static int16_t tim1_buffer[2] = {0}; // These contain values just before and just after overflow
static int16_t tim2_buffer[2] = {0}; // These contain values just before and just after overflow
static int64_t tim1_offset = 0 ; //WCscenario
static int64_t tim2_offset = 0 ; //WCscenario
static bool about_to_overflow_tim1 = FALSE;
static bool about_to_overflow_tim2 = FALSE;


//static Function prototypes 
void EncoderInitialise_TIM2(void);
void ZeroEncoderCount_TIM2();
void ZeroEncoderCount_TIM1();
void EncoderInitialise_TIM1(void);
static void check_overflow_tim1(void);
static void check_overflow_tim2(void);

/* 
    Connections Encoder 2
    PA_0 = Encoder_2 A   A0
    PA_1 = Encoder_2 B   A1
   
    Connections Encoder 1   
    PA_8 = Encoder_1 A   D7
    PA_9 = Encoder_1 B   D8 
*/



void EncoderInitialise_TIM2(void) {
    // configure GPIO PA0 & PA1 as inputs for Encoder
    RCC->AHB1ENR |= 0x00000001;  // Enable clock for GPIOA
 
    GPIOA->MODER   |= GPIO_MODER_MODER0_1 | GPIO_MODER_MODER1_1 ;           //PA0 & PA1 as Alternate Function   /*!< GPIO port mode register,               Address offset: 0x00      */
    GPIOA->OTYPER  |= GPIO_OTYPER_OT_0 | GPIO_OTYPER_OT_1 ;                 //PA0 & PA1 as Inputs               /*!< GPIO port output type register,        Address offset: 0x04      */
    GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR0 | GPIO_OSPEEDER_OSPEEDR1 ;     // Low speed                        /*!< GPIO port output speed register,       Address offset: 0x08      */
    GPIOA->PUPDR   |= GPIO_PUPDR_PUPDR0_1 | GPIO_PUPDR_PUPDR1_1 ;           // Pull Down                        /*!< GPIO port pull-up/pull-down register,  Address offset: 0x0C      */
    GPIOA->AFR[0]  |= 0x00000011 ;                                          //  AF01 for PA0 & PA1              /*!< GPIO alternate function registers,     Address offset: 0x20-0x24 */
    GPIOA->AFR[1]  |= 0x00000000 ;                                          //                                  /*!< GPIO alternate function registers,     Address offset: 0x20-0x24 */
   
    // configure TIM2 as Encoder input
    RCC->APB1ENR |= 0x00000001;  // Enable clock for TIM2
 
    TIM2->CR1   = 0x0001;     // CEN(Counter ENable)='1'     < TIM control register 1
    TIM2->SMCR  = 0x0003;     // SMS='011' (Encoder mode 3)  < TIM slave mode control register
    TIM2->CCMR1 = 0xF1F1;     // CC1S='01' CC2S='01'         < TIM capture/compare mode register 1
    TIM2->CCMR2 = 0x0000;     //                             < TIM capture/compare mode register 2
    TIM2->CCER  = 0x0011;     // CC1P CC2P                   < TIM capture/compare enable register
    TIM2->PSC   = 0x0000;     // Prescaler = (0+1)           < TIM prescaler
    TIM2->ARR   = 0xffffffff; // reload at 0xfffffff         < TIM auto-reload register
  
    TIM2->CNT = 0x0000;  //reset the counter before we use it  
}

void ZeroEncoderCount_TIM2() {
    TIM2->CNT=0 ; //reset count to zero
} 

void ZeroEncoderCount_TIM1() {
    TIM1->CNT=0 ; //reset count to zero
}

void EncoderInitialise_TIM1(void) {
    // configure GPIO PA8 & PA9 aka D7 & D8 as inputs for Encoder
    // Enable clock for GPIOA
    __GPIOA_CLK_ENABLE(); //equivalent from _hal_rcc.h
 
    //stm32f4xx.h 
    GPIOA->MODER   |= GPIO_MODER_MODER8_1 | GPIO_MODER_MODER9_1 ;           //PA8 & PA9 as Alternate Function   /*!< GPIO port mode register,               Address offset: 0x00      */
    GPIOA->OTYPER  |= GPIO_OTYPER_OT_8 | GPIO_OTYPER_OT_9 ;                 //PA8 & PA9 as Inputs               /*!< GPIO port output type register,        Address offset: 0x04      */
    GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR8 | GPIO_OSPEEDER_OSPEEDR9 ;     // Low speed                        /*!< GPIO port output speed register,       Address offset: 0x08      */
    GPIOA->PUPDR   |= GPIO_PUPDR_PUPDR8_1 | GPIO_PUPDR_PUPDR9_1 ;           // Pull Down                        /*!< GPIO port pull-up/pull-down register,  Address offset: 0x0C      */
    GPIOA->AFR[0]  |= 0x00000000 ;                                          //  AF01 for PA8 & PA9              /*!< GPIO alternate function registers,     Address offset: 0x20-0x24 */
    GPIOA->AFR[1]  |= 0x00000011 ;                                          //  bits here refer to gpio8..15    /*!< GPIO alternate function registers,     Address offset: 0x20-0x24 */
   
    // configure TIM2 as Encoder input
    //RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; //0x00000001;  // Enable clock for TIM2
    __TIM1_CLK_ENABLE();
 
    TIM1->CR1   = 0x0001;     // CEN(Counter ENable)='1'     < TIM control register 1
    TIM1->SMCR  = 0x0003;     // SMS='011' (Encoder mode 3)  < TIM slave mode control register
    TIM1->CCMR1 = 0xF1F1;     // CC1S='01' CC2S='01'         < TIM capture/compare mode register 1
    TIM1->CCMR2 = 0x0000;     //                             < TIM capture/compare mode register 2
    TIM1->CCER  = 0x0011;     // CC1P CC2P                   < TIM capture/compare enable register
    TIM1->PSC   = 0x0000;     // Prescaler = (0+1)           < TIM prescaler
    TIM1->ARR   = 0xffff;     // reload at 0xfffffff         < TIM auto-reload register
  
    TIM1->CNT = 0x0000;  //reset the counter before we use it  
}


void encoder_init(void){
    
    EncoderInitialise_TIM2() ;
    EncoderInitialise_TIM1() ;
  
}


static void check_overflow_tim1(void){
    
    if(EncoderPosition_TIM1 > 30000){
        about_to_overflow_tim1 = HIGH;
        tim1_buffer[0] = EncoderPosition_TIM1;
    }
    if((about_to_overflow_tim1 == HIGH) && (EncoderPosition_TIM1 < 0)){
        about_to_overflow_tim1 = LOW;
        tim1_buffer[1] = EncoderPosition_TIM1;
        ++encoder_1_cycle_count;
        int16_t diff_pos = 32767 - tim1_buffer[0];
        int16_t diff_neg = 32768 + tim1_buffer[1];
        tim1_offset = (int64_t)(tim1_buffer[0] + diff_pos + diff_neg);
        ZeroEncoderCount_TIM1();
        EncoderPosition_TIM1 = TIM1->CNT ;
    }    
    L_enc_position = EncoderPosition_TIM1 + encoder_1_cycle_count*tim1_offset;
   
}


static void check_overflow_tim2(void){
    
    if(EncoderPosition_TIM2 > 30000){
        about_to_overflow_tim2 = HIGH;
        tim2_buffer[0] = EncoderPosition_TIM2;
    }
    if((about_to_overflow_tim2 == HIGH) && (EncoderPosition_TIM2 < 0)){
        about_to_overflow_tim2 = LOW;
        tim2_buffer[1] = EncoderPosition_TIM2;//a -ve value 
        ++encoder_2_cycle_count;
        int16_t diff_pos = 32767 - tim2_buffer[0];
        int16_t diff_neg = 32768 + tim2_buffer[1];
        tim2_offset = (int64_t)(tim2_buffer[0] + diff_pos + diff_neg);
        ZeroEncoderCount_TIM2();
        EncoderPosition_TIM2 = TIM2->CNT ;
    }    
    R_enc_position = EncoderPosition_TIM2 + encoder_2_cycle_count*tim2_offset;
   
}

void reset_encoders(void){
    ZeroEncoderCount_TIM1();
    ZeroEncoderCount_TIM2();
    L_enc_position = 0;
    R_enc_position = 0;
}

void feed_enc(void) {
    
        // Print Encoder Quadrature count to debug port every 0.5 seconds
        EncoderPosition_TIM1 = TIM1->CNT ; // Get current position from Encoder
        EncoderPosition_TIM2 = TIM2->CNT ; // Get current position from Encoder
        
       check_overflow_tim1();
       check_overflow_tim2();

        #ifdef DEBUG
        bt.printf("Left ENCA %lli ", L_enc_position );
        bt.printf("Rifht ENCA %lli \n\r",R_enc_position );
        pc.printf("Left ENCA %lli ", L_enc_position );
        pc.printf("Rifht ENCA %lli \n\r",R_enc_position );
        #endif  

}