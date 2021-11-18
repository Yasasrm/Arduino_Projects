#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#define onboardLED 2

const char* ssid     = "SLTH";
const char* password = "nicwifi1122";

WiFiServer server(80);

void setup()
{
  Serial.begin(115200);
  pinMode(onboardLED, OUTPUT);      // set the LED pin mode

  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();

}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin("https://hsm272e.herokuapp.com/monitor/hsts");
    http.addHeader("Authorization", "Basic eWFzYXNybTphMXMyZDNmNA==");
    http.addHeader("Content-Type", "application/json");
    int responseCode = http.GET();
    if (responseCode > 0) {
      Serial.println("Response Code: " + String(responseCode));
      String payload = http.getString();
      Serial.println("payload: " + payload);
      payload.replace(" ", "");
      payload.replace("\n", "");
      payload.trim();
      //payload.remove(0,1);
      char json[500];
      payload.toCharArray(json, 500);
      StaticJsonDocument<200> doc;
      deserializeJson(doc, json);
      const char* light1 = doc["light1"];
      String output1 = String(light1);
      Serial.println("light1: " + output1);
      if (output1.equals("N")) {
        digitalWrite(onboardLED, HIGH);
      } else {
        digitalWrite(onboardLED, LOW);
      }
    } else {
      Serial.println("API calling error");
    }
  } else {
    Serial.println("Connection lost");
  }
  delay(10000);
}
