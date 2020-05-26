/*	Author: agonz250
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
//#include "timer.h"

//TIMER STUFF
///////////////////////////////////////////////////////////////////////
volatile unsigned char TimerFlag = 0; //stuff added 

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn() {
	//AVR timer/counter controller register ....
	TCCR1B = 0x0B;

	//AVR output compare register ....
	OCR1A = 125;

	//AVR timer interrupt mask register
	TIMSK1 = 0x02;

	//initialize avr counter
	TCNT1=0;

	_avr_timer_cntcurr = _avr_timer_M;
	//TimerIRS called every _avr ... milliseconds
	//
	
	//enable global interrupts
	SREG |= 0x80; //0x80: 1000000
}

void TimerOff() {
	TCCR1B = 0x00; //timer off
}

void TimerISR() {
	TimerFlag = 1;
}

//TimerISR() calls this 
ISR(TIMER1_COMPA_vect) {
	//cpu calls when TCNT1 == OCR1
	_avr_timer_cntcurr--;	//counts down to 0

	if (_avr_timer_cntcurr == 0) {
		TimerISR();	//calls ISR that user uses
		_avr_timer_cntcurr = _avr_timer_M;

	}
}

//Set TimerISR() to tick every M ms
void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}
/////////////////////////////////////////////////////////////////////////////////////


/* SM state declarations --- fill in as needed */
typedef enum output_states { OInit, Ooutput } output_states;
typedef enum read_states { RInit, Rread } read_states;

/* shared variables --- fill in as needed */
unsigned char PA;

/* state variables --- do not alter */
output_states output_state;
read_states read_state;

/* SM definitions --- complete each task as a SM */
#include "output.h"
#include "read.h"

/* main function --- do not alter */
int main(void) {
    /* DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;

    // Common period for all tasks set to 100ms
    TimerSet(100);
    TimerOn();

    // init state vars
    output_state = OInit;
    read_state = RInit;

    while (1) {
        Read();
        Output();
        while (!TimerFlag) { }
        TimerFlag = 0;
    }
    return 1;
}
