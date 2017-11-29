#include <LiquidCrystal.h>
#include <Keypad.h>
#include <EEPROM.h>




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




void setup()
{
  pinMode(A0, OUTPUT);
  digitalWrite(A0, HIGH);
  pinMode(A1, OUTPUT);
  digitalWrite(A1, HIGH);
  pinMode (ledVerde, OUTPUT);
  digitalWrite(ledVerde, LOW); 
  pinMode (ledRojo, OUTPUT);
  digitalWrite(ledRojo, HIGH); 
  pinMode (buzzer, OUTPUT);
  digitalWrite(buzzer, HIGH);
  Serial.begin(9600);
  miTeclado.setDebounceTime(200);      //Para evitar rebote de tecla
  for (int i=1; i <= 6; i++){
      codigoSecreto[i] = EEPROM.read(i);    //Busca el pass desde la EEPROM
  }
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Introduzca clave");
  lcd.setCursor(cursor_D, 1);
  lcd.print(">");             
}




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
}

void Pausa(unsigned int tiempo) {
  incrementMilis = 0;
  InitTimer1();
  while (incrementMilis < tiempo){
  }
  TCCR1A = 0;
  TCCR1B = 0;
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
      while (!hayCaracter ()){
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


char hayCaracter () {
  pulsacion = miTeclado.getKey() ;       //Leemos pulsacion
  if (Serial.available()>0){            //Leemos desde BlueTooth
     pulsacion = Serial.read();
  }
  return pulsacion;
}




void loop()
{
  if (hayCaracter()){
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
              if (hayCaracter()){
                  if (pulsacion == '*'){
                      CambiarPass();
                  }
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
