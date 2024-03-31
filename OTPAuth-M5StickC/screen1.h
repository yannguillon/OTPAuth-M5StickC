void OTP_screen() {

  int how_many_otp_registred = 0;

  for (int i = 0; i < maxOTPs; i++) {
    String otpBool = "B" + String((i + 1));
    (NVS.getInt(otpBool) == 1) ? how_many_otp_registred++ : true;

  }

  int pointer = 0;               //point before first OTP code to activate the first search in memory

  bool firstloadScreen = true;   //true firstloadScreen

  String totpCode = "";

  int sec;                       //will store last secend when the bar was updated

  while (switchscreen() == false) {


    if (how_many_otp_registred == 0) {
      if (firstloadScreen) {

        // fill the screen with bg color, except the toolbar
        M5.Lcd.fillRect(0, toolbar_height, screen_x, screen_y - toolbar_height, bg_color);

        if (current_screen == STICKC) {
          M5.Lcd.setCursor(10, 32);
        } else {
          M5.Lcd.setCursor(10, 32);
        }
        M5.Lcd.setFreeFont(&inter8pt7b);
        M5.Lcd.print("NO OTPs");

        if (current_screen == STICKC) {
          M5.Lcd.setCursor(10, 53);
        } else {
          M5.Lcd.setCursor(10, 53);
        }
        M5.Lcd.setFreeFont(&inter8pt7b);
        M5.Lcd.print("GO TO WIFI SETUP");

        if (current_screen == STICKC) {
          M5.Lcd.setCursor(10, 80);
        } else {
          M5.Lcd.setCursor(10, 80);
        }

        M5.Lcd.setFreeFont(&inter8pt7b);
        M5.Lcd.print("OTPAuth-M5StickC");

        // if (current_screen == STICKC) {
        //    M5.Lcd.drawXBitmap(90, 17, logoapp_bits, logoapp_width, logoapp_height, BLACK, WHITE);
        // } else {
        //    M5.Lcd.drawXBitmap(90, 17, logoapp_bits, logoapp_width, logoapp_height, BLACK, WHITE);
        // }


        firstloadScreen = false;
      }
    } else {

      if (M5.BtnA.wasPressed() | firstloadScreen) {
        bool search = true;
        previousMillis = millis();

        while (search) {                                         // Search in memory for valid OTP data
          (pointer == maxOTPs) ? pointer = 1 : pointer++;
          String otpBool = "B" + String(pointer);
          (NVS.getInt(otpBool) == 1) ? search = false : false;
          firstloadScreen = true;
        }
      }

      size_t hmac_length = NVS.getBlobSize(String(pointer));
      uint8_t hmacKey[hmac_length] = {};
      NVS.getBlob(String(pointer), hmacKey, hmac_length);
      TOTP totp = TOTP(hmacKey, hmac_length);
      String newCode = String(totp.getCode(now()));


      if (totpCode != newCode | firstloadScreen) {

        firstloadScreen = false;
        // fill the screen with black color, except the toolbar
        M5.Lcd.fillRect(0, toolbar_height, screen_x, screen_y - toolbar_height, bg_color);

        totpCode = String(newCode);

        if (current_screen == STICKC) { // set text for label
          M5.Lcd.setCursor(10, 32);
          M5.Lcd.setFreeFont(&beta5pt7b);
        } else {
          M5.Lcd.setCursor(10, 31);
          M5.Lcd.setFreeFont(&inter12pt7b); // #1
        }

        String otpLabel = "L" + String(pointer);
        int charsNumber = 25;
        if (current_screen == STICKC) {
          charsNumber = 15;
        }
        M5.Lcd.print(String(pointer) + ": " + NVS.getString(otpLabel).substring(0, charsNumber));

        if (current_screen == STICKC) { // set text for user
          M5.Lcd.setCursor(10, 42);
        } else {
          M5.Lcd.setFreeFont(&inter8pt7b); // #2
          M5.Lcd.setCursor(10, 56);
        }

        String otpUser = "U" + String(pointer);
        M5.Lcd.print(NVS.getString(otpUser).substring(0, charsNumber));

        if (current_screen == STICKC) { // set text for code
          M5.Lcd.setCursor(10, 64);
          M5.Lcd.setFreeFont(&beta15pt7b);
        } else {
          M5.Lcd.setCursor(10, 85);
          M5.Lcd.setFreeFont(&mishmash21pt7b);
        }


        M5.Lcd.print(newCode);
      }

      //-----------------------------------timebar----------------------------------------//
      if (sec != second()) {
        sec = second();
        int OTP_bar = second() % 30;
        float OTP_bar_x = OTP_bar;
        OTP_bar_x = (OTP_bar_x / 30) * screen_x;
        OTP_bar = OTP_bar_x;
        M5.Lcd.fillRect(0, screen_y - 3, OTP_bar, 3, (OTP_bar < 130) ? txt_color : RED);
        M5.Lcd.fillRect(OTP_bar, screen_y - 3, screen_x - OTP_bar, 3, bg_color);
      }
    } //-----------------------------------timebar----------------------------------------//
  }
}
