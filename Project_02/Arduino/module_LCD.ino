void printLCD(String *line1, String *line2) {
  if (line1->length() > 16) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Value too long");
    lcd.setCursor(0, 1);
    lcd.print("Not printable");
  }
  else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(*line1);
    lcd.setCursor(0, 1);
    lcd.print(*line2);
    delay(1000);
  }
}

void lcd_greetings(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Buddha Project");
  lcd.setCursor(0, 1);
  lcd.print("RFID Receiver");
}

