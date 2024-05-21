/*
 * pwm_1.c
 *
 * Created: 5/19/2024 4:55:05 PM
 *  Author: lefaj
 */ 

#include "pwm_1.h"
uint8_t y=0;
uint8_t y2=0;
void initPWM1(uint8_t invertido, uint16_t preescaler){
	DDRB |=(1<<DDB1)|(1<<DDB2);
	TCCR1A=0;
	TCCR1B=0;
	
	if (invertido==1){
		TCCR1A |=(1<<COM1A1)|(1<<COM1A0);//high
		TCCR1A|=(1<<COM1B1)|(1<<COM1B0);
		} else{
		TCCR1A |=(1<<COM1A1); //low
		TCCR1A|=(1<<COM1B1);
	}
	
	switch(preescaler){
		case 0:
		TCCR1B |=(1<<CS10);
		break;
		case 8:
		TCCR1B |=(1<<CS11);
		break;
		case 64:
		TCCR1B |=(1<<CS10)|(1<<CS11);
		break;
		case 256:
		TCCR1B |=(1<<CS12);
		break;
		case 1024:
		TCCR1B |=(1<<CS12)|(1<<CS10); //preescaler de 1024 para un periodo máximo de 4 segundos
		break;
		default:
		TCCR1B|=(1<<CS10);
	}
	
	//FAST PWM 8 bits
	TCCR1A |=(1<<WGM11);
	TCCR1B |=(1<<WGM12)|(1<<WGM12);
	
	ICR1=160;
	
	
	
}


void updateDC1(uint8_t prc_duty, uint8_t prc_duty2){
	y=((0.05*prc_duty)+4.66);
	y2=((0.05*prc_duty2)+4.66);
	//OCR1A-->pot4 eyelid derecha
	OCR1A=y;
	//OCR1B-->pot3 eyelid izquierda
	OCR1B=y2;
}



