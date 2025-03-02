#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <string>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "WiFiCredentialsCallbacks.h"

#define SDA_PIN 17
#define SCL_PIN 18


Adafruit_BME280 bme; // I2C


void setup() {
  const std::string MANDATORY_PREFIX = "TERRA-IOT";
  Serial.begin(115200);
  while(!Serial);

  Serial.println("Start...");


  // if (!bme.begin()) { 
  //   Serial.println("Could not find a valid BME280 sensor, check wiring!");
  // }


  Serial.println("init BLE service...");
  BLEDevice::init(std::string(MANDATORY_PREFIX) + std::string("ESP32"));
  BLEServer *pServer = BLEDevice::createServer();

  pServer->setCallbacks(new BLEServerCallbacks()); // TODO : add callback ?

  const char* serviceWiFiCredsUUID = "4fafc201-1fb5-459e-8fcc-c5c9c331914b";
  const char* characteristicWiFiCredsUUID = "6e400002-b5a3-f393-e0a9-e50e24dcca9e";

  BLEService *pServicePairing = pServer->createService(serviceWiFiCredsUUID);
  BLECharacteristic *pCharacteristicPairing = pServicePairing->createCharacteristic(
                                         characteristicWiFiCredsUUID,
                                         BLECharacteristic::PROPERTY_WRITE);
  pCharacteristicPairing->setCallbacks(new WiFiCredentialsCallbacks(&Serial));
  pServicePairing->start();

  BLEAdvertising *pAdvertisingPairing = pServer->getAdvertising();
  pAdvertisingPairing->start();

  Serial.println("Service : " + String(serviceWiFiCredsUUID));
  Serial.println("Characteristic : " + String(characteristicWiFiCredsUUID));
  Serial.println("Characteristic defined! Now you can read it in your phone!");



  /*
  BLEService *pService = pServer->createService("180A");
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         "2A23",
                                         BLECharacteristic::PROPERTY_READ
                                       );
  pCharacteristic->setValue("Hello World");
  pService->start();

  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();
  */
  
}

void loop() {

}



// void loop() {
//   byte error, address;

//   int nDevices = 0;

//   Wire.begin(17, 18); // Initialise I2C

//   // I2C 7 bits addresses
//   Serial.println("🔎 Recherche de périphériques I2C...");
//   for (address = 1; address < 127; address++) {
//       Wire.beginTransmission(address);
//       error = Wire.endTransmission();

//       if (error == 0) {
//           Serial.print("📍 I2C device trouvé à l'adresse 0x");
//           Serial.println(address, HEX);
//           nDevices++;
//       }
//   }

//   if (nDevices == 0) Serial.println("⚠️ Aucun périphérique I2C trouvé !");
//   else Serial.println("✅ Scan terminé avec succès !");

//   delay(5000);
// }


// const char* ssid = "";
// const char* password = "";

// #define SDA_PIN 21
// #define SCL_PIN 20


// const char* host = "jsonplaceholder.typicode.com";
// const int port = 80;

// WiFiClient client;

// void setup() {
//   Serial.begin(115200);
//   delay(1500);

//   Serial.println("Start...");

//   Wire.begin(SDA_PIN, SCL_PIN); // Initialise I2C
//   Serial.println("\n🔍 Scan I2C en cours...");

//   Serial.println("Hello, world");
  
//   WiFi.mode(WIFI_STA);
//   WiFi.begin(ssid, password);
//   Serial.println("\nConnecting");

//   while(WiFi.status() != WL_CONNECTED){
//     Serial.print(".");
//     delay(100);
//   }

//   Serial.println("\nConnected to the WiFi network");
//   Serial.print("Local ESP32 IP: ");
//   Serial.println(WiFi.localIP());


//   if ( client.connect(host, port) ) {

//     Serial.println("Connected to server successfuly");

//     client.println("GET /todos/1 HTTP/1.1");
//         client.println("Host: " + String(host));
//         client.println("Connection: close");
//         client.println();
    
//         while (client.available() == 0) {
//           delay(10);
//         }
        
//         while (client.available()) {
//           String line = client.readStringUntil('\r');
//           Serial.print(line);
//         }
        
//         client.stop();
//   }


// }

// void loop() {
//   byte error, address;
//   int nDevices = 0;

//   // I2C 7 bits addresses
//   Serial.println("🔎 Recherche de périphériques I2C...");
//   for (address = 1; address < 127; address++) {
//       Wire.beginTransmission(address);
//       error = Wire.endTransmission();

//       if (error == 0) {
//           Serial.print("📍 I2C device trouvé à l'adresse 0x");
//           Serial.println(address, HEX);
//           nDevices++;
//       }
//   }

//   if (nDevices == 0) Serial.println("⚠️ Aucun périphérique I2C trouvé !");
//   else Serial.println("✅ Scan terminé avec succès !");

//   delay(5000);
// }
