#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>

const char* ssid = "";
const char* password = "";

#define SDA_PIN 21
#define SCL_PIN 20


const char* host = "jsonplaceholder.typicode.com";
const int port = 80;

WiFiClient client;

void setup() {
  Serial.begin(115200);
  delay(1500);

  Serial.println("Start...");

  Wire.begin(SDA_PIN, SCL_PIN); // Initialise I2C
  Serial.println("\n🔍 Scan I2C en cours...");

  // Serial.println("Hello, world");
  
  // WiFi.mode(WIFI_STA);
  // WiFi.begin(ssid, password);
  // Serial.println("\nConnecting");

  // while(WiFi.status() != WL_CONNECTED){
  //   Serial.print(".");
  //   delay(100);
  // }

  // Serial.println("\nConnected to the WiFi network");
  // Serial.print("Local ESP32 IP: ");
  // Serial.println(WiFi.localIP());


  // if ( client.connect(host, port) ) {

  //   Serial.println("Connected to server successfuly");

  //   client.println("GET /todos/1 HTTP/1.1");
  //       client.println("Host: " + String(host));
  //       client.println("Connection: close");
  //       client.println();
    
  //       while (client.available() == 0) {
  //         delay(10);
  //       }
        
  //       while (client.available()) {
  //         String line = client.readStringUntil('\r');
  //         Serial.print(line);
  //       }
        
  //       client.stop();
  // }


}

void loop() {
  byte error, address;
  int nDevices = 0;

  // I2C 7 bits addresses
  Serial.println("🔎 Recherche de périphériques I2C...");
  for (address = 1; address < 127; address++) {
      Wire.beginTransmission(address);
      error = Wire.endTransmission();

      if (error == 0) {
          Serial.print("📍 I2C device trouvé à l'adresse 0x");
          Serial.println(address, HEX);
          nDevices++;
      }
  }

  if (nDevices == 0) Serial.println("⚠️ Aucun périphérique I2C trouvé !");
  else Serial.println("✅ Scan terminé avec succès !");

  delay(5000);
}

// // put function declarations here:
// int myFunction(int, int);

// void setup() {
//   // put your setup code here, to run once:
//   int result = myFunction(2, 3);
// }

// void loop() {
//   // put your main code here, to run repeatedly:
// }

// // put function definitions here:
// int myFunction(int x, int y) {
//   return x + y;
// }