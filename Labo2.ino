#include <Keypad.h>
#include <LiquidCrystal.h>


/*  The circuit:
 * LCD RS pin to digital pin 13
 * LCD Enable pin to digital pin 12
 * LCD D4 pin to digital pin 11
 * LCD D5 pin to digital pin 10
 * LCD D6 pin to digital pin 9
 * LCD D7 pin to digital pin 8
 * LCD R/W pin to ground
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
*/




const short int maxint=32767;
const short int minint=-32768;
const long int maxlong=2147483647;
const long int minlong=-2147483648;
const byte fil_teclado = 4;
const byte col_teclado = 4;


char teclas[fil_teclado][col_teclado] = {
  {'1','2','3', 'A'},
  {'4','5','6', 'B' },
  {'7','8','9', 'C'},
  {'*','0','#', 'D'}
};
byte filPins[fil_teclado] = {4, 3, 2, 1}; //connect to the row pinouts of the keypad
byte colPins[col_teclado] = {8, 7, 6, 5}; //connect to the column pinouts of the keypad

Keypad teclado = Keypad(makeKeymap(teclas), filPins, colPins, fil_teclado, fil_teclado);


// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);




short int SumaInt (int a, int b)
{
    if (a>0 && b>0){
        if (a <= maxint-b) {
            return(a + b);
        }
        else {
            printf("OVERFLOW!!!!\n");
            return(0);
        }
    }
    if (a<0 && b<0){
        if (a >= minint-b) {
            return(a + b);
        }
        else {
            printf("OVERFLOW!!!!\n");
            return(0);
        }
    }
    else {
        return(a + b);
    }
}


short int RestaInt (int a, int b)
{
    if ((a<0 && b>0) || (a>0 && b<0)){
        if (b < 0) {
            if (a <= maxint+b) {
                return(a - b);
            }
            else {
                printf("OVERFLOW!!!!\n");
                return(0);
            }
        }
        else {
            if (a >= minint+b) {
                return(a - b);
            }
            else {
                printf("OVERFLOW!!!!\n");
                return(0);
            }
        }
    }
    else {
        return(a - b);
    }
}




void setup() {
  teclado.begin(makeKeymap(teclas));
  
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // turn on the cursor:
  lcd.cursor();
}


void loop() {
  char tec = teclado.getKey();
  
  if (tec){
      for (int  thisRow = 0; thisRow < 2; thisRow++) {
        for (int thisCol = 0; thisCol < 16; thisCol++) {
          // set the cursor position:
          lcd.setCursor(thisCol, thisRow);
          // print the letter:
          lcd.write(tec);
          delay(400);
        }
      }
  }
}
