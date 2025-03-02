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
      pCharacteristic->setValue("ERROR");
      pCharacteristic->notify();
      return;
    }

    const char* ssid = doc["ssid"] | "";
    const char* password = doc["password"] | "";

    if (strlen(ssid) == 0 || strlen(password) == 0) {
      Serial->println("Erreur: SSID ou password manquant dans le JSON.");
      pCharacteristic->setValue("ERROR");
      pCharacteristic->notify();
      return;
    }

    Serial->print("SSID: ");
    Serial->println(ssid);
    Serial->print("Password: ");
    Serial->println(password);

    saveWiFiCredentials(ssid, password, pCharacteristic);
  }
}

void WiFiCredentialsCallbacks::saveWiFiCredentials(const char* ssid, const char* password, BLECharacteristic *pCharacteristic) {
  preferences.begin("wifi-creds", false);
  preferences.putString("ssid", ssid);
  preferences.putString("password", password);
  preferences.end();
  Serial->println("Credentials saved successfully");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial->println("\nConnecting");
  unsigned long startAttemptTime = millis();
  const unsigned long timeout = 30000; // 30 secondes

  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < timeout) {
    Serial->print(".");
    delay(300);
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial->println("\nFailed to connect to the WiFi network (timeout)");
    pCharacteristic->setValue("ERROR");
    pCharacteristic->notify();
  } else {
    Serial->println("\nConnected to the WiFi network");
    Serial->print("Local ESP32 IP: ");
    Serial->println(WiFi.localIP());
    pCharacteristic->setValue("OK");
    pCharacteristic->notify();
  }

}
