/*
  WiFi Web Server LED Blink

  A simple web server that lets you blink an LED via the web.
  This sketch will print the IP address of your WiFi Shield (once connected)
  to the Serial monitor. From there, you can open that address in a web browser
  to turn on and off the LED on pin 5.

  If the IP address of your shield is yourAddress:
  http://yourAddress/H turns the LED on
  http://yourAddress/L turns it off

  This example is written for a network using WPA encryption. For
  WEP or WPA, change the Wifi.begin() call accordingly.

  Circuit:
   WiFi shield attached
   LED attached to pin 5

  created for arduino 25 Nov 2012
  by Tom Igoe

  ported for sparkfun esp32
  31.01.2017 by Jan Hendrik Berlin

*/

#include <WiFi.h>

const char* ssid     = "SLTH";
const char* password = "nicwifi1122";

WiFiServer server(80);

#define CONNECTED 1
#define RECEIVING 2
#define NOT_AVAILABLE 3
#define outputLED 2
#define greenLED 4
#define yellowLED 5
#define redLED 18

void setup()
{
  pinMode(outputLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  delay(10);
  connectWifi();
}

int value = 0;

void loop() {
  WiFiClient client = server.available();
  if (client) {
    indicate(RECEIVING);
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            client.print("Click <a href=\"/N\">here</a> to turn the LED on pin 2 on.<br>");
            client.print("Click <a href=\"/O\">here</a> to turn the LED on pin 2 off.<br>");
            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }

        if (currentLine.endsWith("GET /N")) {
          digitalWrite(outputLED, HIGH);
        }
        if (currentLine.endsWith("GET /O")) {
          digitalWrite(outputLED, LOW);
        }
      }
    }
    // close the connection:
    client.stop();
    indicate(CONNECTED);
  }

  if (WiFi.status() != WL_CONNECTED)
    connectWifi();

}

void connectWifi() {
  indicate(NOT_AVAILABLE);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  server.begin();
  indicate(CONNECTED);
}

void indicate(int type) {
  digitalWrite(greenLED, HIGH);
  digitalWrite(yellowLED, HIGH);
  digitalWrite(redLED, HIGH);
  digitalWrite(type == 1 ? greenLED : type == 2 ? yellowLED : redLED, LOW);
}
