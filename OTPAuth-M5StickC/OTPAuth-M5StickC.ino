/*
    OXOTP+
   ------------------------------------------
       Copyright (c) 2024 Alex Licata
      Copyright (c) 2020 Mezghache imad
          github.com/xick/OXOTP-plus
  -------------------------------------------
 */

// Go to WIFI settings and connect to the specified Access point.
// Then go to http://192.168.4.1/ to configure

#define maxOTPs 50
//#define timeout_ScreenOn 180000 // Shutdown time


char random_letters[] = "0123456789"
                        "abcdefghijklmnopqrstuvwxyz"
                        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"; // This string contains the characters used to generate the wifi password

#include <WiFi.h>
#include <WebServer.h>
#include <WiFiClient.h>
#include <ESPmDNS.h>
#include <Update.h>
#include <WiFiAP.h>
#include <TOTP.h>
#include <TimeLib.h>
#include <ArduinoJson.h>
#include "ArduinoNvs.h"
#include "M5Unified.h"

// Run-Time Variables //

WebServer server(80);

#include "Mishmash21pt7b.h"
#include "Inter8pt7b.h"
#include "Inter12pt7b.h"

#include "variable_runtime.h"

#include "webserver/dist/gzip/index.html.h"
#include "webserver/dist/gzip/tailwind.min.css.h"
#include "webserver/dist/gzip/favicon.png.h"
#include "webserver/dist/gzip/app.js.h"

#include "screen.h"
#include "screen1.h"
#include "screen2.h"
#include "math.h"

void setup() {

  Serial.begin(115200);
  while (!Serial);

  auto cfg = M5.config();
  M5.begin(cfg);

  NVS.begin();

  Serial.println("===============ESP32-OTPAuth-M5StickC==============");
  Serial.println("================= V 1.2 ================");
  Serial.println("===============ESP32-OTPAuth-M5StickC+==============");

  // -- Get Preferences from NVS
  if (NVS.getInt("lcd_brightness") != 0) {
    lcd_brightness = NVS.getInt("lcd_brightness");
  }
  M5.Display.setBrightness(lcd_brightness); // 0 - 255

  // get timeout from NVS
  if (NVS.getInt("timeout_ScreenOn") != 0) {
    timeout_ScreenOn = NVS.getInt("timeout_ScreenOn");
  }

  // get timezone from NVS
  timezone_h = NVS.getInt("timezone_h");
  timezone_m = NVS.getInt("timezone_m");

  // get bg and txt color from NVS
  if (NVS.getInt("bg_color") != 0) {
    bg_color = NVS.getInt("bg_color");
  }

  if (NVS.getString("txt_color") != "") {
    txt_color = NVS.getString("txt_color").toInt();
    Serial.println("txt_color: " + NVS.getString("txt_color"));
  }

  M5.Lcd.setRotation(1);
  screen_x = M5.Lcd.width();
  screen_y = M5.Lcd.height();

  M5.Lcd.fillScreen(bg_color);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setTextColor(txt_color, bg_color);

  // get tm from RTC
  tm rtc_tm = M5.Rtc.getDateTime().get_tm();
  setTime(rtc_tm.tm_hour, rtc_tm.tm_min, rtc_tm.tm_sec, rtc_tm.tm_mday, rtc_tm.tm_mon + 1, rtc_tm.tm_year + 1900);
}

void loop() {

//   this handle the switch
  switch (menu_index) {
    case 0:
      OTP_screen();
      break;
    case 1:
      Wifi_screen();
      break;
  }

  switchscreen();

}
