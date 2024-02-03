#include "webAPI.h"

SensSetting settings {30};

bool GetSettings(char mac[]){
        HTTPClient http;
        String response;
        char apiUrl[100];

        Serial.printf("\n\nGET request to http://pythoff.com/api/moisture/setting/by/%s/\n", mac);
        sprintf(apiUrl, "http://pythoff.com/api/moisture/setting/by/%s/", mac);
        http.begin(apiUrl);
        http.addHeader("Content-Type", "application/json"); 
        int httpCode = http.GET();

        if (httpCode > 0) { //Check for the returning code
 
            response = http.getString();
            Serial.println(httpCode);
            Serial.println(response);
 
        }else {
 
            Serial.println("Error on HTTP request");
 
        } 

        StaticJsonDocument<500> doc;
        DeserializationError error = deserializeJson(doc, response);
        if (error) {
            Serial.println("deserializeJson() failed: ");
            Serial.println(error.c_str());
         }

        http.end();
        uint32_t id = doc["id"];
        Serial.printf("Setting id == %d\n", id);

        settings.timeToSleep = doc["time_to_sleep_min"];
        return true;
        
}

void POSTDataMeasure(float temp, uint16_t capread, char mac[]){
      HTTPClient http;
      char outputData[128];
      const int capacity = JSON_OBJECT_SIZE(2);

      StaticJsonDocument<capacity>doc;
      doc["moisture"].set(capread);
      doc["temperature"].set(temp);
      serializeJson(doc, outputData);

      char apiUrl[100];
    //   sprintf(apiUrl, "http://pythoff.com/api/sensor/%s/", mac);
      sprintf(apiUrl, "http://pythoff.com/api/moisture/sensor/measure/by/%s/", mac);


      Serial.println("POST request to ");
      Serial.println(apiUrl);
      Serial.println(outputData);
      http.begin(apiUrl);
      http.addHeader("Content-Type", "application/json"); 
      int httpResponseCode = http.POST(outputData);

      if (httpResponseCode > 0)
      {
          String response = http.getString();
          Serial.println(httpResponseCode);
          Serial.println(response);
      }
      else {
 
          Serial.println("Error on HTTP request");
          Serial.println(httpResponseCode);
 
      }
      http.end(); 
  }


uint32_t getNumberSens(){
  // Запрос номера сенсора если флаг не выстален
    HTTPClient http;
    String response;

    Serial.println("GET request to http://pythoff.com/api/sensor/get-name/");
    http.begin("http://pythoff.com/api/sensor/get-name/");
    
    int httpCode = http.GET();

    if (httpCode > 0) { //Check for the returning code
 
          response = http.getString();
          Serial.println(httpCode);
          Serial.println(response);
 
    }else {
 
          Serial.println("Error on HTTP request");
 
    } 

    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, response);
    if (error) {
          Serial.println("deserializeJson() failed: ");
          Serial.println(error.c_str());
    }

    http.end();

    return doc["number"];

  }