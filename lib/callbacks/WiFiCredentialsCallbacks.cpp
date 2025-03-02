#include "WiFiCredentialsCallbacks.h"

WiFiCredentialsCallbacks::WiFiCredentialsCallbacks(HardwareSerial* Serial) {
  this->Serial = Serial;
}

void WiFiCredentialsCallbacks::onWrite(BLECharacteristic *pCharacteristic) {
  std::string jsonStringValue = pCharacteristic->getValue();

  if (jsonStringValue.length() > 0) {
    Serial->print("Received value: ");
    Serial->println(jsonStringValue.c_str());

    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, jsonStringValue.c_str());

    if (error) {
      Serial->print(F("deserializeJson() failed: "));
      Serial->println(error.c_str());
      return;
    }

    const char* ssid = doc["ssid"] | "";
    const char* password = doc["password"] | "";

    if (strlen(ssid) == 0 || strlen(password) == 0) {
      Serial->println("Erreur: SSID ou password manquant dans le JSON.");
      return;
    }

    Serial->print("SSID: ");
    Serial->println(ssid);
    Serial->print("Password: ");
    Serial->println(password);

    saveWiFiCredentials(ssid, password);
  }
}

void WiFiCredentialsCallbacks::saveWiFiCredentials(const char* ssid, const char* password) {
  preferences.begin("wifi-creds", false);
  preferences.putString("ssid", ssid);
  preferences.putString("password", password);
  preferences.end();
}
