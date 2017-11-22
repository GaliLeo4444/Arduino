#include<SoftwareSerial.h>
#include<stdio.h> 
#include<string.h> 

// Se crea una UART virtual en los pines 8 y 9 (Rx y Tx respectivamente)
SoftwareSerial miSerial(8,9);

char modo;

void setup() {
  // inicializa ambas unidades serie:
  Serial.begin(9600);
  miSerial.begin(4800);
  
  modo = '0';
  
  // Menu
  Serial.println("Ingrese 1 para modo calcular Checksum de un msj");
  Serial.println("Ingrese 2 para modo enviar msj con checksum");
}

void loop() {
  // Elección del modo de funcionamiento
  if (Serial.available() > 0) {
    modo = Serial.read();
  }
    
  if(modo == '1'){
    // Entra en modo 1 para calcular el checksum de un mensaje ingresado por consola
    Serial.println("Ingrese Mensaje (ingrese 'salir' para volver al menu anterior)");
    while(modo == '1'){
      if (Serial.available() > 0){
        String IncomingMsj = Serial.readString();
        if(IncomingMsj == "salir"){
          modo = '0';
          Serial.println("Ingrese 1 para modo calcular Checksum de un msj");
          Serial.println("Ingrese 2 para modo enviar msj con checksum");
        }else{
          Serial.print("Mensaje: ");
          Serial.println(IncomingMsj);
          char mensaje[24] = {0};
          IncomingMsj.toCharArray(mensaje, 24);
          unsigned char checksum = CalcularCheckSum(mensaje);
          Serial.print("Checksum: ");
          //Serial.println(checksum);
          Serial.println(checksum, HEX); 
          //Serial.println(checksum, DEC);        
        }
      }
    }     
  }
  
  else if (modo == '2'){
    // Entra en modo 2 para enviar y recibir mensajes+checksum por softwareserial
    Serial.println("Ingrese Mensaje (ingrese 'salir' para volver al menu anterior)");
    while(modo == '2'){
      // Mensaje desde consola para enviar a otro dispositivo
      if (Serial.available() > 0){
        String localMsj = Serial.readString();
        if(localMsj == "salir"){
          modo = '0';
          Serial.println("Ingrese 1 para modo calcular Checksum de un msj");
          Serial.println("Ingrese 2 para modo enviar msj con checksum");
        }else{
          Serial.println("Ingrese Checksum en hexadecimal");      
          while (!Serial.available()); // Espera hasta que se ingrese el checksum
          String Checksum = Serial.readString();
          // Muestra Mensaje + CheckSum
          Serial.print("Mensaje: ");
          Serial.println(localMsj);
          Serial.print("Checksum: ");
          Serial.println(Checksum);
          // Envia msj+checksum al dispositivo conectado por softwareserial
          miSerial.print(localMsj+HextoChar(Checksum));    
        }
      }
      // Mensaje proveniente de otro disposivo para realizar la comprobación de error
      if(miSerial.available() > 0){
        String IncomingMsj = "";
        while(miSerial.available()){ // Leo el mensaje caracter a caracter
          char inChar = (char)miSerial.read();
          IncomingMsj += inChar;
          delay(100);
        }     
        if(IncomingMsj == "Ok" || IncomingMsj == "Error"){
          // Respuesta del receptor de nuestro mensaje
          Serial.println(IncomingMsj);
        }else{
          // Comprobación del mensaje recibido
          char mensaje[25] = {0};
          IncomingMsj.toCharArray(mensaje, 24);
          if (ComprobarCheckSum(mensaje)){             
            miSerial.println("Ok");     
          }else{
            miSerial.println("Error");
          }   
        }     
      }
    }
  }  
}

unsigned char CalcularCheckSum(char msj[]){
  unsigned char suma = 0;
  //Serial.print("largo: ");
  //Serial.println(sizeof(msj));
  for (int i=0; i<24; i++) {
       suma += msj[i];
  }
  suma = (~suma)+1;     //Complemento a 2
  return suma;
}

int ComprobarCheckSum(char msj[]){
  // 0: Error, 1: Ok
  unsigned char i, check = 0;
  for (i=0; i<25; i++) {
       check += msj[i];
  }
  if (check == 0){             
     return (1);     
  }else{
       return (0);
  }
}

unsigned char convertirParte(char hexa){
  switch (hexa) {
    case '0':
          return (0x00);    //0000 0000
    case '1':
          return (0x01);
    case '2':
          return (0x02);
    case '3':
          return (0x03);
    case '4':
          return (0x04);
    case '5':
          return (0x05);
    case '6':
          return (0x06);
    case '7':
          return (0x07);
    case '8':
          return (0x08);
    case '9':
          return (0x09);
    case 'A':
          return (0x0A);    //0000 1010
    case 'B':
          return (0x0B);
    case 'C':
          return (0x0C);
    case 'D':
          return (0x0D);
    case 'E':
          return (0x0E);
    case 'F':
          return (0x0F);
    default:
          return (0);
  }
}

char HextoChar(String chk){
  unsigned char partealta, partebaja = 0;
  if (chk.length() == 1){                         //Tiene un solo caracter
      partealta = 0x00;
      partebaja = convertirParte(chk[0]);
  } else {                                        //Tiene dos caracteres
        partealta = convertirParte(chk[0])<<4;    //Desplaza a la izquiera (agrega 4 0's)
        partebaja = convertirParte(chk[1]);
  }
  return (partealta + partebaja);
}
