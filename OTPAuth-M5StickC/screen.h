// Copyright 2024 Yann Guillon
// Copyright 2024 Alex Licata
// Copyright 2020 Imad Mezghache


#ifndef OTPAUTH_M5STICKC_SCREEN_H_
#define OTPAUTH_M5STICKC_SCREEN_H_

#define OTP_width 19
#define OTP_height 16
#define N_screen 2  // number of pages

#define toolbar_height 17  // height of the toolbar area (battery, icons)

#define battery_width 20
#define battery_height 10
#define batt_margin 2
#define batt_border 2


PROGMEM const unsigned char OTP_bits[4][48] = {{
    0x00, 0x00, 0xf8, 0xfe, 0xff, 0xfb, 0xfe, 0xff, 0xfb, 0x1e, 0xc0, 0xfb,  // OTP_ICON
    0xfe, 0xfd, 0xfb, 0xce, 0x8d, 0xfb, 0xb6, 0x6d, 0xfb, 0xb6, 0x6d, 0xfb,
    0xb6, 0x8d, 0xfb, 0xb6, 0xed, 0xfb, 0xb6, 0xed, 0xfb, 0xb6, 0xed, 0xf9,
    0xce, 0xef, 0xf8, 0xfe, 0x7f, 0xf8, 0xfe, 0x3f, 0xf8, 0x00, 0x00, 0xf8
  }, {
    0x00, 0x00, 0xf8, 0xfe, 0xff, 0xfb, 0xfe, 0xff, 0xfb, 0xfe, 0xff, 0xfb,  // WIFI_ICON
    0x7e, 0xf0, 0xfb, 0x3e, 0xe7, 0xfb, 0xde, 0xdf, 0xfb, 0xfe, 0xf8, 0xfb,
    0x7e, 0xf7, 0xfb, 0xfe, 0xff, 0xfb, 0xfe, 0xfd, 0xfb, 0xfe, 0xff, 0xf9,
    0xfe, 0xff, 0xf8, 0xfe, 0x7f, 0xf8, 0xfe, 0x3f, 0xf8, 0x00, 0x00, 0xf8
  }
};



#define wait_icon_width 40
#define wait_icon_height 40
static unsigned char wait_icon[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xfe, 0x07, 0x00, 0x00, 0x00, 0x45, 0x04, 0x00,
  0x00, 0x80, 0x14, 0x05, 0x00, 0x00, 0x80, 0xd1, 0x06, 0x00, 0x00, 0x80,
  0x06, 0x04, 0x00, 0x00, 0x80, 0x68, 0x05, 0x00, 0x00, 0x80, 0x22, 0x05,
  0x00, 0x00, 0x00, 0xbb, 0x03, 0x00, 0x00, 0x00, 0x6e, 0x01, 0x00, 0x00,
  0x00, 0xd4, 0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x00, 0x30,
  0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x48, 0x00, 0x00,
  0x00, 0x00, 0x94, 0x00, 0x00, 0x00, 0x00, 0x02, 0x01, 0x00, 0x00, 0x00,
  0x21, 0x02, 0x00, 0x00, 0x80, 0x00, 0x04, 0x00, 0x00, 0x80, 0x10, 0x04,
  0x00, 0x00, 0x80, 0x00, 0x04, 0x00, 0x00, 0x80, 0x00, 0x04, 0x00, 0x00,
  0x80, 0x55, 0x05, 0x00, 0x00, 0x80, 0xaa, 0x02, 0x00, 0x00, 0x80, 0xff,
  0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};



void show_wait_icon() {
  M5.Lcd.drawXBitmap(60, 22, wait_icon, wait_icon_width, wait_icon_height, txt_color, bg_color);
}

void clear_wait_icon() {
  M5.Lcd.fillRect(60, 22, wait_icon_width, wait_icon_height, bg_color);
}

void drawBattery() {
  float batteryLevel = M5.Power.getBatteryLevel();

  // draw the outer base rectangle with white color, the inner rectangle with black color,
  // then fill the inner rectangle with greencolor with the battery level
  int batteryLevelWidth = static_cast<int>(batteryLevel / 100.0 * (battery_width - batt_border));

  int batt_x = screen_x - battery_width - batt_margin;
  int batt_y = batt_margin;

  int batt_color = batteryLevel < 20 ? TFT_RED : TFT_GREEN;

  M5.Lcd.drawRect(batt_x, batt_y, battery_width, battery_height, txt_color);
  M5.Lcd.fillRect(batt_x + 1, batt_y + 1, battery_width - batt_border, battery_height - batt_border, bg_color);
  M5.Lcd.fillRect(batt_x + 1, batt_y + 1, batteryLevelWidth, battery_height - batt_border, batt_color);
}

void showmenu() {
  for (int i = 0; i < N_screen; i++) {
    int primary_color = menu_index == i ? txt_color : bg_color;
    int secondary_color = menu_index == i ? bg_color : txt_color;
    M5.Lcd.drawXBitmap(20 * i, 0, OTP_bits[i], OTP_width, OTP_height, primary_color, secondary_color);
  }
}

bool switchscreen() {
  uint32_t currentMillis = millis();
  if (currentMillis - previousMillis >= timeout_ScreenOn) {
    M5.Power.powerOff();
  }
  M5.update();
  if (M5.BtnB.wasPressed()) {
    previousMillis = currentMillis;
    if (menu_index == (N_screen - 1)) {
      menu_index = 0;
    } else {
      menu_index++;
    }
    showmenu();
    drawBattery();
    return true;
  } showmenu();
  return false;
}

#endif  // OTPAUTH_M5STICKC_SCREEN_H_
