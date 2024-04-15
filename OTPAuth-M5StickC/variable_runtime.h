// Copyright 2024 Yann Guillon
// Copyright 2024 Alex Licata
// Copyright 2020 Imad Mezghache

#ifndef OTPAUTH_M5STICKC_VARIABLE_RUNTIME_H_
#define OTPAUTH_M5STICKC_VARIABLE_RUNTIME_H_

StaticJsonDocument<2000> jsondata;
m5::rtc_datetime_t localTime;

uint16_t screen_x;
uint16_t screen_y;

uint16_t bg_color = TFT_BLACK;
uint16_t txt_color = TFT_WHITE;

uint32_t previousMillis = 0;
int8_t lcd_brightness = 100;
int timeout_ScreenOn = 180000;  // 3 minutes

// timezone offset (hours and minutes for local time zone from UTC)
int8_t timezone_h = 0;
int8_t timezone_m = 0;

int menu_index = 0;
int passwordMode = 0;  // 0 random, 1 fixed
String Wifi_Mode = "AP";  // Default mode
String Wifi_SSID = "";
String Wifi_PASSWORD = "";

bool _mdnsStarted = false;

#endif  // OTPAUTH_M5STICKC_VARIABLE_RUNTIME_H_
