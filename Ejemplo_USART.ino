/*
  Ejemplo de configuración y uso de la UART del micro controlador ATMEGA328p (Arduino UNO)  
 */
#include <avr/interrupt.h>
#include <avr/io.h>

volatile char gLetra;
unsigned char gPalabra [20];
volatile uint8_t i = 0;


void USART_Transmit(unsigned char data){  
  /* Espera por si el buffer de transmición no está vacío */
  while ( !( UCSR0A & (1<<UDRE0) ) );
  /* Coloca el byte en el buffer de transmisión */
  UDR0 = data;
}

/* ------------ ISRs (WDT, USART_Rx, USART_Tx) ------------ */

ISR(USART_RX_vect){
    PINB |= (1 << PINB5); // TOGGLE del puerto PORTB5 (pin 13)
  //Rx interrupt
  if (UCSR0A & ((1<<FE0)|(1<<DOR0))){
    //Error de trama o data overrun
    char basura = UDR0;
    USART_Transmit('*');
  }
  else
  {
    if ((UCSR0A & (1<<RXC0))){
      /* Recepción completa. Leo el dato recibido y lo almaceno en un char */
      gLetra = UDR0;
      if (gLetra == '*' || i == 19){
        // Transmisión de los datos almacenados
        for (int j = i; j > 0; j--){
          USART_Transmit(gPalabra[i-j]);
        }
        i = 0;
        USART_Transmit('\n');
      } else{
        // Guardar datos recibidos en arreglo
        gPalabra[i] = gLetra;
        i++;        
      }       
    }
  }
}


/* ---- FUNCIONES PARA MANEJO DE LA USART ------- */
void USART_Init(){
  cli();
  /* Set baud rate */
  UBRR0H = 0;
  UBRR0L = 103;
  // U2x1 = 1 para Double Speed Mode,   UBRR1L = 207 => BAUDRATE = 9600
  UCSR0A &= ~(1 << U2X0);  
  UCSR0A &= ~(1<<FE0);
  UCSR0A &= ~(1<<DOR0);
  /* Enable receiver and transmitter */
  UCSR0B |= (1 << RXEN0)|(1<<RXCIE0);
  UCSR0B |= (1 << TXEN0);
  UCSR0B &= ~(1 << TXCIE0);
  UCSR0B &= ~(1 << UCSZ02);
  /* Set frame format: 8data, 1stop bit */
  UCSR0C |= (1<<UCSZ00)|(1<<UCSZ01);
  UCSR0C &= ~(1<<USBS0);
  UCSR0C &= ~((1<<UMSEL01)|(1<<UMSEL00)); 
  UCSR0C &= ~((1<<UPM01)|(1<<UPM00));
  
  sei();
}


/* ---------ENDLESS LOOP --------- */
int main (void){
    
  //Inicialización del puerto serie (USART)
  USART_Init(); // Por defecto a 9600 bps
  DDRB |= (1 << DDB5); //PORT B5 como salida (pin 13 Arduino)
 
  while (1){
	  
  }  
}

