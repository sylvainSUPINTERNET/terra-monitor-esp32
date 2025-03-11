#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>
#include <Preferences.h>

#define SERVER_PORT 80
#define WIFI_SSID_ESP "ESP32_DEVKIT"
#define WIFI_PASSWORD_ESP ""

Preferences preferences;
AsyncWebServer server(SERVER_PORT);


void setup() {
  Serial.begin(115200);
  while(!Serial);
  delay(2500);

  WiFi.setTxPower(WIFI_POWER_19_5dBm);

  // Existing configuration
  preferences.begin("wifi", false);
  String ssid = preferences.getString("ssid", "");
  String password = preferences.getString("password", "");
  preferences.end();
  if ( ssid.length() > 0 && password.length() > 0 ) {
    Serial.println("using preferences SSID: " + ssid);
    Serial.println("using preferences Password: *******");

    Serial.println("Attempt to connect with preferences ...");
    WiFi.begin(ssid, password);
    unsigned long startAttemptTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 25000) { // 15 sec max
      delay(500);
      Serial.print(".");
    }
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("Connected using preferences ! IP: " + WiFi.localIP().toString());
    } else {
      Serial.println("\nFailed to connect from existing preferences.");
      preferences.begin("wifi", false);
      preferences.clear();
      preferences.end();
      Serial.println("Preferences cleared, restart ESP ...");
      ESP.restart();
    }
  }


  // Default
  Serial.println("Starting default mode ...");
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(WIFI_SSID_ESP, WIFI_PASSWORD_ESP);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/html", R"rawliteral(
      <!DOCTYPE html>
      <html>
        <body>
        <h1>TerraIO - Configure WiFi</h1>
        <form action="/config" method="post">
          SSID: <input type="text" name="ssid"><br>
          Password: <input type="password" name="password"><br>
          <input type="submit" value="Envoyer">
        </form>
        </body>
      </html>
    )rawliteral");
  }).setFilter(ON_AP_FILTER);

  server.on("/config", HTTP_POST, [](AsyncWebServerRequest *request) {
    String ssid = request->arg("ssid");
    String password = request->arg("password");

    Serial.println("SSID: " + ssid);
    Serial.println("Password: *******");

    if (ssid.length() == 0 || password.length() == 0) {
      request->send(400, "text/html", "<h1>Bad Request</h1>");
      return;
    }

    preferences.clear();
    preferences.begin("wifi", false);
    preferences.putString("ssid", ssid);
    preferences.putString("password", password);
    preferences.end();
  
    request->send(200, "text/html", "<h1>Attempt to connect, restart ...</h1>");
    
    delay(1000);
    ESP.restart();

  }).setFilter(ON_AP_FILTER);

  server.onNotFound([](AsyncWebServerRequest *request) {
    Serial.println("Unknown endpoint : " + request->url());
    request->send(200, "text/html", "<h1>Unknown</h1>");
  });
  
  Serial.println("Server started port : " + String(SERVER_PORT));

  server.begin();
}

unsigned long previousMillis = 0;
const long interval = 10000; // 10 secondes (ms)

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    // TODO : 
    // CHECK wifi ( try-catch ) is OK and try to send request, if we fail 3 times, we restart ESP
    Serial.println(" send 10s");
  }
}
