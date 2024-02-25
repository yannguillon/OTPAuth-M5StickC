/*
    OXOTP+
   ------------------------------------------
      Copyright (c) 2024 Alex Licata
      Copyright (c) 2020 Mezghache imad
            github.com/xick/OXOTP
  -------------------------------------------
 */

// Go to WIFI settings and connect to the specified Access point. 
// Then go to http://192.168.4.1/ to configure

#define maxOTPs 30                         // max OTP can hande OXOTP
#define timeout_ScreenOn 180000            // The time at which the OXOTP shutdown after inactivity
#define lcd_brightness 100                   // brightness of the LCD


// timeout and brightness could set on app later

String rondom_letters = "AEF2345689";      // This string contains the characters used to generate the wifi password

#include<WiFi.h>
#include<WebServer.h>
#include<WiFiAP.h>
#include <WiFi.h>
#include<TOTP.h>
#include<TimeLib.h>
#include<ArduinoJson.h>
#include"ArduinoNvs.h"

#include"M5Unified.h"

// Run-Time Variables //

WebServer server(80);

#include"beta15pt7b.h"
#include"beta10pt7b.h"
#include"beta8pt7b.h"
#include"beta5pt7b.h"

#include"variable_runtime.h"
#include"index.h"
#include"css.h"

#include"screen.h"
#include"screen1.h"
#include"screen2.h"
#include"screen3.h"
#include"math.h"

void setup() {

  Serial.begin(115200);
  while (!Serial);

  auto cfg = M5.config();
  M5.begin(cfg);

  M5.Display.setBrightness(lcd_brightness); // 0 - 255
  NVS.begin();

  Serial.println("===============ESP32-OXOTP+==============");
  Serial.println("================= V 0.9 ================");
  Serial.println("===============ESP32-OXOTP+==============");

  M5.Rtc.getTime(&TimeStruct);
  M5.Rtc.getDate(&DateStruct);

  M5.Lcd.setRotation(3);
  screen_x = M5.Lcd.width();
  screen_y = M5.Lcd.height();

  if (screen_x == 135) {
    current_screen = STICKC;
  } else {
    current_screen = STICKCPLUS;
  }

  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextSize(1);

  setTime(TimeStruct.hours, TimeStruct.minutes, TimeStruct.seconds, DateStruct.date, DateStruct.month, DateStruct.year);
}

void loop() {

//   this handle the switch
  switch (menu_index) {
    case 0:
      OTP_screen();
      break;
    case 1:
      Time_screen();
      break;
    case 2:
      Wifi_screen();
      break;
  }

  switchscreen();
  
}
