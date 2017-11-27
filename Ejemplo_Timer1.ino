#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned long incrementMilis;

void InitTimer1(){
	// disable global interrupts
	cli();             
    // initialize Timer1
    TCCR1A = 0;     // set entire TCCR1A register to 0
    TCCR1B = 0;     // same for TCCR1B
    // turn on CTC mode:
    TCCR1B |= (1 << WGM12);
    // Set CS10 bit for no prescaler.  timertick = 62,5 nseg:
	TCCR1B |= (1 << CS10);
    // set compare match register to desired timer count. (15999+1) * 62,5 nseg = 1 mseg
    OCR1A = 15999;
	// Enable timer1 interrupt mask for compare match A
	TIMSK1 |= (1 << OCIE1A);
	// Enable global interrupts
	sei(); 
}

ISR(TIMER1_COMPA_vect){
	// Timer1 interrupt --> timeout
	incrementMilis++;    // c/1 mseg
   
	/* Cuenta la cantidad de ticks hasta 1 seg */
	if (incrementMilis >= 1000){
		PINB |= (1 << PINB5); // TOGGLE del puerto PORTB5 (pin 13)
		incrementMilis = 0;
	}
}

int main(void){
	
    DDRB |= (1 << DDB5); //PORT B5 como salida (pin 13 Arduino) // Reemplaza a pinMode()

    InitTimer1();
    while(1)
    {
		// NADA
    }
}


