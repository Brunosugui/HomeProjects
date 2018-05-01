#include <dht.h>
#include "SoftwareSerial.h"

#define dht_apin A0
#define ldr_apin A1
#define sound_dpin 12
#define presence_dpin 4
//#define button1Pin 5
#define redLedPin 2
#define blueLedPin 3
#define yellowLedPin 5

SoftwareSerial ESP(7, 8); // RX, TX

String serialString = "";

int button1State = HIGH;
int buttonSet = false;

dht DHT;

int sensorFlag;

void initSerial() {
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("System started!");

  ESP.begin(115200);
}

void initPins() {
  pinMode(LED_BUILTIN, OUTPUT);
  //pinMode(button1Pin, INPUT);
  pinMode(sound_dpin, INPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(blueLedPin, OUTPUT);
  pinMode(presence_dpin, INPUT);
  pinMode(yellowLedPin, OUTPUT);
}

//void buttonAction() {
//  int pinread = digitalRead(button1Pin);
//
//  if (pinread == LOW && buttonSet == false) {
//    digitalWrite(LED_BUILTIN, button1State);
//    button1State = !button1State;
//    buttonSet = true;
//    delay(50);
//  }
//  if (pinread == HIGH) {
//    buttonSet = false;
//    delay(50);
//  }
//}


/*********************************************************  SETUP & LOOP CODE **********************************************************************/
void setup() {
  initPins();
  initSerial();
}

void loop() {


  //ledBlink();
  //buttonAction();

  PIRread();

  LDRread();

  getSound();

  while (ESP.available()) {
    String inData = ESP.readStringUntil('\n');
    Serial.println("ESP32: " + inData);
    espCommands(inData);
  }
  while (Serial.available()) {
    String outData = Serial.readStringUntil('\n');
    serialCommands(outData);
    Serial.println(outData);
    delay(30);
  }
}

