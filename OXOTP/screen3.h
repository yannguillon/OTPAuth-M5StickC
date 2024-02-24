  void handleRoot() {
  server.send(200, "text/html", webpage);
}

void Wifi_screen() {

  WiFi.mode(WIFI_AP);

  //M5.Lcd.fillRect(0, 17, 160, 63, BLACK);
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.drawXBitmap(60, 22, wait_icon, wait_icon_width, wait_icon_height, TFT_WHITE, TFT_BLACK);      // wait icon


  String pass_gen = "";
  char password[9];
  char ssid[13];


  // //----- generate password for wifi
  // for (int i = 0; i < 8; i++) {
  //   pass_gen += rondom_letters[random(0, rondom_letters.length() - 1)];
  // }
  pass_gen = "1A2B3C4D"; // static
  
  pass_gen.toCharArray(password, 9);


  //----- generate SSID for wifi
  String macaddr = String(WiFi.macAddress());
  macaddr.replace(":", "");
  macaddr = macaddr.substring(6);
  String ssid_mac = "OXOTP_" + macaddr;
  ssid_mac.toCharArray(ssid, 13);


  //----- set SSID and PASSWORD

  WiFi.softAP(ssid, password);

  Serial.println("AP started, password: " + pass_gen);

  M5.Lcd.fillRect(60, 22, wait_icon_width, wait_icon_height, BLACK);
  M5.Lcd.setFreeFont(&beta10pt7b);
  M5.Lcd.setCursor(10, 36);
  M5.Lcd.print ("CONNECT TO:");
  M5.Lcd.setFreeFont(&beta8pt7b);
  M5.Lcd.setCursor(10, 64);
  M5.Lcd.print (ssid_mac);
  M5.Lcd.setCursor(10, 90);
  M5.Lcd.print (pass_gen);


  //----- start server

  server.enableCORS();
  server.enableCrossOrigin();
  server.on("/", HTTP_GET, handleRoot);
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



  server.begin();
  while (switchscreen() == false) {
    server.handleClient();


  }
  WiFi.mode(WIFI_OFF);
}
