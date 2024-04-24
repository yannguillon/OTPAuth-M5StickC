# OTPAuth-M5StickC
![Arduino Build](https://github.com/yannguillon/OTPAuth-M5StickC/actions/workflows/ci.yml/badge.svg)

### Sync your OTPAuth codes directly to your M5Stick only using your iPhone !


## Flash/Build

- #### M5 Burner (Win/Mac/Linux)
  - Install [M5Burner](https://docs.m5stack.com/en/download)
  - Search for OTPAuth-M5StickC
  - Click Download

- #### Flash Download tool (Win)
  - Download the latest [Firmware](https://github.com/yannguillon/OTPAuth-M5StickC/releases/latest) from the release page
  - Install [Flash Download Tool]([https://docs.m5stack.com/en/download](https://www.espressif.com/en/support/download/other-tools))
  - After launch, select **ChipType: ESP32** and** WorkMore: Develop**
  - load the **.bin firmware** in the first element of the list
  - Connect the M5Stick via USB and Click **Start**

- #### Build from source
  - Install [Arduino IDE 2](https://docs.m5stack.com/en/download)
  - Clone the repo and open OTPAuth-M5StickC.ino
  - Import all the dependencies:
    - M5Unified library
    - ArduinoHTTPClient
    - ArduinoMDNS
    - ArduinoJson
    - TOTP-Arduino
  - In boards manager Select M5StickCPlus2
  - Click Upload

## WIKI
_Coming soon_

## License 
This software is licensed under the MIT License, to read the full license <a href="LICENSE" target="_blank">HERE</a>.

## Credits
<h4><i>Thanks: </i><a target="_blank" class="font-bold" href="https://github.com/Hinaser/jscrypto">@Hinaser</a>, contributors of <a target="_blank" class="font-bold" href="https://github.com/Hinaser/jscrypto">Browserify</a>, <a target="_blank" class="font-bold" href="https://github.com/pedrus16">@Pedrus16</a> For the 3D renders</h4>
                <i>Not Thanks: </i>
                <span class="font-bold">W3C</span>
                <a class="font-medium text-blue-600 underline dark:text-blue-500 hover:no-underline" target="_blank" href="https://github.com/w3c/webcrypto/issues/28#issuecomment-1174004465">for deliberately blocking browsers crypto APIs to makers</a></h4>

