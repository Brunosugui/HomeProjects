void printLCD() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Buddha Project");
  lcd.setCursor(0, 1);
  lcd.print("Moisture Monitor");
  delay(1000);
}
