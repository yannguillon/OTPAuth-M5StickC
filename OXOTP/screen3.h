void Wifi_screen() {


  //----- generate SSID for wifi AP
  char AP_ssid[13];

  String macaddr = String(WiFi.macAddress());
  macaddr.replace(":", "");
  macaddr = macaddr.substring(6);
  String ssid_mac = "OXOTP_" + macaddr;
  ssid_mac.toCharArray(AP_ssid, 13);

  String pass_gen = "";
  
  //----- generate password for wifi
  for (int i = 0; i < 8; i++) {
    pass_gen += rondom_letters[random(0, rondom_letters.length() - 1)];
  }
  

  String pass_static = ""; // Default password
  pass_static = NVS.getString("wifi_password");
  if (pass_static == "") {
    pass_static = "1A2B3C4D5E"; // fallback to an unsecure password
  }

  //0 : random password mode, 1 : static password mode
  passwordMode = NVS.getInt("passwordMode");


  Wifi_Mode = NVS.getString("wifiMode"); // Default mode
  // if the mode is not set, default to AP mode
  if (Wifi_Mode == "") {
    Wifi_Mode = "AP";
  }


  if (Wifi_Mode == "AP") {
    Wifi_SSID = ssid_mac;
  } else {
    Wifi_SSID = NVS.getString("wifi_ssid");
  }
  


  // Use the saved settings
  if (passwordMode == 0 && Wifi_Mode == "AP") {
    Serial.println("Using random password mode");
    Wifi_PASSWORD = pass_gen;
  } else {
    Serial.println("Using static password mode");
    Wifi_PASSWORD = pass_static;
  }

  char password[Wifi_PASSWORD.length() + 1];
  Wifi_PASSWORD.toCharArray(password, Wifi_PASSWORD.length() + 1);

  if (Wifi_Mode == "AP") {
    Serial.println("Starting in AP mode");
    WiFi.mode(WIFI_AP);
    WiFi.softAP(AP_ssid, password);
  } 
  else if (Wifi_Mode != "AP" && Wifi_SSID != "") {
    Serial.println("Starting in STA mode");
    WiFi.mode(WIFI_STA);
    WiFi.begin(Wifi_SSID, password);
    Serial.println("Try Connecting to " + Wifi_SSID + " with password " + password);

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

    WiFi.mode(WIFI_AP);
    WiFi.softAP(AP_ssid, password);
    Serial.println("AP mode started, ssid: " + String(AP_ssid) + ", password: " + password);
  }



  //M5.Lcd.fillRect(0, 17, 160, 63, BLACK);
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.drawXBitmap(60, 22, wait_icon, wait_icon_width, wait_icon_height, TFT_WHITE, TFT_BLACK);      // wait icon

  M5.Lcd.fillRect(60, 22, wait_icon_width, wait_icon_height, BLACK);
  M5.Lcd.setFreeFont(&beta10pt7b);
  M5.Lcd.setCursor(9, 36);
  M5.Lcd.print ("CONNECT TO:");
  M5.Lcd.setFreeFont(&beta8pt7b);
  M5.Lcd.setCursor(10, 60);
  M5.Lcd.print (Wifi_SSID);


  M5.Lcd.setCursor(10, 90);
  // we don't want to show the password in the screen if not in AP mode
  if (Wifi_Mode == "AP") {
    M5.Lcd.print (Wifi_PASSWORD);
    M5.Lcd.setFreeFont(&beta5pt7b);
    // print ip
    M5.Lcd.setCursor(10, 110);
    // print the ip address in the screen
    M5.Lcd.print ("192.168.4.1");

  } else {
    // print just the ip
    M5.Lcd.print (WiFi.localIP().toString());
  }

  //----- start server
  server.enableCORS();
  server.enableCrossOrigin();

    // Route for serving the zipped HTML page
  server.on("/", HTTP_GET, []() {
    // Send the zipped HTML page as a response
    server.sendHeader("Content-Encoding", "gzip");
    server.send_P(200, "text/html", index_html_gz, sizeof(index_html_gz));
  });

  // Route to load style.css file
  server.on("/css/pico.violet.min.css", HTTP_GET, []() {
    server.sendHeader("Content-Encoding", "gzip");
    server.send_P(200, "text/css", css_gz, sizeof(css_gz));
  });



  server.handleClient();





  //----------------add-------------------

  // preflicht OPTIONS request for the /add endpoint
  server.on("/add", HTTP_OPTIONS, []() {
    server.send(200);
  });
  server.on("/add", HTTP_POST, []() {
    jsondata.clear();
    previousMillis = millis();
    String pincode = NVS.getString ("pincode");
    if (pin_UNLOCK == pincode) {

      deserializeJson(jsondata, server.arg(0));
      int hmac_length = jsondata["hmac_length"];
      uint8_t hmacKey[hmac_length] = {};

      for (int i = 0; i < hmac_length; i++) {
        hmacKey[i] = jsondata["data"][i];

        Serial.println("key ' " + String(i) + "'" + hmacKey[i]);
      }

      int id = jsondata["id"];

      String otpBool = jsondata["id"];
      otpBool = "B" + otpBool;

      if (NVS.getInt (otpBool) == 0) {

        NVS.setInt (otpBool, 1);

        String otpUser = jsondata["id"];
        otpUser = "U" + otpUser;
        NVS.setString (otpUser, jsondata["user"]);

        String otpLabel = jsondata["id"];
        otpLabel = "L" + otpLabel;
        NVS.setString (otpLabel, jsondata["label"]);

        NVS.setBlob(jsondata["id"], hmacKey, hmac_length);

        // server.sendHeader("Access-Control-Allow-Origin", "*");
        // server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
        server.send(200, " text/html", "OK");

      } else {
        server.send(200, " text/html", "ALREADY EXIST");
      }
    } else {
      server.send(200, " text/html", "LOCKED");
    }
  });



  //----------------delete-------------------

  // Handle preflight OPTIONS request for the /delete endpoint
  server.on("/delete", HTTP_OPTIONS, []() {
    server.send(200);
  });

  server.on("/delete", HTTP_POST, []() {
    previousMillis = millis();
    jsondata.clear();

    String pincode = NVS.getString ("pincode");
    if (pin_UNLOCK == pincode) {

      // server.sendHeader("Access-Control-Allow-Origin", "*");
      // server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
      server.send(200, " text/html", "OK");


      deserializeJson(jsondata, server.arg(0));

      int id = jsondata["id"];

      String  otpBool = jsondata["id"];
      otpBool = "B" + otpBool;


      String otpUser = jsondata["id"];
      otpUser = "U" + otpUser;

      String otpLabel = jsondata["id"];
      otpLabel = "L" + otpLabel;

      NVS.erase(otpBool);
      NVS.erase(otpLabel);
      NVS.erase(otpUser);
      NVS.erase(String(id));


    } else {
      server.send(200, " text/html", "LOCKED");
    }
  });



  //----------------factoryReset--------------

  // Handle preflight OPTIONS 
  server.on("/factoryReset", HTTP_OPTIONS, []() {
    server.send(200);
  });

  server.on("/factoryReset", HTTP_POST, []() {
    jsondata.clear();
    previousMillis = millis();
    String pincode = NVS.getString ("pincode");
    if (pin_UNLOCK == pincode) {
      deserializeJson(jsondata, server.arg(0));
      String  data1 = jsondata["data"];

      // server.sendHeader("Access-Control-Allow-Origin", "*");
      // server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");

      server.send(200, " text/html", "OK");
      NVS.eraseAll();
      ESP.restart();
    } else {
      server.send(200, " text/html", "LOCKED");
    }
  });



  //----------------check_pin_secure--------------       check if there is a pin in OXOTP
  // handle preflight OPTIONS request
  server.on("/check_pin_secure", HTTP_OPTIONS, []() {
    server.send(200);
  });

  server.on("/check_pin_secure", HTTP_POST, []() {
    previousMillis = millis();
    jsondata.clear();
    String  pincode = NVS.getString ("pincode");

    if (pincode == "") {
      server.send(200, " text/html", "NOT");
    } else {      
      // server.sendHeader("Access-Control-Allow-Origin", "*");
      // server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
      server.send(200, " text/html", "OK");
    }
  });



  //----------------check_pin_LOCK--------------         check if OXOTP is LOCKED
  // handle preflight OPTIONS request
  server.on("/check_pin_LOCK", HTTP_OPTIONS, []() {
    server.send(200);
  });
  server.on("/check_pin_LOCK", HTTP_POST, []() {
    previousMillis = millis();
    jsondata.clear();
    String  pincode = NVS.getString ("pincode");

    if (pin_UNLOCK == pincode) {
      // server.sendHeader("Access-Control-Allow-Origin", "*");
      // server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
      server.send(200, " text/html", "UNLOCKED");
    } else {
      server.send(200, " text/html", "LOCKED");
    }
  });



  //----------------set_pin--------------------           set pin to OXOTP
  // handle preflight OPTIONS request
  server.on("/set_pin", HTTP_OPTIONS, []() {
    server.send(200);
  });
  server.on("/set_pin", HTTP_POST, []() {
    previousMillis = millis();
    jsondata.clear();
    deserializeJson(jsondata, server.arg(0));
    String  pin = jsondata["newpin"];
    String  pincode = NVS.getString ("pincode");

    if (pin_UNLOCK == pincode) {
      pin_UNLOCK = pin;
      NVS.setString ("pincode", pin);
      // server.sendHeader("Access-Control-Allow-Origin", "*");
      // server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
      server.send(200, " text/html", "OK");
    } else {
      server.send(200, " text/html", "LOCKED");
    }


  });
  //----------------UNLOCK--------------                UNLOCK the OXOTP
  // handle preflight OPTIONS request
  server.on("/UNLOCK", HTTP_OPTIONS, []() {
    server.send(200);
  });
  server.on("/UNLOCK", HTTP_POST, []() {
    previousMillis = millis();
    jsondata.clear();
    deserializeJson(jsondata, server.arg(0));
    String  pin = jsondata["pin"];
    String  pincode = NVS.getString ("pincode");


    if (pin == pincode) {

      pin_UNLOCK = pin;
      // server.sendHeader("Access-Control-Allow-Origin", "*");
      // server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
      server.send(200, " text/html", "OK");
    } else {
      server.send(200, " text/html", "LOCKED");
    }
  });


  //----------------unix--------------                 set time with UNIX
  // handle preflight OPTIONS request
  server.on("/unix", HTTP_OPTIONS, []() {
    server.send(200);
  });
  server.on("/unix", HTTP_POST, []() {
    previousMillis = millis();
    jsondata.clear();
    String pincode = NVS.getString ("pincode");
    if (pin_UNLOCK == pincode) {

      deserializeJson(jsondata, server.arg(0));
      time_t unix = jsondata["unix"];
      setTime(unix);

      TimeStruct.hours   = hour();
      TimeStruct.minutes = minute();
      TimeStruct.seconds = second();
      DateStruct.month = month();
      DateStruct.date = day();
      DateStruct.year = year();
      M5.Rtc.setDate(&DateStruct);
      M5.Rtc.setTime(&TimeStruct);

      server.send(200, "text/html", "OK");
    } else {
      server.send(200, "text/html", "LOCKED");
    }
  });



  //----------------getOTPs--------------              get the OXOTP list
  server.on("/getOTPs", HTTP_GET, []() {
    previousMillis = millis();
    jsondata.clear();

    int  json_counter = 0;
    JsonArray data = jsondata.createNestedArray("OTPs");

    for (int  i = 0; i < maxOTPs; i++) {

      String  otpBool = "B" + String ((i + 1));
      int  id = NVS.getInt (otpBool);

      if (id == 1) {
        jsondata["OTPs"][json_counter]["id"] = (i + 1);
        String  otpLabel = "L" + String ((i + 1));
        jsondata["OTPs"][json_counter]["L"] = NVS.getString (otpLabel);
        String  otpUser = "U" + String ((i + 1));
        jsondata["OTPs"][json_counter]["U"] = NVS.getString (otpUser);
        json_counter++;
      }
    }

    String  dataout = "";
    serializeJson(jsondata, dataout);

    server.send(200, "text/html", dataout);
  });


  // -- Set WiFi endpoint. Payload is a JSON object with the following fields:
  
    // "mode" (string): "AP" or "STA"
    // "ssid" (string): SSID of the network to connect to
    // "password" (string): Password of the network to connect to or the password for the AP
    // "passwordMode" (bool): true or false

  

  server.on("/setWifi", HTTP_OPTIONS, []() {
    server.send(200);
  });

server.on("/setWifi", HTTP_POST, []() {
    previousMillis = millis();
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
  server.on("/getWifi", HTTP_GET, []() {
      previousMillis = millis();
      jsondata.clear();

      jsondata["mode"] = NVS.getString("wifiMode");
      jsondata["ssid"] = NVS.getString("wifi_ssid");
      jsondata["passwordMode"] = NVS.getInt("passwordMode", 1);

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
