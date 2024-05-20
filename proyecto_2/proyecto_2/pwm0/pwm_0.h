/*
 * pwm_0.h
 *
 * Created: 5/19/2024 4:53:01 PM
 *  Author: lefaj
 */ 

#ifndef PWM_0_H_
#define PWM_0_H_

#include <avr/io.h>
#include <stdint.h>

//invertido=1, no invertido =0

//configurarpwm1 modo fast
void initPWM0(uint8_t invertido, uint16_t preescaler);

void updateDC0(uint8_t duty_cycle,uint8_t duty_cycle2); //percentage duty



#endif /* PWM_0_H_ */