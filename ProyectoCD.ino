#include <LiquidCrystal.h>
#include <Keypad.h>
#include <EEPROM.h>

LiquidCrystal lcd(A0, A1, 10, 11, 12, 13);
const byte COLUMNAS= 4;
const byte FILAS= 4;
char teclas [FILAS] [COLUMNAS]= {{'1', '2', '3', 'A'},{'4', '5', '6', 'B'},{'7', '8', '9', 'C'},{'*', '0', '#', 'D'}};
byte filasPines[FILAS]= {9,8,7,6}; //define lineas
byte columnasPines[COLUMNAS]= {5,4,3,2}; //define columnas

Keypad miTeclado= Keypad( makeKeymap(teclas), filasPines, columnasPines, FILAS, COLUMNAS );

char codigoSecreto[6]= {'C','D','2','0','1','7'}; // aqui va el codigo secreto
char pulsacion;
int eeAddress= 0;    //necesaria para guardar la clave en memoria eeprom
int posicion= 0;    // necesaria para la clave
int cursor= 5;      // posicion inicial de la clave en el LCD
int clave= 0;       // para el LCD
int ledVerde= A2;   // pin para el LED verde
int ledRojo= A3;    // pin para el LED rojo
int buzzer= A4;     // pin altavoz

void setup()
{
Serial.begin(9600) ;
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
lcd.begin(16, 2);
lcd.setCursor(0, 0);
lcd.print("Introduzca clave");
lcd.setCursor(4, 1);
lcd.print(">");             
}

void loop()
{ 
  EEPROM.put(eeAddress, codigoSecreto);
  pulsacion= miTeclado.getKey() ; // leemos pulsacion
  if (Serial.available()>0){
   pulsacion= Serial.read();
}
 
      if (pulsacion != 0) //Si el valor es 0 es que no se ha pulsado ninguna tecla
        { // descartamos numeral y asterisco
          if (pulsacion != '#' && pulsacion != '*' && clave==0)
           { lcd.print(pulsacion); // imprimimos pulsacion
             cursor++;             // incrementamos el cursor
             tone(buzzer, 350);     // tono de pulsacion
             delay(200);
             noTone(buzzer);
             
//--- Condicionales para comprobar la clave introducida -----------

      // comparamos entrada con cada uno de los digitos, uno a uno
      if (pulsacion == codigoSecreto[posicion])
          posicion ++; // aumentamos posicion si es correcto el digito
      if (posicion == 6)
       { // comprobamos que se han introducido los 4 correctamente
         digitalWrite (ledVerde, HIGH);  // encendemos LED
         clave=1; // indicamos que se ha introducido la clave
         lcd.clear();
         lcd.setCursor(0,0);      // situamos el cursor el la pos 0 de la linea 0.
         lcd.print("Clave correcta");         // escribimos en LCD
         delay(2500);                           // tono de clave correcta
         lcd.clear();
         lcd.print("Bienvenido!");         // escribimos en LCD
         tone(buzzer, 500);
         delay(100);
         noTone(buzzer);
         tone(buzzer, 600);
         delay(100);
         noTone(buzzer);
         tone(buzzer, 800);
         delay(100);
         noTone(buzzer);
         digitalWrite(ledRojo, LOW); // apagamos el LED rojo
         digitalWrite(ledVerde,HIGH); // encendemos el verde
     }
     
//--- En el caso de que este incompleta o no hayamos acertado ----------
     
     if(cursor>10 && clave !=1)        // comprobamos que no pase de la cuarta posicion
       {   lcd.clear();
           cursor= 4;     // lo volvemos a colocar al inicio
           posicion= 0;           // borramos clave introducida
           lcd.setCursor(0, 0);
           clave=0;
           lcd.print("Clave incorrecta");       // borramos la clave de la pantalla
           delay(2500);
           lcd.setCursor(0, 0);
           lcd.print("Intente otra vez");
           digitalWrite(ledRojo, HIGH); // encendemos el LED rojo
           lcd.setCursor(4, 1);
           lcd.print(">");
           if(clave == 0)         // comprobamos que no hemos acertado
              { tone(buzzer, 70, 500); // para generar
                delay(250); // tono de error
                noTone(buzzer);
              }
        }
     }
   } 

 //--- Condicionales para resetear clave introducida -------------
 
 if (pulsacion == '*')
     { // asterisco para resetear el contador
       posicion= 0;
       cursor= 4;
       clave= 0;
       posicion= 0;
       lcd.clear();
       lcd.setCursor(0, 0); // situamos el cursor el la posición 0 de la linea 0.
       lcd.print("Introduzca clave"); // escribimos en LCD
       lcd.setCursor(4, 1);
       lcd.print(">"); // borramos de la pantalla los numeros
       digitalWrite(ledRojo, HIGH); // encendemos el LED rojo
       digitalWrite(ledVerde, LOW); // apagamos el verde
    }
 }
