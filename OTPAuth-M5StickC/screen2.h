

//--Int converter 4(int) -> "04"(String) 
String getString_2digit(int number) {
  if (number < 10) {
    return "0" + String(number);

  } else {
    return String(number);
  }
}
//--------------------------------

// Function to get the current time from RTC and adjust for time zone offset
void getCurrentLocalTime(m5::rtc_datetime_t* localTime, int timeZoneOffsetHours, int timeZoneOffsetMinutes) {
    // Convert rtc_datetime_t to tm struct
    // tm tm_time;
    // tm_time.tm_year = localTime->date.year - 1900;
    // tm_time.tm_mon = localTime->date.month - 1;
    // tm_time.tm_mday = localTime->date.date;
    // tm_time.tm_hour = localTime->time.hours;
    // tm_time.tm_min = localTime->time.minutes;
    // tm_time.tm_sec = localTime->time.seconds;

    // // Adjust for time zone offset
    // time_t time_seconds = mktime(&tm_time) + timeZoneOffsetHours * 3600 + timeZoneOffsetMinutes * 60;
    // tm* adjusted_tm = localtime(&time_seconds);

    // // Update rtc_datetime_t with adjusted time
    // localTime->date.year = adjusted_tm->tm_year + 1900;
    // localTime->date.month = adjusted_tm->tm_mon + 1;
    // localTime->date.date = adjusted_tm->tm_mday;
    // localTime->date.weekDay = adjusted_tm->tm_wday;
    // localTime->time.hours = adjusted_tm->tm_hour;
    // localTime->time.minutes = adjusted_tm->tm_min;
    // localTime->time.seconds = adjusted_tm->tm_sec;
   // Adjust for time zone offset
    tm tm_struct = localTime->get_tm();
    time_t time_seconds = mktime(&tm_struct) + timeZoneOffsetHours * 3600 + timeZoneOffsetMinutes * 60;
    // Convert back to tm struct
    struct tm* adjusted_tm = localtime(&time_seconds);

    // Set rtc_datetime_t with adjusted time
    localTime->set_tm(adjusted_tm);

}

// return weekday as string
String getWeekDay(int weekday) {
  switch (weekday) {
    case 0:
      return "Sunday";
    case 1:
      return "Monday";
    case 2:
      return "Tuesday";
    case 3:
      return "Wednesday";
    case 4:
      return "Thursday";
    case 5:
      return "Friday";
    case 6:
      return "Saturday";
    default:
      return "Err";
  }
}

void  Time_screen() {
  bool firstloadScreen = true;
  int sec;
  while (switchscreen() == false) {

    if (sec != second() | firstloadScreen) {
      firstloadScreen = false;
      // clear the screen, except the toolbar area
      M5.Lcd.fillRect(0, toolbar_height, screen_x, screen_y - toolbar_height, bg_color);
      sec = second();

      localTime = M5.Rtc.getDateTime();
      getCurrentLocalTime(&localTime, timezone_h, timezone_m);

      String timedatenow = getString_2digit(localTime.time.hours) + ":" + getString_2digit(localTime.time.minutes) + ":" + getString_2digit(localTime.time.seconds);
      
      if (current_screen == STICKC) {
        M5.Lcd.setFreeFont(&beta15pt7b);
        M5.Lcd.setCursor(1, 30);
      } else {
        M5.Lcd.setFreeFont(&mishmash21pt7b);
        M5.Lcd.setCursor(13, 32);
      }
      M5.Lcd.setTextColor(txt_color, bg_color);
      M5.Lcd.print(timedatenow);

      
      timedatenow = getString_2digit(localTime.date.date) + "/" + getString_2digit(localTime.date.month) + "/" + String(localTime.date.year);
      if (current_screen == STICKC) {
        M5.Lcd.setFreeFont(&beta10pt7b);
        M5.Lcd.setCursor(1, 66);
        M5.Lcd.print(timedatenow);
      } else {
        // print also weekday on bigger screen
        M5.Lcd.setFreeFont(&beta10pt7b); 
        M5.Lcd.setCursor(13, 78);
        M5.Lcd.print(getWeekDay(localTime.date.weekDay));

        M5.Lcd.setFreeFont(&beta15pt7b); // todo set 15pt font
        M5.Lcd.setCursor(13, 98);
        M5.Lcd.print(timedatenow);
      }
      

    }
  }
}
