#include <IRremote.hpp>

int b = 8;
int x = A0;
int y = A1;
int address = 0x43;

int x_pos;
int y_pos;

int maximo = 700;
int minimo = 300;

int command = 0x00;
unsigned long currenttime;
unsigned long lasttransmittime = 0;
int transmit_interval = 200;

void setup() {
  Serial.begin(9600);
  IrSender.begin(5);
  
  pinMode(b, INPUT_PULLUP);
  pinMode(x, INPUT);
  pinMode(y, INPUT);
}

void loop() {
  currenttime = millis();

  x_pos = analogRead(x);
  y_pos = analogRead(y);

  command = 0x00;
  
  if(x_pos > maximo && y_pos < maximo && y_pos > minimo) {
    Serial.println("l");
    command = 0x6C;
  } else if (x_pos < minimo && y_pos < maximo && y_pos > minimo) {
    Serial.println("r");
    command = 0x72;
  }

  if(y_pos > maximo && x_pos < maximo && x_pos > minimo) {
    Serial.println("f");
    command = 0x66;
  } else if (y_pos < minimo && x_pos < maximo && x_pos > minimo) {
    Serial.println("b");
    command = 0x62;
  }

  if(digitalRead(b) == LOW) {
    Serial.println("a");
    command = 0x61;
  }
  
  if ((currenttime-lasttransmittime >= transmit_interval)) {
    IrSender.sendNEC(address, command, 3);
    lasttransmittime = millis();
  }
}



