

//--Int converter 4(int) -> "04"(String) 
String getString_2digit(int number) {
  if (number < 10) {
    return "0" + String(number);

  } else {
    return String(number);
  }
}
//--------------------------------


void  Time_screen() {
  bool firstloadScreen = true;
  int sec;
  while (switchscreen() == false) {

    if (sec != second() | firstloadScreen) {
      firstloadScreen = false;
      //M5.Lcd.fillRect(0, 17, 160, 63, BLACK);
      M5.Lcd.fillScreen(TFT_BLACK);
      sec = second();

      String timedatenow = getString_2digit(hour()) + ":" + getString_2digit(minute()) + ":" + getString_2digit(second());
      
      if (current_screen == STICKC) {
        M5.Lcd.setFreeFont(&beta15pt7b); // todo add 21pt font
        M5.Lcd.setCursor(1, 30);
      } else {
        M5.Lcd.setFreeFont(&beta15pt7b);
        M5.Lcd.setCursor(15, 35);
      }
      M5.Lcd.setTextColor(WHITE, BLACK);
      M5.Lcd.print(timedatenow);

      timedatenow = getString_2digit(day()) + "/" + getString_2digit(month()) + "/" + String(year());

      if (current_screen == STICKC) {
        M5.Lcd.setCursor(1, 64);
        M5.Lcd.setFreeFont(&beta10pt7b); // todo set 15pt font
      } else {
        M5.Lcd.setCursor(15, 90);
        M5.Lcd.setFreeFont(&beta10pt7b);
      }
      
      
      M5.Lcd.print(timedatenow);
    }
  }
}
