


// #include <WiFi.h>
// #include <AsyncTCP.h>
// #include <ESPAsyncWebServer.h>
// #include <DNSServer.h>
// #include <Preferences.h>

// #define DNS_PORT 53

// AsyncWebServer server(80);
// DNSServer dnsServer;
// const IPAddress apIP(192, 168, 4, 1);
// const char* ESP_SSID = "ESP32_PORTAIL_1337";
// Preferences preferences;

// bool shouldConnect = false;

// void setup() {
//   Serial.begin(115200);
//   while(!Serial);
//   delay(2000); // Laisse le temps au port série et USB de bien se remettre

//   // Boost TX power
//   // WiFi.setTxPower(WIFI_POWER_19_5dBm);
  
//   //Check if we have existing Preference
//   preferences.begin("wifi", false);
//   String ssid = preferences.getString("ssid", "");
//   String password = preferences.getString("password", "");
//   preferences.end();
  
//   if (ssid.length() > 0 && password.length() > 0) {
//     Serial.println("📡 using preferences SSID: " + ssid);
//     Serial.println("🔑 using preferences Password: *******");

//     WiFi.begin(ssid.c_str(), password.c_str());

//     Serial.println("Attempt to connect with preferences ...");
//     int attempts = 0;
//     while (WiFi.status() != WL_CONNECTED && attempts < 50) {
//       delay(500);
//       Serial.print(".");
//       attempts++;
//     }
//     if (WiFi.status() == WL_CONNECTED) {
//       Serial.println("Connected using preferences ! IP: " + WiFi.localIP().toString());
//       return;
//     } else {
//       Serial.println("\nFailed to connect from existing preferences.");
//       // clean preferences
//       preferences.begin("wifi", false);
//       preferences.clear();
//       preferences.end();
//     }
//   }


//   //Check if we have existing Preference
//   Serial.println("📡 No preferences found. Starting default mode ...");
//   WiFi.disconnect();
//   WiFi.mode(WIFI_AP_STA);
//   WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
//   WiFi.softAP(ESP_SSID, nullptr, 6);
//   Serial.print("🚀 AP started. IP : ");
//   Serial.println(WiFi.softAPIP());

//   dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
//   dnsServer.start(DNS_PORT, "*", apIP);
//   Serial.println("📡 DNS Server started");

//   server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
//     Serial.println("📥 Requête sur /");
//     request->send(200, "text/html", R"rawliteral(
//       <!DOCTYPE html>
//       <html><body>
//       <h1>Configuration Wi-Fi</h1>
//       <p>Si cette page ne s'ouvre pas auto, allez à 192.168.4.1</p>
//       <form action="/config" method="post">
//         SSID: <input type="text" name="ssid"><br>
//         Password: <input type="password" name="password"><br>
//         <input type="submit" value="Envoyer">
//       </form>
//       </body></html>
//     )rawliteral");
//   }).setFilter(ON_AP_FILTER);

//   server.on("/config", HTTP_POST, [](AsyncWebServerRequest *request){
//     String ssid = request->arg("ssid");
//     String password = request->arg("password");

//     Serial.println("📡 SSID: " + ssid);
//     Serial.println("🔑 Password: *******");

//     delay(1000);

//     if (ssid.length() == 0 || password.length() == 0) {
//       request->send(400, "text/html", "<h1>Bad Request</h1>");
//       return;
//     }

//     shouldConnect = true;
//     preferences.begin("wifi", false);
//     preferences.putString("ssid", ssid);
//     preferences.putString("password", password);
//     preferences.end();
//     request->send(200, "text/html", "<h1>Connecting ...</h1>");

//   }).setFilter(ON_AP_FILTER);

//   server.onNotFound([](AsyncWebServerRequest *request){
//     Serial.println("Unknown endpoint : " + request->url());
//     request->send(200, "text/html", "<h1>Unknown</h1>");
//   });
//   server.begin();
//   Serial.println("Server started");
// }



// void loop() {
//   dnsServer.processNextRequest();
  
//   if ( shouldConnect ) {
//     shouldConnect = false;
//     preferences.begin("wifi", false);
//     String ssid = preferences.getString("ssid", "");
//     String password = preferences.getString("password", "");
//     preferences.end();

//     WiFi.begin(ssid.c_str(), password.c_str());
//     Serial.println("Connecting ...");
//     int attempts = 0;
//     while (WiFi.status() != WL_CONNECTED && attempts < 50) {
//       delay(500);
//       Serial.print(".");
//       attempts++;
//     }
//     if (WiFi.status() == WL_CONNECTED) {
//       Serial.println("Connected ! IP: " + WiFi.localIP().toString());
//     } else {
//       Serial.println("Failed to connect. Restarting ESP ...");
//       ESP.restart();
//     }
//   }

// }



// void loop() {
//   dnsServer.processNextRequest();

//   try {

//     if ( shouldConnect ) {
//       shouldConnect = false;
//       Serial.println("🔗 Reconnexion ...");
//       WiFi.reconnect();
//     }

//   } catch (const std::exception& e) {
//     Serial.println("Exception caught in loop : " + String(e.what()));
//   }
  
// }


// #include <WiFi.h>
// #include <AsyncTCP.h>
// #include <ESPAsyncWebServer.h>
// #include <DNSServer.h>

// #define DNS_PORT 53

// AsyncWebServer server(80);
// DNSServer dnsServer;

// void setup() {
//   Serial.begin(115200);

//   WiFi.mode(WIFI_AP);
//   WiFi.softAP("ESP32_SETUP");
//   Serial.print("🚀 AP lancé. IP : ");
//   Serial.println(WiFi.softAPIP());

//   dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());

//   server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
//     request->send(200, "text/html", R"rawliteral(
//       <!DOCTYPE html>
//       <html><body>
//       <h1>Configuration Wi-Fi</h1>
//       <form action="/config" method="post">
//         SSID: <input type="text" name="ssid"><br>
//         Password: <input type="password" name="password"><br>
//         <input type="submit" value="Envoyer">
//       </form>
//       </body></html>
//     )rawliteral");
//   }).setFilter(ON_AP_FILTER);

//   server.on("/config", HTTP_POST, [](AsyncWebServerRequest *request){
//     String ssid = request->arg("ssid");
//     String password = request->arg("password");

//     Serial.println("📡 SSID: " + ssid);
//     Serial.println("🔑 Password: " + password);

//     request->send(200, "text/html", "<h1>Merci ! Connexion...</h1>");

//     delay(1000);

//     WiFi.softAPdisconnect(true);
//     WiFi.mode(WIFI_STA);
//     WiFi.begin(ssid.c_str(), password.c_str());

//     Serial.println("🔄 Tentative de connexion...");
//   }).setFilter(ON_AP_FILTER);

//   // Intercepter les requêtes de détection spécifiques
//   server.on("/generate_204", HTTP_GET, [](AsyncWebServerRequest *request){
//     Serial.println("📥 Requête Android détectée (/generate_204)");
//     request->redirect("http://192.168.4.1/");
//   }).setFilter(ON_AP_FILTER);

//   server.on("/hotspot-detect.html", HTTP_GET, [](AsyncWebServerRequest *request){
//     Serial.println("📥 Requête iOS détectée (/hotspot-detect.html)");
//     request->redirect("http://192.168.4.1/");
//   }).setFilter(ON_AP_FILTER);

//   server.on("/ncsi.txt", HTTP_GET, [](AsyncWebServerRequest *request){
//     Serial.println("📥 Requête Windows détectée (/ncsi.txt)");
//     request->redirect("http://192.168.4.1/");
//   }).setFilter(ON_AP_FILTER);

//   // Gestion des requêtes non trouvées
//   server.onNotFound([](AsyncWebServerRequest *request){
//     if (WiFi.getMode() & WIFI_AP) {
//       Serial.println("📥 Requête inconnue : " + request->url());
//       request->redirect("http://192.168.4.1/");
//     } else {
//       request->send(404, "text/plain", "Not Found");
//     }
//   });


//   server.begin();
//   Serial.println("Server started");
// }

// void loop() {
//   dnsServer.processNextRequest();
// }













// #include <Arduino.h>
// #include <WiFi.h>
// #include <Wire.h>
// #include <BLEDevice.h>
// #include <BLEUtils.h>
// #include <BLEServer.h>
// #include <string>
// #include <Adafruit_Sensor.h>
// #include <Adafruit_BME280.h>
// #include "WiFiCredentialsCallbacks.h"
// #include <WebServer.h>
// #include <DNSServer.h>


// void handleConfig() {
//   if (server.hasArg("ssid") && server.hasArg("password")) {
//     String ssid = server.arg("ssid");
//     String password = server.arg("password");
    
//     Serial.println("SSID: " + ssid);
//     //Serial.println("Password: " + password);

//     server.send(200, "text/plain", "OK");

//     delay(1000);

//     WiFi.softAPdisconnect(true);
//     WiFi.mode(WIFI_STA);
//     WiFi.begin(ssid.c_str(), password.c_str());    
//   } else {
//     server.send(400, "text/plain", "Bad Request");
//   }
// }

// void setup() {
//   Serial.begin(115200);
//   while(!Serial);
  
//   WiFi.mode(WIFI_AP);
//   WiFi.softAP("ESP32_SETUP", "12345678");
//   Serial.print("AP WIFI - IP ESP32 : ");
//   Serial.println(WiFi.softAPIP());
//   dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
//   dnsServer.setTTL(300);
//   dnsServer.start(53, "*", WiFi.softAPIP());

//   server.on("/setup", HTTP_POST, handleConfig);
//   server.on("/", HTTP_GET, [](){

//     const char* html_page =
//         "<!DOCTYPE html>"
//         "<html>"
//         "<head><title>ESP32 Setup</title></head>"
//         "<body>"
//         "<h1>Bienvenue sur l'ESP32</h1>"
//         "<form action=\"/setup\" method=\"post\">"
//         "SSID: <input type=\"text\" name=\"ssid\"><br>"
//         "Password: <input type=\"password\" name=\"password\"><br>"
//         "<input type=\"submit\" value=\"Envoyer\">"
//         "</form>"
//         "</body>"
//         "</html>";

//         /*
//         const char index_html[] PROGMEM = R"rawliteral(
//           <!DOCTYPE html>
//           <html>
//           <head>
//             <title>Captive Portal</title>
//           </head>
//           <body>
//             <h1>Welcome to the Captive Portal</h1>
//             <p>Please agree to the terms to access the internet.</p>
//             <form>
//               <input type="checkbox" id="terms" name="terms">
//               <label for="terms">I agree to the terms of service</label><br>
//               <input type="submit" value="Continue">
//             </form>
//           </body>
//           </html>
//           )rawliteral";
//         */

//     server.send(200, "text/html", html_page);
//   });


//   server.onNotFound([](){
//     server.send(200, "text/plain", "<h1>Page du portail captif</h1>");
//   });


//   server.begin();
//   Serial.println("Web server started");

  
// }

// void loop() {
//   dnsServer.processNextRequest();
//   server.handleClient();
// }

// #define SDA_PIN 17
// #define SCL_PIN 18

// Adafruit_BME280 bme; // I2C


// class MyServerCallbacks: public BLEServerCallbacks {
//   void onConnect(BLEServer* pServer) {
//     Serial.println("✅ Client connected");
//   };

//   void onDisconnect(BLEServer* pServer) {
//     Serial.println("❌ Client disconnected");
//     pServer->getAdvertising()->start();
//   }
// };



// void setup() {
//   const std::string MANDATORY_PREFIX = "TERRA-IOT";
//   Serial.begin(115200);
//   while(!Serial);

//   Serial.println("Start...");


//   // if (!bme.begin()) { 
//   //   Serial.println("Could not find a valid BME280 sensor, check wiring!");
//   // }


//   Serial.println("init BLE service ...");
//   BLEDevice::init(std::string(MANDATORY_PREFIX) + std::string("ESP32"));
//   BLEServer *pServer = BLEDevice::createServer();

//   pServer->setCallbacks(new MyServerCallbacks());

//   const char* serviceWiFiCredsUUID = "4fafc201-1fb5-459e-8fcc-c5c9c331914b";
//   const char* characteristicWiFiCredsUUID = "6e400002-b5a3-f393-e0a9-e50e24dcca9e";

//   BLEService *pServicePairing = pServer->createService(serviceWiFiCredsUUID);
//   BLECharacteristic *pCharacteristicPairing = pServicePairing->createCharacteristic(
//                                          characteristicWiFiCredsUUID,
//                                          BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY);
//   pCharacteristicPairing->setCallbacks(new WiFiCredentialsCallbacks(&Serial));
//   pServicePairing->start();
//   delay(500);
  
//   BLEAdvertising *pAdvertisingPairing = pServer->getAdvertising();
//   pAdvertisingPairing->start();

//   Serial.println("Service : " + String(serviceWiFiCredsUUID));
//   Serial.println("Characteristic : " + String(characteristicWiFiCredsUUID));
//   Serial.println("Characteristic defined! Now you can read/write it in your phone");


//   /*
//   BLEService *pService = pServer->createService("180A");
//   BLECharacteristic *pCharacteristic = pService->createCharacteristic(
//                                          "2A23",
//                                          BLECharacteristic::PROPERTY_READ
//                                        );
//   pCharacteristic->setValue("Hello World");
//   pService->start();

//   BLEAdvertising *pAdvertising = pServer->getAdvertising();
//   pAdvertising->start();
//   */
  
// }

// void loop() {

// }



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
