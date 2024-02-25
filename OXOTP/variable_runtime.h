
// Run-Time Variables //
StaticJsonDocument<2000> jsondata;

m5::rtc_time_t TimeStruct;
m5::rtc_date_t DateStruct;


unsigned long previousMillis = 0;

int menu_index = 0;

String pin_UNLOCK = "";
int passwordMode = 0; // 0 random, 1 fixed
String Wifi_Mode = "AP"; // Default mode
String Wifi_SSID = "";
String Wifi_PASSWORD = "";