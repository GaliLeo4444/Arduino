byte pinPote = ;
byte pinMotor = ;
byte pinRPMs = ;
byte mostrar = 0;
int valor_motor;
int rpm_reales;
volatile int ranuras = 0;




void setup() {
  pinMode(pinMotor, OUTPUT);
  pinMode(pinRPMs, INPUT_PULLUP);
  Serial.begin(9600);
  //attachInterrupt(digitalPinToInterrupt(pinRPMs), ControlarMotor, CHANGE);
}




void ControlarMotor() {
  ranuras += 1;
}


void Realimentacion() {
  int rpm_ideales = valor_motor * 2000 / 255;     //255 ------> 2000 RPM
  int diferencia = rpm_ideales - rpm_reales;
  if (diferencia > 100) {
      valor_motor += (diferencia * 255 / 2000);
      if (valor_motor > 255)
          analogWrite(pinMotor, 255);
      else
          analogWrite(pinMotor, valor_motor);
      Serial.println("*");
  }
}


void loop() {
  int valor_pote = analogRead(pinPote);
  valor_motor = valor_pote / 4;
  analogWrite(pinMotor, valor_motor);     // analogRead: 0..1023, analogWrite: 0..255
  rpm_reales = pulseIn(pinRPMs, HIGH) * 2;    //Período
  rpm_reales = 60000000 / (rpm_reales * 82);            //Frecuencia: 1/período, 82 ranuras, 1 millón de microsegundos = 1 segundo
  Realimentacion();
  mostrar += 1;
  if (mostrar == 10) {                  //Muestra cada 10 ciclos
      Serial.print("Valor Pote: ");
      Serial.println(valor_pote);
      Serial.print("Valor motor: ");
      Serial.println(valor_motor);
      Serial.print("RPMs: ");
      Serial.println(rpm_reales);
      mostrar = 0;
  }  
  //delay(1000);
}

