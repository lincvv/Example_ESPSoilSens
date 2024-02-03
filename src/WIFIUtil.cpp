#include "WIFIUtil.h"

uint8_t start_addr_ssid = 5;
uint8_t start_addr_pass = 20; 
uint8_t eepromSize = 32;

void loopConnectEspTouch(void){
      Serial.println("Connect EspTouch");
      while (!WiFi.smartConfigDone())
      {
        digitalWrite(LED_STATUS_WIFI_STA, HIGH);
        delay(300);
        digitalWrite(LED_STATUS_WIFI_STA, LOW);
        delay(500);
        Serial.print(".");
      }
}

void connectWIFI_STA(){
      EEPROM.begin(eepromSize);
      WiFi.mode(WIFI_STA);
      WiFi.beginSmartConfig();
      loopConnectEspTouch();

      wl_status_t status = WiFi.status();
      
      if (status != WL_CONNECTED)
      {
        EEPROM.writeByte(A_FLAG_IS_CONFIG, FLAG_IS_NOT_CONFIG);
        EEPROM.commit();
        EEPROM.end();
        Serial.print("\n");
        Serial.printf("STATUS == %d\n", status);

        digitalWrite(LED_ERROR, HIGH);
        digitalWrite(LED_STATUS_WIFI_STA, HIGH);
        delay(300);
        digitalWrite(LED_ERROR, LOW);
        digitalWrite(LED_STATUS_WIFI_STA, LOW);
        delay(200);
        digitalWrite(LED_ERROR, HIGH);

        delay(5000);
        ESP.restart();
      }
        
      String resSSID = WiFi.SSID();
      String resPASS = WiFi.psk();
      
      int8_t lssid = resSSID.length();
      int8_t lps = resPASS.length();
      
      EEPROM.write(A_LENGHT_SSID, lssid);
      EEPROM.write(A_LENGHT_PASS, lps);
      EEPROM.writeByte(A_FLAG_IS_CONFIG, FLAG_IS_CONFIG);
      EEPROM.writeString(start_addr_ssid, resSSID);
      EEPROM.writeString((start_addr_ssid + lssid + 1), resPASS);
      EEPROM.commit();
      EEPROM.end();
}

void connectSSID(){
      EEPROM.begin(eepromSize);
      String _ssid = EEPROM.readString(start_addr_ssid);
      String _pass = EEPROM.readString(start_addr_ssid + EEPROM.read(A_LENGHT_SSID) + 1);

      Serial.println("Currently connected to:");
      Serial.println(_ssid);
      Serial.println(_pass);

      WiFi.begin(_ssid.c_str(), _pass.c_str());             // Connect to the network
      Serial.print("Connecting to ");
      Serial.print(_ssid); Serial.println(" ...");

      int i = 0;
      while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
        ++i;
        digitalWrite(LED_STATUS_CONECT_OK, HIGH);
        digitalWrite(LED_ERROR, HIGH);
        delay(100);
        digitalWrite(LED_STATUS_CONECT_OK, LOW);
        digitalWrite(LED_ERROR, LOW);
        delay(200);
        digitalWrite(LED_STATUS_CONECT_OK, HIGH);
        digitalWrite(LED_ERROR, HIGH);
        delay(100);
        digitalWrite(LED_STATUS_CONECT_OK, LOW);
        delay(600);
        digitalWrite(LED_ERROR, LOW);

        Serial.print(i); Serial.print(' ');

        if(i == WAITE_SSID_CONNECT){
          EEPROM.end();
          ESP.restart();
        }
      }
    EEPROM.end();
    }