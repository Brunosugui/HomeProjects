#include <RCSwitch.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define transmitter_pin 8
#define led_pin 13

RCSwitch mySwitch = RCSwitch();

// Inicializa o display no endereco 0x27
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

void setup() {

  Serial.begin(115200);

  Wire.begin();

  lcd.begin (16, 2);
  
  mySwitch.enableReceive(0);
  
//mySwitch.enableTransmit(transmitter_pin);
  // Optional set pulse length.
  //mySwitch.setPulseLength(321);

  // set protocol (default is 1, will work for most outlets)
  // mySwitch.setProtocol(2);

  // Optional set number of transmission repetitions.
  // mySwitch.setRepeatTransmit(15);

  pinMode(led_pin, OUTPUT);

  lcd.setBacklight(HIGH);

lcd_greetings();
}

void loop() {
  //i2c_scanner();

  //RFID_send();

  RFID_rcv();

}
