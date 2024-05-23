#include "commons.h"
#include "buzzer.h"

#define minimum_battery_voltage 7

DigitalOut buzzer(buzzer_Pin);     

void buzzer_on(void){
buzzer = HIGH;
}

void buzzer_off(void){
buzzer = LOW;
}
