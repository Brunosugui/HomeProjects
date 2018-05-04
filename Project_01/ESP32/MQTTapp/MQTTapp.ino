#include <WiFi.h>
#include <PubSubClient.h>
#include <HardwareSerial.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

// Update these with values suitable for your network.
const char* ssid = "AP_16";
const char* password = "246813579";
const char* mqtt_server = "broker.hivemq.com";
#define mqtt_port 1883
#define MQTT_USER "Buddha"
#define MQTT_PASSWORD "Buddha"
#define MQTT_SERIAL_PUBLISH_CH_1 "Buddha/ESP32/serialdata/log"
#define MQTT_SERIAL_PUBLISH_CH_2 "Buddha/ESP32/serialdata/humidity"
#define MQTT_SERIAL_PUBLISH_CH_3 "Buddha/ESP32/serialdata/temperature"
#define MQTT_SERIAL_PUBLISH_CH_4 "Buddha/ESP32/serialdata/presence"
#define MQTT_SERIAL_RECEIVER_CH_1 "Buddha/ESP32/serialdata/rx"

typedef enum{
  LOG,
  HUMIDITY,
  TEMPERATURE,
  PRESENCE
}mqtt_data_t;

HardwareSerial Serial2(2);
WiFiClient WIFIclient;
PubSubClient MQTTclient(WIFIclient);

char Response = '0';    //publish response from module

static volatile uint16_t intTriggeredCount=0; // this variable will be changed in the ISR, and Read in main loop
// static: says this variable is only visible to function in this file, its value will persist, it is a global variable
// volatile: tells the compiler that this variables value must not be stored in a CPU register, it must exist
//   in memory at all times.  This means that every time the value of intTriggeredCount must be read or
//   changed, it has be read from memory, updated, stored back into RAM, that way, when the ISR 
//   is triggered, the current value is in RAM.  Otherwise, the compiler will find ways to increase efficiency
//   of access.  One of these methods is to store it in a CPU register, but if it does that,(keeps the current
//   value in a register, when the interrupt triggers, the Interrupt access the 'old' value stored in RAM, 
//   changes it, then returns to whatever part of the program it interrupted.  Because the foreground task,
//   (the one that was interrupted) has no idea the RAM value has changed, it uses the value it 'know' is 
//   correct (the one in the register).  

//IRAM_ATTR tells the complier, that this code Must always be in the 
// ESP32's IRAM, the limited 128k IRAM.  use it sparingly.

void IRAM_ATTR initWifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  reconnectWifi();
}

void IRAM_ATTR initMQTT() {
  MQTTclient.setServer(mqtt_server, mqtt_port);
  MQTTclient.setCallback( callback);

  if(MQTTclient.connected()){
    Serial.print("MQTT connected to broker: ");
    Serial.print(mqtt_server);
    Serial.print(" at port ");
    Serial.print(mqtt_port); 
  }

  reconnectMQTT();
}

void reconnectWifi() {

  if (WiFi.status() == WL_CONNECTED)
    return;

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnectMQTT() {
  // Loop until we're reconnected
  while (!MQTTclient.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (MQTTclient.connect(clientId.c_str(), MQTT_USER, MQTT_PASSWORD)) {
      Serial.println("connected");
      //Once connected, publish an announcement...
      MQTTclient.publish("ESP32/reconnected", "reconnected. ");
      // ... and resubscribe
      MQTTclient.subscribe(MQTT_SERIAL_RECEIVER_CH_1);
    } else {
      Serial.print("failed, rc=");
      Serial.print(MQTTclient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
      reconnectWifi();
    }
  }
}

void checkConnection() {
  if (!MQTTclient.connected()) {
    reconnectMQTT();
  }
  reconnectWifi();
}

void publishData(char *Data, mqtt_data_t type) {
  switch(type){
    case LOG:
    MQTTclient.publish(MQTT_SERIAL_PUBLISH_CH_1, Data);
    break;
    case HUMIDITY:
    MQTTclient.publish(MQTT_SERIAL_PUBLISH_CH_2, Data);
    break;
    case TEMPERATURE:
    MQTTclient.publish(MQTT_SERIAL_PUBLISH_CH_3, Data);
    break;
    case PRESENCE:
    MQTTclient.publish(MQTT_SERIAL_PUBLISH_CH_4, Data);
    break;
  }
  
}

void callback(char* topic, byte *payload, unsigned int length) {
  Serial.println("-------new message from broker-----");
  Serial.print("channel:");
  Serial.println(topic);
  Serial.print("data:");
  Serial.write(payload, length);
  Serial.println();
  sendUnoCmd(String((char*)payload));
  memset(payload, 0, length);
}

void sendUnoCmd(String command){
  if(command.indexOf("umidade") >= 0){
    Serial2.println("hum");
    delay(30);
    Serial.println("comando identificado");
  }
  if(command.indexOf("temperatura") >= 0){
    Serial2.println("tmp");
    delay(30);
    Serial.println("comando identificado");
  }
  else{
    Serial.print("Failed to recognize command: ");
    Serial.println(command);
  }
}

void IRAM_ATTR getUnoData(){
  if (Serial2.available()>0){
        char uno_data[128];
    memset(uno_data, 0, 128);
    Serial2.readBytesUntil( '\n', uno_data, 128);
    Serial.println("Buffer from UNO: " + String(uno_data));
    if( String(uno_data).indexOf("P") >= 0){
      char aux[128];
      unsigned int len = 0;
      len = String(uno_data).length();
      publishData(uno_data, PRESENCE);
    }
    else if( String(uno_data).indexOf("H") >= 0){   
      unsigned int len = 0;
      len = String(uno_data).length();            
      publishData(uno_data, HUMIDITY);
    }
    else if( String(uno_data).indexOf("T") >= 0){     
      unsigned int len = 0;
      len = String(uno_data).length();            
      publishData(uno_data, TEMPERATURE);
    }
  }
}
/****************************************************Setup & Loop***************************************************/
void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector
  Serial.begin(115200);
  Serial2.begin(115200);
  Serial.setTimeout(500);// Set time out for
  delay(500);
  initWifi();
  initMQTT();
}


void loop() {
  checkConnection();

  MQTTclient.loop();
  
  getUnoData();
  
//  if (Serial.available() > 0) {
//    char serial_bfr[128];
//    memset(serial_bfr, 0, 128);
//    Serial.readBytesUntil( '\n', serial_bfr, 128);
//    publishData(serial_bfr, LOG);
//  }
}

