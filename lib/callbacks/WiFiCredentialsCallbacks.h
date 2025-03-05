#ifndef WIFI_CREDENTIALS_CALLBACKS_H
#define WIFI_CREDENTIALS_CALLBACKS_H

#include <BLECharacteristic.h>
#include <HardwareSerial.h>
#include <Preferences.h>
#include <ArduinoJson.h>
#include <WiFi.h>

class WiFiCredentialsCallbacks : public BLECharacteristicCallbacks {
  public:
    WiFiCredentialsCallbacks(HardwareSerial* Serial);
    virtual ~WiFiCredentialsCallbacks() {}
  protected:
    HardwareSerial* Serial;
    Preferences preferences;

    void onWrite(BLECharacteristic *pCharacteristic);
    void saveWiFiCredentials(const char* ssid, const char* password,  BLECharacteristic *pCharacteristic);
};

#endif
