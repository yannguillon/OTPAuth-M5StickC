
void Wifi_screen() {

  // -------  GET THE WIFI SETTINGS --------- //

  // generate SSID for wifi AP
  char AP_ssid[13];
  String macaddr = String(WiFi.macAddress());
  macaddr.replace(":", "");
  macaddr = macaddr.substring(6);
  String ssid_mac = "M5STICK_" + macaddr;
  ssid_mac.toCharArray(AP_ssid, 13);


  //----- generate password for wifi
  String pass_gen = "";
  for (int i = 0; i < 8; i++) {
    pass_gen += random_letters[random(0, random_letters.length() - 1)];
  }

  String pass_static = "";  // Default password
  pass_static = NVS.getString("wifi_password");
  if (pass_static == "") {
    pass_static = "1A2B3C4D5E";  // fallback to an unsecure password
  }

  //0 : random password mode, 1 : static password mode
  passwordMode = NVS.getInt("passwordMode");
  Wifi_Mode = NVS.getString("wifiMode");  // Default mode

  if (Wifi_Mode == "") {
    Wifi_Mode = "AP";  // if the mode is not set, default to AP mode
  }

  if (Wifi_Mode == "AP") {
    Wifi_SSID = ssid_mac;
  } else {
    Wifi_SSID = NVS.getString("wifi_ssid");
  }

  // get password mode settings
  if (passwordMode == 0 && Wifi_Mode == "AP") {
    Serial.println("Using random password mode");
    Wifi_PASSWORD = pass_gen;
  } else {
    Serial.println("Using static password mode");
    Wifi_PASSWORD = pass_static;
  }

  char password[Wifi_PASSWORD.length() + 1];
  Wifi_PASSWORD.toCharArray(password, Wifi_PASSWORD.length() + 1);


  // clear the screen, except the toolbar area
  M5.Lcd.fillRect(0, toolbar_height, screen_x, screen_y - toolbar_height, bg_color);
  show_wait_icon();


  //-------  START WIFI  --------- //
  if (Wifi_Mode == "AP") {
    Serial.println("Starting in AP mode");
    WiFi.mode(WIFI_AP);
    WiFi.softAP(AP_ssid, password);
  } else if (Wifi_Mode != "AP" && Wifi_SSID != "") {
    Serial.println("Starting in STA mode");
    WiFi.mode(WIFI_STA);
    WiFi.begin(Wifi_SSID, password);
    Serial.println("Try Connecting to " + Wifi_SSID);

    // Wait for the connection to complete
    unsigned long startTime = millis();
    while (WiFi.status() != WL_CONNECTED) {
      delay(250);
      // If the connection doesn't complete within 10 seconds, break the loop
      if (millis() - startTime > 10000) {
        break;
      }
    }
    Serial.println("Connection status: " + String(WiFi.status()));
    // print the ip address in the serial monitor and in the screen
    Serial.println("Connected to " + Wifi_SSID + " with IP: " + WiFi.localIP().toString());
  }


  // If the connection failed, fallback to AP mode
  if (Wifi_Mode != "AP" && WiFi.status() != WL_CONNECTED) {
    Serial.println("Failed to connect to the specified network. Falling back to AP mode.");
    Wifi_SSID = ssid_mac;
    Wifi_Mode = "AP";
    Wifi_PASSWORD = pass_gen;  // generate a new password for fallback AP mode
    WiFi.mode(WIFI_AP);
    WiFi.softAP(AP_ssid, pass_gen);
    Serial.println("AP mode started, ssid: " + String(AP_ssid) + ", password: " + password);
  }

  // use mdns to resolve the hostname
  if (MDNS.begin("m5stick")) {  //http://esp32.local
    Serial.println("mDNS responder started");
    _mdnsStarted = true;
  } else {
    Serial.println("Error setting up MDNS responder!");
    _mdnsStarted = false;
  }


  // clear the wait icon
  clear_wait_icon();

  // print the wifi settings in the screen
  if (current_screen == STICKC) {
    M5.Lcd.setFreeFont(&inter8pt7b);
    M5.Lcd.setCursor(10, 33);
  } else {
    M5.Lcd.setFreeFont(&inter8pt7b);
    M5.Lcd.setCursor(9, 36);
  }
  M5.Lcd.print("CONNECT TO:");


  if (current_screen == STICKC) {
    M5.Lcd.setFreeFont(&inter12pt7b);
    M5.Lcd.setCursor(10, 38);
  } else {
    M5.Lcd.setFreeFont(&inter12pt7b);
    M5.Lcd.setCursor(10, 53);
  }
  M5.Lcd.print(Wifi_SSID);

  if (current_screen == STICKC) {
    M5.Lcd.setFreeFont(&inter12pt7b);
    M5.Lcd.setCursor(10, 52);
  } else {
    M5.Lcd.setFreeFont(&inter12pt7b);
    M5.Lcd.setCursor(10, 80);
  }

  // we don't want to show the password in the screen if not in AP mode
  if (Wifi_Mode == "AP") {
    M5.Lcd.print(Wifi_PASSWORD);
    M5.Lcd.setFreeFont(&inter8pt7b);
    // print ip
    if (current_screen == STICKC) {
      M5.Lcd.setCursor(10, 72);
    } else {
      M5.Lcd.setCursor(10, 108);
    }
    // print the ip address in the screen
    if (_mdnsStarted) {
      M5.Lcd.print("GO TO: m5stick.local");
    } else {
      M5.Lcd.print("192.168.4.1");
    }

  } else {
      M5.Lcd.setFreeFont(&inter8pt7b);
      // print ip
      if (current_screen == STICKC) {
        M5.Lcd.setCursor(10, 72);
      } else {
        M5.Lcd.setCursor(10, 108);
      }
    if (_mdnsStarted) {
      M5.Lcd.print("GO TO: m5stick.local");
    } else {
      M5.Lcd.print(WiFi.localIP().toString());
    }
  }

  //----- start server
  server.enableCORS();
  server.enableCrossOrigin();

  // Route for serving the zipped HTML page
  server.on("/", HTTP_GET, []() {
    previousMillis = millis();
    server.sendHeader("Content-Encoding", "gzip");
    server.send_P(200, "text/html", index_html_gz, sizeof(index_html_gz));
  });

  // Route to load style.css file
  server.on("/tailwind.min.css", HTTP_GET, []() {
    server.sendHeader("Content-Encoding", "gzip");
    server.send_P(200, "text/css", tailwind_min_css_gz, sizeof(tailwind_min_css_gz));
  });

  // Route to load the favicon.ico file
  server.on("/favicon.png", HTTP_GET, []() {
    server.sendHeader("Content-Encoding", "gzip");
    server.send_P(200, "image/png", favicon_png_gz, sizeof(favicon_png_gz));
  });

  server.on("/app.js", HTTP_GET, []() {
    server.sendHeader("Content-Encoding", "gzip");
    server.send_P(200, "text/javascript", app_js_gz, sizeof(app_js_gz));
  });

  server.handleClient();

  //----------------add-------------------

  // preflight OPTIONS request for the /add endpoint
  server.on("/otps", HTTP_OPTIONS, []() {
    server.send(200);
  });
  server.on("/otps", HTTP_POST, []() {
    jsondata.clear();
    previousMillis = millis();
    String pincode = NVS.getString("pincode");
    if (pin_UNLOCK == pincode) {

      deserializeJson(jsondata, server.arg(0));
      int hmac_length = jsondata["hmac_length"];
      uint8_t hmacKey[hmac_length] = {};

      for (int i = 0; i < hmac_length; i++) {
        hmacKey[i] = jsondata["data"][i];
      }

      int id = jsondata["id"];

      String otpBool = jsondata["id"];
      otpBool = "B" + otpBool;

      if (NVS.getInt(otpBool) == 0) {

        NVS.setInt(otpBool, 1);

        String otpUser = jsondata["id"];
        otpUser = "U" + otpUser;
        NVS.setString(otpUser, jsondata["user"]);

        String otpLabel = jsondata["id"];
        otpLabel = "L" + otpLabel;
        NVS.setString(otpLabel, jsondata["label"]);

        NVS.setBlob(jsondata["id"], hmacKey, hmac_length);

        server.send(200, " text/html", "OK");

      } else {
        server.send(200, " text/html", "ALREADY EXISTS");
      }
    } else {
      server.send(200, " text/html", "LOCKED");
    }
  });

  //----------------delete all -------------------

  server.on("/otps", HTTP_DELETE, []() {
    for (int i = 0; i < maxOTPs; i++) {
      String realIndex = String(i + 1);
      NVS.erase(realIndex);
      NVS.erase("B" + realIndex);
      NVS.erase("U" + realIndex);
      NVS.erase("L" + realIndex);
    }

    server.send(200);
  });

  //----------------factoryReset--------------

  // Handle preflight OPTIONS
  server.on("/reset", HTTP_OPTIONS, []() {
    server.send(200);
  });

  server.on("/reset", HTTP_POST, []() {
    server.send(200, " text/html", "OK");
    NVS.eraseAll();
    ESP.restart();
  });

  //----------------unix--------------                 set time with UNIX
  // handle preflight OPTIONS request
  server.on("/config", HTTP_OPTIONS, []() {
    server.send(200);
  });

  server.on("/config/time", HTTP_PATCH, []() {
    previousMillis = millis();
    jsondata.clear();

    deserializeJson(jsondata, server.arg(0));
    time_t unix = jsondata["unix"];
    setTime(unix);

    // from unix to rtc_datetime_t
    tm* tm_struct = gmtime(&unix);

    M5.Rtc.setDateTime(tm_struct);

    server.send(200, "text/html", "OK");
  });

  // get timezone
  server.on("/config/timezone", HTTP_GET, []() {
    previousMillis = millis();
    jsondata.clear();
    // String  pincode = NVS.getString ("pincode");
    // if (pin_UNLOCK != pincode) {
    //   server.send(200, "text/html", "LOCKED");
    //   return;
    // }

    // return the timezone value from timezone_h and timezone_m
    float _timezone = timezone_h + (timezone_m / 60.0);

    jsondata["timezone"] = _timezone;

    String dataout = "";
    serializeJson(jsondata, dataout);

    server.send(200, "text/html", dataout);
  });

  // set the timezone

  server.on("/config/timezone", HTTP_OPTIONS, []() {
    server.send(200);
  });

  server.on("/config/timezone", HTTP_PATCH, []() {
    previousMillis = millis();
    jsondata.clear();
    deserializeJson(jsondata, server.arg(0));

    // set timezone_h and timezone_m from the float value
    float _timezone = jsondata["timezone"];

    if (_timezone < -14 || _timezone > 14) {
      server.send(200, "text/html", "INVALID");
      return;
    }

    timezone_h = static_cast<int>(_timezone);
    timezone_m = static_cast<int>((_timezone - timezone_h) * 60);

    // save the timezone value to the NVS
    NVS.setInt("timezone_h", timezone_h);
    NVS.setInt("timezone_m", timezone_m);

    server.send(200, "text/html", "OK");
  });

  // -----------------setWifi-----------------            set the wifi settings
  server.on("/config/wlan", HTTP_OPTIONS, []() {
    server.send(200);
  });

  server.on("/config/wlan", HTTP_PATCH, []() {
    previousMillis = millis();
    String pincode = NVS.getString("pincode");
    if (pin_UNLOCK != pincode) {
      server.send(200, "text/html", "LOCKED");
      return;
    }
    jsondata.clear();
    deserializeJson(jsondata, server.arg(0));

    if (jsondata.containsKey("mode")) {
      String _mode = jsondata["mode"];
      NVS.setString("wifiMode", _mode);
      Wifi_Mode = _mode;
    }

    if (jsondata.containsKey("ssid")) {
      String _ssid = jsondata["ssid"];
      NVS.setString("wifi_ssid", _ssid);
      Wifi_SSID = _ssid;
    }

    if (jsondata.containsKey("password")) {
      String _pass = jsondata["password"];
      NVS.setString("wifi_password", _pass);
      Wifi_PASSWORD = _pass;
    }

    if (jsondata.containsKey("passwordMode")) {
      int _passMode = jsondata["passwordMode"];
      NVS.setInt("passwordMode", _passMode);
      passwordMode = _passMode;
    }

    server.send(200, "text/html", "OK");
  });

  // endpoint for get the current wifi settings (mode, ssid, passwordMode)
  server.on("/config/wlan", HTTP_GET, []() {
    previousMillis = millis();
    String pincode = NVS.getString("pincode");
    if (pin_UNLOCK != pincode) {
      server.send(200, "text/html", "LOCKED");
      return;
    }

    jsondata.clear();

    jsondata["mode"] = NVS.getString("wifiMode");
    jsondata["ssid"] = NVS.getString("wifi_ssid");
    jsondata["passwordMode"] = NVS.getInt("passwordMode", 1);

    String dataout = "";
    serializeJson(jsondata, dataout);

    server.send(200, "text/html", dataout);
  });

  // Set the LCD Brightness and timeout
  server.on("/config/display", HTTP_OPTIONS, []() {
    server.send(200);
  });

  server.on("/config/display", HTTP_PATCH, []() {
    previousMillis = millis();
    jsondata.clear();
    deserializeJson(jsondata, server.arg(0));

    if (jsondata.containsKey("brightness")) {
      int _brightness = jsondata["brightness"];
      // clamp the brightness value between 10 and 255
      _brightness = constrain(_brightness, 10, 255);
      M5.Lcd.setBrightness(_brightness);
      lcd_brightness = _brightness;

      // save the brightness value to the NVS
      NVS.setInt("lcd_brightness", _brightness);
    }

    if (jsondata.containsKey("timeout")) {
      int _timeout = jsondata["timeout"];
      if (_timeout <= 0) {
        _timeout = -1;
      } else {
        _timeout = _timeout * 1000;
      }

      timeout_ScreenOn = _timeout;
      NVS.setInt("timeout_ScreenOn", _timeout);
      Serial.println("timeout_ScreenOn: " + String(timeout_ScreenOn));

      if (jsondata.containsKey("bg_color")) {
        bg_color = jsondata["bg_color"];
        NVS.setInt("bg_color", bg_color);
      }

      if (jsondata.containsKey("txt_color")) {
        txt_color = jsondata["txt_color"];
        NVS.setString("txt_color", String(txt_color));
      }
      M5.Lcd.fillScreen(bg_color);
      M5.Lcd.setTextColor(txt_color, bg_color);
    }

    server.send(200, "text/html", "OK");
  });
  // Get the current LCD Brightness and timeout in seconds
  server.on("/config/display", HTTP_GET, []() {
    previousMillis = millis();
    jsondata.clear();

    jsondata["brightness"] = NVS.getInt("lcd_brightness");
    int _timeout = timeout_ScreenOn == UINT32_MAX ? 0 : timeout_ScreenOn / 1000;
    jsondata["timeout"] = timeout_ScreenOn / 1000;

    jsondata["bg_color"] = bg_color;
    jsondata["txt_color"] = txt_color;

    String dataout = "";
    serializeJson(jsondata, dataout);

    server.send(200, "text/html", dataout);
  });

  server.begin();
  while (switchscreen() == false) {
    server.handleClient();
  }
  WiFi.mode(WIFI_OFF);
}
