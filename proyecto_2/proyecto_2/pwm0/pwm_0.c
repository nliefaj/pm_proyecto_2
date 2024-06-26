/*
 * pwm_0.c
 *
 * Created: 5/19/2024 4:54:18 PM
 *  Author: lefaj
 */ 

#include "pwm_0.h"
#include <avr/io.h>
#include <stdint.h>

void initPWM0(uint8_t invertido, uint16_t preescaler){
	DDRD|=(1<<DDD6)|(1<<DDD5);
	
	TCCR0A=0;
	TCCR0B=0;
	
	if (invertido==1){
		TCCR0A|=(1<<COM0A1)|(1<<COM0A0);
		TCCR0A|=(1<<COM0B1)|(1<<COM0B0);
		}else{
		TCCR0A|=(1<<COM0A1);
		TCCR0A|=(1<<COM0B1);
	}
	
	TCCR0A |=(1<<WGM00)|(1<<WGM01);
	//TCCR0B|=(1<<WGM02);
	
	switch (preescaler){
		case 0:
		TCCR0B |=(1<<CS00);
		break;
		case 8:
		TCCR0B |=(1<<CS01);
		break;
		case 64:
		TCCR0B |=(1<<CS00)|(1<<CS01);
		break;
		case 256:
		TCCR0B |=(1<<CS02);
		break;
		case 1024:
		TCCR0B |=(1<<CS00)|(1<<CS02); //preescaler de 1024 para un periodo m�ximo de 4 segundos
		break;
		default:
		TCCR0B|=(1<<CS00);
	}
	
}

void updateDC0(uint8_t duty_cycle,uint8_t duty_cycle2){
	uint8_t a;
	uint8_t a2;
	a=((0.035*duty_cycle)+4.67);
	a2=((0.02*duty_cycle2)+4.67);
	//pot2--> eyeballs-->135�
	OCR0A=a;
	//pot1--> boca-->45�
	OCR0B=a2;
}