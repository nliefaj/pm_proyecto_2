/*
 * proyecto_2.c
 *
 * Created: 5/19/2024 1:51:04 PM
 * Author : lefaj
 */ 
#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include "pwm1/pwm_1.h"
#include "pwm0/pwm_0.h"
//#include "pwm2/pwm_2.h"

uint8_t modo=0;
uint8_t valor_pc=0;
uint8_t pulsador=0;
uint8_t valor_ADC=0;
uint8_t valor_pot1=0;
uint8_t valor_pot2=0;
uint8_t valor_pot3=0;
uint8_t valor_pot4=0;

uint8_t valor_pot1_esc=0;
uint8_t valor_pot2_esc=0;
uint8_t valor_pot3_esc=0;
uint8_t valor_pot4_esc=0;

//int servos[4];
//int servos_mem[4];
uint8_t posicion=0;
uint8_t switch_pot=0;
uint8_t mem_flag=0;

//CONFIGURACIONES
void setup(void){
	cli();
	DDRC=0b00000000;
	EICRA=0;
	EICRA|=(1<<ISC01);
	EIMSK=0,
	EIMSK|=(1<<INT0);
	PCICR=0;
	PCICR |=(1<<PCIE1);
	PCMSK1|=(1<<PCINT8)|(1<<PCINT9)|(1<<PCINT10)|(1<<PCINT11);
	sei();
}

void init_adc(void){
	ADMUX = 0;
	ADCSRA=0;
	
	// VCC=5V
	ADMUX |=(1<<REFS0);
	
	// Entrada 0 de MUX ADC0 para el prelab entonces no se cambian los bits de mux
	ADMUX |=(1<<ADLAR);
	
	//Encender ADC y  habilitar isr adc
	ADCSRA=0;
	ADCSRA|=(1<<ADEN)|(1<<ADIE);
	
	// preescaler = 16M/128=125k
	ADCSRA|=(1<<ADPS0)|(1<<ADPS1)|(1<<ADPS2);
	
	//no se utilizara el estado B
	ADCSRB=0;
	
	
	//DIDRO=0;
}

void initUART9600(void){
	//configurar pines tx y rx
	DDRD &=~(1<<DDD0);//entrada
	DDRD|=(1<<DDD1);//salida Tx
	
	//configurar registro A modo fast u2x0 en 1
	UCSR0A=0;
	UCSR0A|=(1<<U2X0);
	
	//configurar registro B, habilotar isr rx, bits para habilitar RX y TX
	UCSR0B=0;
	UCSR0B|=(1<<RXCIE0)|(1<<RXEN0)|(1<<TXEN0);
	
	//Configurar registro C<Frame asincrono 8 bits de datos sin pariedad, 1 bit de stop
	UCSR0C=0;
	UCSR0C|=(1<<UCSZ01)|(1<<UCSZ00);
	
	//baudrate = 207 con % de error igual a 0.16% con 9600
	UBRR0=207;
	
}

void writetxtUART(char* texto){
	uint8_t i;
	for (i=0;texto[i]!='\0';i++){
		while(!(UCSR0A&(1<<UDRE0)));//esperar hasta que el udre0 est� en 1
		UDR0=texto[i];//cuando i nulo se acaba
	}
}

//MAIN LOOP

int main(void){	
	CLKPR=(1<<CLKPCE);
	CLKPR|=(1<<CLKPS0);
	cli();
	initUART9600();
	initPWM0(0,1024);
	initPWM1(0,1024);
	//initPWM2(0,1024);
	init_adc();
	setup();
	EECR=0;
    /* Replace with your application code */
    while (1) 
    {	
		ADCSRA|=(1<<ADSC);
		//int servos[4]={valor_pot1,valor_pot2,valor_pot3,valor_pot4};
		if (modo==0){
			//aqu� se pueden mover los pots libremente
			PORTD&=~(1<<PORTD3);
			PORTD&=~(1<<PORTD4);
			updateDC1(valor_pot1,valor_pot2);
			updateDC0(valor_pot3,valor_pot4);
			_delay_ms(10);
		}else if (modo==1){
			//se guardan los valores de los pots a la eeprom (escribir)
			PORTD|=(1<<PORTD4);
			PORTD&=~(1<<PORTD3);
			char* txt=(char*)posicion;
			writetxtUART(txt);
			/*
			switch(posicion){
				case 1:
				eeprom_write_byte((uint8_t*)0,valor_pot1);
				eeprom_write_byte((uint8_t*)1,valor_pot2);
				eeprom_write_byte((uint8_t*)2,valor_pot3);
				eeprom_write_byte((uint8_t*)3,valor_pot4);
				break;
				case 2:
				eeprom_write_byte((uint8_t*)4,valor_pot1);
				eeprom_write_byte((uint8_t*)5,valor_pot2);
				eeprom_write_byte((uint8_t*)6,valor_pot3);
				eeprom_write_byte((uint8_t*)7,valor_pot4);
				break;
				case 3:
				eeprom_write_byte((uint8_t*)8,valor_pot1);
				eeprom_write_byte((uint8_t*)9,valor_pot2);
				eeprom_write_byte((uint8_t*)10,valor_pot3);
				eeprom_write_byte((uint8_t*)11,valor_pot4);
				break;
				case 4:
				eeprom_write_byte((uint8_t*)12,valor_pot1);
				eeprom_write_byte((uint8_t*)13,valor_pot2);
				eeprom_write_byte((uint8_t*)14,valor_pot3);
				eeprom_write_byte((uint8_t*)15,valor_pot4);
				break;
			}*/
		}else if(modo==2){
			//modo leer eeprom
			PORTD|=(1<<PORTD3);
			PORTD&=~(1<<PORTD4);
			/*
			switch(posicion){
				case 1:
					valor_pot1_esc=eeprom_read_byte((const uint8_t*)0);
					valor_pot2_esc=eeprom_read_byte((const uint8_t*)1);
					valor_pot3_esc=eeprom_read_byte((const uint8_t*)2);
					valor_pot4_esc=eeprom_read_byte((const uint8_t*)3);
					break;
				case 2:
					valor_pot1_esc=eeprom_read_byte((const uint8_t*)4);
					valor_pot2_esc=eeprom_read_byte((const uint8_t*)5);
					valor_pot3_esc=eeprom_read_byte((const uint8_t*)6);
					valor_pot4_esc=eeprom_read_byte((const uint8_t*)7);
					break;
				case 3:
					valor_pot1_esc=eeprom_read_byte((const uint8_t*)8);
					valor_pot2_esc=eeprom_read_byte((const uint8_t*)9);
					valor_pot3_esc=eeprom_read_byte((const uint8_t*)10);
					valor_pot4_esc=eeprom_read_byte((const uint8_t*)11);
					break;
				case 4:
					valor_pot1_esc=eeprom_read_byte((const uint8_t*)12);
					valor_pot2_esc=eeprom_read_byte((const uint8_t*)13);
					valor_pot3_esc=eeprom_read_byte((const uint8_t*)14);
					valor_pot4_esc=eeprom_read_byte((const uint8_t*)15);
				break;
			}	
			updateDC1(valor_pot1_esc,valor_pot2_esc);
			updateDC0(valor_pot3_esc,valor_pot4_esc);
			_delay_ms(10);*/
		}else if (modo==3){
			//modo=2, aqu� se conecta con adafruit
			PORTD|=(1<<PORTD3)|(1<<PORTD4);
		}
		
    }
}


//INTERRUPCIONES
ISR(INT0_vect){
	modo++;
	if (modo>=4){
		modo=0;
	}
}

ISR(PCINT1_vect){
	//revisa si es la primera o segunda interrupci�n
	valor_pc=PINC;
	valor_pc=(valor_pc&0b00001111);
	if (pulsador==0){
		if ((modo==1)|(modo==2)){
			if (valor_pc==0b00001110){
				PORTD|=(1<<PORTD7);
				posicion=1;
				pulsador++;
			}else if(valor_pc==0b00001101){
				PORTD&=~(1<<PORTD7);
				posicion=2;
				pulsador++;
			}else if(valor_pc==0b00001011){
				PORTB|=(1<<PORTB0);
				posicion=3;
				pulsador++;
			}else if(valor_pc==0b00000111){
				PORTB&=~(1<<PORTB0);
				posicion=4;
				pulsador++;
			}
		}
		
	}else{
	pulsador=0;}
}

ISR(ADC_vect){
		switch_pot=(ADMUX&0x0F);
		
		if(switch_pot==7){
			valor_pot1=ADCH;
			ADMUX=((ADMUX&0xF0)|6);
			} else if(switch_pot==6){
			valor_pot2=ADCH;
			ADMUX=((ADMUX&0xF0)|5);
			}else if(switch_pot==5){
			valor_pot3=ADCH;
			ADMUX=((ADMUX&0xF0)|4);
			}else{
			valor_pot4=ADCH;
			ADMUX=((ADMUX&0xF0)|7);
		}
		
		ADCSRA|=(1<<ADIF);//apagar bandera
}