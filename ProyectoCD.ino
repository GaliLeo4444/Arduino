#include <LiquidCrystal.h>
#include <Keypad.h>
#include <EEPROM.h>
#include <avr/interrupt.h>
#include <avr/io.h>




volatile unsigned long incrementMilis;
const byte COLUMNAS = 4;
const byte FILAS = 4;
char teclas [FILAS] [COLUMNAS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte filasPines[FILAS] = {9,8,7,6};             //define lineas
byte columnasPines[COLUMNAS] = {5,4,3,2};       //define columnas
Keypad miTeclado = Keypad( makeKeymap(teclas), filasPines, columnasPines, FILAS, COLUMNAS );

LiquidCrystal lcd(A0, A1, 10, 11, 12, 13);


char codigoSecreto[6] = {};        // aqui va el codigo secreto. Por defecto {'C','D','2','0','1','7'}
char codigoIngresado[6] = {};
char pulsacion;
byte num_caracter = 0;
byte cursor_D = 4;      // posicion de la clave en el LCD
boolean clave_OK = false;       //Para saber cuando se puede cambiar la clave
byte ledVerde = A2;   // pin para el LED verde
byte ledRojo = A3;    // pin para el LED rojo
byte buzzer = A4;     // pin altavoz




//Inicializacion USART
void InitUSART(){
  cli();
  /* Set baud rate */
  UBRR0H = 0;
  UBRR0L = 103;
  // U2x1 = 1 para Double Speed Mode,   UBRR1L = 207 => BAUDRATE = 9600
  UCSR0A &= ~(1 << U2X0);   //Síncrono??
  UCSR0A &= ~(1<<FE0);
  UCSR0A &= ~(1<<DOR0);
  /* Enable receiver and transmitter */
  UCSR0B |= (1 << RXEN0)|(1<<RXCIE0);
  UCSR0B |= (1 << TXEN0);
  UCSR0B &= ~(1 << TXCIE0);
  UCSR0B &= ~(1 << UCSZ02);
  /* Set frame format: 8data, 1stop bit */
  UCSR0C |= (1<<UCSZ00)|(1<<UCSZ01);    //8 bit de data
  UCSR0C &= ~(1<<USBS0);    //1stop bit
  UCSR0C &= ~((1<<UMSEL01)|(1<<UMSEL00));   //Asíncrono
  UCSR0C &= ~((1<<UPM01)|(1<<UPM00));   //Paridad disabled
  sei();
}


void USART_Transmit(unsigned char data){  
  /* Espera por si el buffer de transmición no está vacío */
  while ( !( UCSR0A & (1<<UDRE0) ) );
  /* Coloca el byte en el buffer de transmisión */
  UDR0 = data;
}


ISR(USART_RX_vect){
  //Rx interrupt
  if (UCSR0A & ((1<<FE0)|(1<<DOR0))){   //Error de trama o data overrun
    char basura = UDR0;
    USART_Transmit('?');
  }
  else {
    if ((UCSR0A & (1<<RXC0))){    // Recepción completa. Leo el dato recibido y lo almaceno en un char
      pulsacion = UDR0;
    }
  }
}




void InitTimer1 () {
  // disable global interrupts
  cli();             
    // Initialize Timer1
    TCCR1A = 0;     // Set entire TCCR1A register to 0
    TCCR1B = 0;     // same for TCCR1B
    // Turn on CTC mode:
    TCCR1B |= (1 << WGM12);
    // Timertick = 62,5 nseg:
    // Set compare match register to desired timer count. (15999+1) * 62,5 nseg = 1 mseg
    OCR1A = 15999;
  // Enable timer1 interrupt mask for compare match A
  TIMSK1 |= (1 << OCIE1A);
  // Enable global interrupts
  sei(); 
}

ISR (TIMER1_COMPA_vect) {
  // Timer1 interrupt --> timeout
  incrementMilis++;    // c/1 mseg
  /* Cuenta la cantidad de ticks hasta 1 seg */
}

void Pausa (unsigned int tiempo) {
  increment_mseg = 0;
  TCCR1B |= (1 << CS10);          // Set CS10 bit for no prescaler (clk/1)
  while (increment_mseg < tiempo){
  }
  TCCR1B &= ~(1 << CS10);         //Timer/Counter stopped
}




void initTimer2 () {
  //Fast PWM Mode & OCRA top & Toggle OC2A(PB[3]) on Compare Match
  TCCR2A = 0;
  TCCR2B = 0;
  TCCR2A |= (1 << COM2A0) | (1 << WGM21) | (1 << WGM20);
  TCCR2B |= (1 << WGM22);
}


void GenerarTono (unsigned int tono, unsigned int tiempo) {
  //Output OCR2A duty cycle: 50% siempre
  // F.tono = F.clock/F.Preescaler/(OCR2A+1)/2
  // F.tono = 16MHz/1024/(OCR2A+1)/2
  //OCR2A+1 = 16MHz/(tono*2048)
  OCR2A = byte((7812/tono)+1);
  TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20);    //Prescaler
  Pausa(tiempo);
  TCCR2B &= ~(1 << CS22) | ~(1 << CS21) | ~(1 << CS20);  //Timer/Counter stopped
}




boolean comprobarPass(){
  boolean check = true;
  byte pos = 1;
  while (check && (pos < 7)){
    if (codigoIngresado[6] == codigoSecreto[6]){
        pos++;
    }
    else{
         check = false;
    }
  }
  return check;
}


void CambiarPass()
{
  cursor_D = 4;
  lcd.clear();
  lcd.print("Ing. nuevo pass"); // escribimos en LCD
  lcd.setCursor(4, 1);
  lcd.print(">");
  for (int i=1; i <= 6; i++){
      pulsacion = miTeclado.getKey();
      while (!pulsacion){
      }  
      codigoSecreto[i] = pulsacion;
      EEPROM.write(i, pulsacion);
      cursor_D++;
      lcd.setCursor(cursor_D, 1);
      lcd.print(pulsacion);
      tone(buzzer, 350, 200);
  }
  lcd.setCursor(0, 0);
  lcd.print("Nueva Clave!!!!");
  tone(buzzer, 500, 100);
  tone(buzzer, 600, 100);
  tone(buzzer, 800, 100);
  delay(4000);
}




void setup()
{
  InitUSART();
  InitTimer1();
  initTimer2();
  pinMode(A0, OUTPUT);
  digitalWrite(A0, HIGH);
  pinMode(A1, OUTPUT);
  digitalWrite(A1, HIGH);
  pinMode (ledVerde, OUTPUT);
  digitalWrite(ledVerde, LOW); 
  pinMode (ledRojo, OUTPUT);
  digitalWrite(ledRojo, HIGH); 
  pinMode (buzzer, OUTPUT);
  //digitalWrite(buzzer, HIGH);
  miTeclado.setDebounceTime(200);      //Para evitar rebote de tecla
  for (int i=1; i <= 6; i++){
      codigoSecreto[i] = EEPROM.read(i);    //Busca el pass desde la EEPROM
  }
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Introduzca clave");
  lcd.setCursor(cursor_D, 1);
  lcd.print(">");
  GenerarTono(800, 400);
}




void loop()
{
  pulsacion = miTeclado.getKey();
  if (pulsacion){
      num_caracter++;
      cursor_D++;               // incrementamos el cursor
      lcd.setCursor(cursor_D, 1);
      lcd.print(pulsacion);     // imprimimos el caracter ingresado
      tone(buzzer, 350, 200);        // tono de pulsacion
      if (num_caracter == 6){
         if (comprobarPass()){
              lcd.clear();
              lcd.print("Clave correcta");
              tone(buzzer, 500, 100);
              tone(buzzer, 600, 100);
              tone(buzzer, 800, 100);
              delay(1000);
              lcd.clear();
              lcd.print("Bienvenido!!!!");
              digitalWrite(ledRojo, LOW);     //Apagamos el LED rojo
              digitalWrite(ledVerde,HIGH);    //Encendemos el verde
              delay(4000);                  //Cuanto o qué esperamos????
              pulsacion = miTeclado.getKey();
              if (pulsacion == '*'){
                      CambiarPass();
              }
              digitalWrite(ledVerde, LOW); 
              digitalWrite(ledRojo, HIGH);
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("Introduzca clave");
              cursor_D = 4;
              lcd.setCursor(cursor_D, 1);
              lcd.print(">"); 
         }
         else{              
              lcd.clear();
              lcd.print("Clave incorrecta");  //Borramos la clave de la pantalla
              tone(buzzer, 70, 500);          //Tono de error
              delay(3000);
              lcd.setCursor(0, 0);
              lcd.print("Intente otra vez");
              digitalWrite(ledRojo, HIGH); // encendemos el LED rojo
              cursor_D = 4;
              lcd.setCursor(cursor_D, 1);
              lcd.print(">");
         }
      }
  }
  Pausa(250);
}
