/*	Author: lab
 *  Partner(s) Name: Kenny
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

void transmit_data(unsigned char data){
	int i;
	for(i = 0; i < 8; ++i){
		PORTC = 0x08;
		PORTC |= ((data >>i) & 0x01);
		PORTC |= 0x02;
	}
	PORTC |= 0x04;
	PORTC = 0x00;
}

enum States{init, wait, inc, dec, comb} state;
unsigned char led = 0x00;
void reg_tick(){
	unsigned char up = ~PINA & 0x01;
	unsigned char down = ~PINA & 0x02;
	switch(state) {
		case init:
			state = wait;
			break;
		case wait:
			if(up && down){led = 0x00; state = comb;}
			else if(up && !down && led < 0xFF){++led; state = inc;}
			else if(!up && down && led > 0x00){--led; state = dec;}
			else{state = wait;}
			break;
		case inc:
			if(up && down){led = 0x00;}
			else if(up){state = inc;}
			else{state = wait;}
			break;
		case dec:
			if(up && down){led = 0x00;}
			else if (down){state = dec;}
			else{state = wait;}
			break;
		case comb:
			if(up && down){led = 0x00; state = comb;}
			else{state = wait;}
			break;
		default:
			break;
	}
	transmit_data(led);
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	
	state = init;
    /* Insert your solution below */
    while (1) {
	reg_tick();
    }
    return 1;
}
