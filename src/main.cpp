/************************************************************
[*] include
*************************************************************/
#include "main.h"

extern  SensSetting settings;

RTC_DATA_ATTR int bootCount = 0;
Adafruit_seesaw ss;
WiFiServer server(80);
uint32_t number_sens = 0;
uint8_t flag_rst_ssid = -1;

void IRAM_ATTR resetConfigSSID(){
    Serial.print("\n");
    Serial.println("RESET SSID....");
    flag_rst_ssid = 1;

}

// const char* ssid     = "";
// const char* password = "";


void setup() {
/************************************************************
[*] INIT
*************************************************************/

  ++bootCount;
  uint8_t countLedConnectOk = 5;
  uint8_t countTimeToRstSSID = 10;

  pinMode(LED_STATUS_CONECT_OK, OUTPUT);
  pinMode(LED_STATUS_WIFI_STA, OUTPUT);
  pinMode(LED_ERROR, OUTPUT);
  pinMode(PIN_BTN_RST_SSID, INPUT_PULLUP);

  digitalWrite(LED_STATUS_WIFI_STA, LOW);
  digitalWrite(LED_STATUS_CONECT_OK, LOW);
  digitalWrite(LED_ERROR, LOW);
  
  EEPROM.begin(eepromSize);
  attachInterrupt(digitalPinToInterrupt(PIN_BTN_RST_SSID), resetConfigSSID, FALLING);
  Wire.begin();

  Serial.begin(115200);         // Start the Serial communication
  Serial.println("Boot number: " + String(bootCount));

  uint8_t chipid[6];
  esp_efuse_mac_get_default(chipid);

  char macAddr[12];
  for (byte i = 0; i < 6; ++i)
  {
    char buf[3];
    sprintf(buf, "%2X", chipid[i]);
    strcat(macAddr, buf);
  }
  // String MacAddr = macAddr;
  Serial.print("MAC: "); 
  Serial.println(macAddr);

/************************************************************
[*] SENSOR CONNECT
*************************************************************/
  Serial.println("Soil Sensor!");
  
  if (!ss.begin(0x36)) {
    Serial.println("ERROR! seesaw not found");
    // while(1);
  } else {
    Serial.print("seesaw started! version: ");
    Serial.println(ss.getVersion(), HEX);
  }

/************************************************************
[*] WIFI CONNECT
*************************************************************/
    // EEPROM.readByte(A_FLAG_IS_CONFIG) check if there is nothing in RTC_DATA == power was turned off
  if (flag_rst_ssid != 1 && EEPROM.readByte(A_FLAG_IS_CONFIG) == FLAG_IS_CONFIG){
      Serial.println("\n Time to run reset ssid!");
      
      for (size_t i = 0; i < countTimeToRstSSID; i++)
      {
        digitalWrite(LED_STATUS_WIFI_STA, HIGH);
        delay(200);
        digitalWrite(LED_STATUS_CONECT_OK, HIGH);
        digitalWrite(LED_STATUS_WIFI_STA, LOW);
        delay(200);
        digitalWrite(LED_STATUS_CONECT_OK, LOW);
      }
      
  }

  delay(10);

  Serial.println("\n\n\n");
  Serial.print("A_FLAG_IS_CONFIG == ");
  Serial.println(EEPROM.readByte(A_FLAG_IS_CONFIG));

  if (EEPROM.readByte(A_FLAG_IS_CONFIG) != FLAG_IS_CONFIG || flag_rst_ssid == 1)
  {
      connectWIFI_STA();
  }
  else
  {
      connectSSID();
  }


  Serial.println("\n Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP()); 
  Serial.print("WIFI SSID:\t");
  Serial.println(WiFi.SSID());
  Serial.print("WIFI Status:\t");
  Serial.println(WiFi.status());

  for (size_t i = 0; i < countLedConnectOk; i++)
  {
    digitalWrite(LED_STATUS_CONECT_OK, HIGH);
    delay(800);
    digitalWrite(LED_STATUS_CONECT_OK, LOW);
    delay(200);

  } 

// Serial.println(EEPROM.read(0));

/************************************************************
[*] REQ DATA FROM SENSOR
*************************************************************/
  // product data sensor
  float tempC = ss.getTemp();
  uint16_t capread = ss.touchRead(0);

  //test data sensor
  // float tempC = 230.34;
  // uint16_t capread = 36;

  Serial.print("Temperature: "); Serial.print(tempC); Serial.println("*C");
  Serial.print("Capacitive: "); Serial.println(capread);
  delay(100);

  GetSettings(macAddr);

  Serial.printf("TimeToSleep - %d\n\n", settings.timeToSleep);
  
  POSTDataMeasure(tempC, capread, macAddr);

  EEPROM.end();
  
  Serial.println("Sleep...");
  uint64_t  timeToSleep;
  timeToSleep = settings.timeToSleep * uS_TO_M_FACTOR;
  esp_sleep_enable_timer_wakeup(timeToSleep);
  esp_deep_sleep_start();

}

void loop(){}
// ##############################################################################

