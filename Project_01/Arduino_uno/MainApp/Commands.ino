//COMMAND PARSER code

 void espCommands(String command) {
  if (command.indexOf("hum") >= 0) {
    Serial.println("Command received!");
    DHT.read11(dht_apin);
    ESP.println("H = " + String(DHT.humidity) + "%  ");
    Serial.println("Current Humidity: " + String(DHT.humidity) + "%  ");
  }
  else if (command.indexOf("tmp") >= 0) {
    Serial.println("Command received!");
    DHT.read11(dht_apin);
    ESP.println("T = " + String(DHT.temperature) + "C  ");
    Serial.println("Current Temperature: " + String(DHT.temperature) + "C  ");
  }
  else{
    Serial.println(command);
  }
}

void serialCommands(String command) {

  if (command == "sensor\r") {
    DHTread();
    return;
  }
  else
    Serial.println(serialString);
}

