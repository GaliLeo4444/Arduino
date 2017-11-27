#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void InitADC()
{
  // Selecciono la referencia de voltaje en Vref=AVcc
  ADMUX |=  (1 << REFS0);
  //ADMUX |= 0x40;
  //ADMUX |= 0b01000000;
  //Selecciono el  prescaller a 128 y habilito el ADC
  ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0) | (1 << ADEN);
}
uint16_t LeerADC(uint8_t Canal_ADC)
{
  //Selecciono el canal del ADC con una mascara para q los primeros cuatro bits queden intactos.
  ADMUX = (ADMUX & 0xF0) | (Canal_ADC & 0x0F);
  //single conversion mode
  ADCSRA |= (1 << ADSC);
  // Espera que el ADC termina la conversion, cuando ADSC es cero
  while ( ADCSRA & (1 << ADSC) );
  return ADC;
}



int main(void)
{
  Serial.begin(9600);
  
  Serial.println("Inicio");
  InitADC();
  while (1)
  {
    _delay_ms(1000);
    Serial.println((int)LeerADC(0));
  }
}


