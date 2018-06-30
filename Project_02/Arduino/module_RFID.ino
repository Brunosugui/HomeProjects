void RFID_send() {
  mySwitch.send("100000101100100011001000");
  digitalWrite(led_pin, HIGH);
  delay(500);
  digitalWrite(led_pin, LOW);
  delay(10000);
}

void RFID_rcv(){
  if(mySwitch.available()){
//    char aux_value[12];
//    ltoa(mySwitch.getReceivedValue(), aux_value, 10);
//    String aux_length = mySwitch.getReceivedBitlength();
//    String aux_protocol ;
//    ltoa mySwitch.getReceivedProtocol();

    String line1 = "rcvd: " + String(mySwitch.getReceivedValue());
    String line2 = String(mySwitch.getReceivedBitlength()) + " bits, ptcl " + String(mySwitch.getReceivedProtocol());
    printLCD(&line1, &line2);
    Serial.print("Received ");
    Serial.print(mySwitch.getReceivedValue());
    Serial.print(" / ");
    Serial.print(mySwitch.getReceivedBitlength() );
    Serial.print("bit ");
    Serial.print("Protocol: ");
    Serial.println(mySwitch.getReceivedProtocol());

    mySwitch.resetAvailable();
  }
}

