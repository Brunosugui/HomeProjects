//READERS code

#define LDR_trigger_value 225
#define interval 500
#define clap_interval 300

unsigned long soundTime1 = 0;
unsigned long soundTime2 = 0;
unsigned long pirTime = 0;

void LDRread(){
  int LDR_value = analogRead(ldr_apin);
  delay(10);
  if( LDR_value > LDR_trigger_value ){
    digitalWrite(yellowLedPin, HIGH);
  }
  else{
    digitalWrite(yellowLedPin, LOW);
  }
}

void PIRread(){
  if(!digitalRead(presence_dpin)){
    digitalWrite(redLedPin, HIGH);
    digitalWrite(blueLedPin, LOW);
  }
  else{
    digitalWrite(redLedPin, LOW);
    digitalWrite(blueLedPin, HIGH);
  }
  if( pirTime == 0){
    pirTime = millis();
    ESP.println("P = " + String(digitalRead(presence_dpin)));
  }else{
    if( millis() - pirTime >= 500){
      pirTime = 0;
    }
  }
  delay(50);
}

void DHTread() {
  Serial.println("Reading value from DHT sensor...");
  DHT.read11(dht_apin);
  Serial.print("Current humidity = ");
  Serial.print(DHT.humidity);
  Serial.print("%  ");
  Serial.print("temperature = ");
  Serial.print(DHT.temperature);
  Serial.println("C  ");
  sensorFlag = false;
}


void getSound() {
  if (digitalRead(sound_dpin)) {
    if (soundTime1 != 0) {
      soundTime2 = millis();
      if (soundTime2 - soundTime1 <= 75) {
        soundTime1 = millis();
        soundTime2 = 0;
      }
      else if (soundTime2 - soundTime1 >= clap_interval && soundTime2 - soundTime1 <= 2 * clap_interval) {
        DHTread();
        Serial.println(String(soundTime2 - soundTime1));
        soundTime1 = 0;
        soundTime2 = 0;
      }
      else {
        soundTime1 = 0;
        soundTime2 = 0;
      }
    } else {
      soundTime1 = millis();
    }
  }
}

