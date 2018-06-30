#include <RCSwitch.h>
#include <Servo.h>

/*SINAL DO PORTAO DO APARTAMENTO
 * "1010 0001 0000 1111 0101 1110 0101"
 * 28 BITS
 * PROTOCOLO 6
 */

int servoPin = 5;
int servoAngle = 0;
int buttonPin = 4;


Servo servo;
RCSwitch mySwitch = RCSwitch();


void rfid_send(int message){

mySwitch.send("1010000100001111010111100101");
delay(1000);
  
//  mySwitch.send(message, 24);
//  delay(1000);
//  mySwitch.send(message, 24);
}

void setup() {
  Serial.begin(115200);
  servo.attach(servoPin);
  pinMode(buttonPin, INPUT);
  mySwitch.enableTransmit(7);
  mySwitch.setProtocol(6);
}

void loop() {
  char data[40];
  int data_send;
  int numero;

  if(digitalRead(buttonPin) == LOW){
    rfid_send(0);
  }

//  if(Serial.available()>0){
//    numero = Serial.readBytesUntil(13, data, 40);
//    data[numero] = 0;
//    data_send = atoi(data);
//    if(data_send > 0){
//    Serial.print("Sent : ");
//    Serial.print(data_send);
//    Serial.print( " - Characters : ");
//    Serial.println(strlen(data));
//    rfid_send(data_send);
//    }else{
//      Serial.println("Failed sending data.");
//    }
//  }
  
}

