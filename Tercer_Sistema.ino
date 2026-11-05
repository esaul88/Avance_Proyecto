#include <Servo.h>

#define ENA 5
#define ENB 6
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 11
#define SERVO 3
#define LED_DEBUG 13 // Usaremos el LED integrado de la placa

Servo servo;

unsigned char carSpeed = 250;
char currentCommand = 's';

void forward(){ 
  digitalWrite(ENA,HIGH); digitalWrite(ENB,HIGH);
  digitalWrite(IN1,HIGH); digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);  digitalWrite(IN4,HIGH);
}

void back(){
  digitalWrite(ENA,HIGH); digitalWrite(ENB,HIGH);
  digitalWrite(IN1,LOW);  digitalWrite(IN2,HIGH);
  digitalWrite(IN3,HIGH); digitalWrite(IN4,LOW);
}

void left(){
  analogWrite(ENA,carSpeed); analogWrite(ENB,carSpeed);
  digitalWrite(IN1,LOW);     digitalWrite(IN2,HIGH);
  digitalWrite(IN3,LOW);     digitalWrite(IN4,HIGH); 
}

void right(){
  analogWrite(ENA,carSpeed); analogWrite(ENB,carSpeed);
  digitalWrite(IN1,HIGH);    digitalWrite(IN2,LOW);
  digitalWrite(IN3,HIGH);    digitalWrite(IN4,LOW);
}

void stopCar(){
  digitalWrite(ENA,LOW); digitalWrite(ENB,LOW);
}

void setup() { 
  Serial.begin(9600); // Velocidad estándar, pero sospechamos de esto
  pinMode(LED_DEBUG, OUTPUT);
  digitalWrite(LED_DEBUG, LOW);
  
  servo.attach(SERVO);
  pinMode(IN1,OUTPUT); pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT); pinMode(IN4,OUTPUT);
  pinMode(ENA,OUTPUT); pinMode(ENB,OUTPUT);
  stopCar();
}

void loop() { 
  if (Serial.available() > 0) {
    // ¡Chismoso visual! El LED parpadea si entra CUALQUIER dato por RX
    digitalWrite(LED_DEBUG, HIGH);
    
    char getstr = Serial.read();

    if (getstr != '\n' && getstr != '\r') {
      currentCommand = getstr;
      
      switch(currentCommand){
        case 'f': forward();   break;
        case 'b': back();      break;
        case 'l': left();      break;
        case 'r': right();     break;
        case 's': stopCar();   break;
        default:  break;
      }
    }
    
    // Apaga el LED rápido para que se vea el parpadeo en el siguiente comando
    delay(30); 
    digitalWrite(LED_DEBUG, LOW);
  }
}