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

enum states{start,wait,inc,dec,both}state;
unsigned char led = 0x00;
void tick() {
	unsigned char A0 = ~PINA & 0x01;
	unsigned char A1 = ~PINA & 0x02;

	switch(state) {
		case start:
			state = wait;
			break;
		case wait:
			if(A0 && A1) {
				led = 0x00;
				state = both;
			}
			else if(A0 && !A1 && led < 0xFF) {
				++led;
				state = inc;
			}
			else if(!A0 && A1 && led > 0x00) {
				--led;
				state = dec;
			}

			else {
				state = wait;
			}
			break;
		case inc:
			if(A0 && A1) {
				led = 0x00;
			}
			else if(A0){
				state = inc;
			}
			else {
				state = wait;
			}
			break;

		case dec:
			if(A0 && A1) {
				led = 0x00;
			}
			else if (A1) {
				state = dec;
			}
			else {
				state = wait;
			}
			break;
		case both:
			if(A0 && A1) {
				led = 0x00;
				state = both;
			}
			else {
				state = wait;
			}
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
	
	state = start;
    /* Insert your solution below */
    while (1) {
	tick();
    }
    return 1;
}
