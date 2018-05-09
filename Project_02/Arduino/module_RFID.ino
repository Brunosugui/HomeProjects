void send_RFID_msg() {
  mySwitch.send("100000101100100011001000");
  digitalWrite(led_pin, HIGH);
  delay(500);
  digitalWrite(led_pin, LOW);
  delay(10000);
}

