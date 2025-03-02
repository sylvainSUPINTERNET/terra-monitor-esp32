#include <BLECharacteristic.h>
#include "HardwareSerial.h"
#include <Preferences.h>
#include <ArduinoJson.h>



class WiFiCredentialsCallbacks : public BLECharacteristicCallbacks {
    public: 
        WiFiCredentialsCallbacks(HardwareSerial* Serial) {
            this->Serial = Serial;
        }
    protected:
        HardwareSerial* Serial;
        Preferences preferences;

    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string jsonStringValue = pCharacteristic->getValue();
  
      if (jsonStringValue.length() > 0) {
        std::string value = jsonStringValue.c_str();
        Serial->print("Received value: ");
        Serial->println(value.c_str());

        StaticJsonDocument<512> doc;
        DeserializationError error = deserializeJson(doc, value.c_str());
        if (error) {
          Serial->print(F("deserializeJson() failed: "));
          Serial->println(error.c_str());
          return;
        } else {
            const char* ssid = doc["ssid"];
            const char* password = doc["password"];
            Serial->print("SSID: ");
            Serial->println(ssid);
            Serial->print("Password: ");
            Serial->println(password);
            saveWiFiCredentials(ssid, password);
        }
      }
    }

    void saveWiFiCredentials(const char* ssid, const char* password) {
        preferences.begin("wifi-creds", false);
        preferences.putString("ssid", ssid);
        preferences.putString("password", password);
        preferences.end();
    }

  };
  