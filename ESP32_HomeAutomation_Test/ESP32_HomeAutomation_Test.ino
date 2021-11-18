#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <HTTPClient.h>
#include <LiquidCrystal.h>
#include <WiFi.h>

#define CONNECTED 1
#define RECEIVING 2
#define NOT_AVAILABLE 3

#define onboardLED 2

#define START 0
#define FORWARD 1
#define EMPTY_RIGHT 0
#define EMPTY_LEFT 1
#define EMPTY_MIDDLE 2
#define FILL_RIGHT 3
#define FILL_LEFT 4
#define FILL_MIDDLE 5

const char* ssid     = "SLTH";
const char* password = "nicwifi1122";

int barPosition;
int delayTime = 1000;

byte boxLeft[8] = {
  B11111,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B11111,
};
byte boxMiddle[8] = {
  B11111,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
};
byte boxRight[8] = {
  B11111,
  B00001,
  B00001,
  B00001,
  B00001,
  B00001,
  B11111,
};
byte boxFillMiddle[8] = {
  B11111,
  B00000,
  B11111,
  B11111,
  B11111,
  B00000,
  B11111,
};
byte boxFillLeft[8] = {
  B11111,
  B10000,
  B11111,
  B11111,
  B11111,
  B10000,
  B11111,
}; byte boxFillRight[8] = {
  B11111,
  B00001,
  B11111,
  B11111,
  B11111,
  B00001,
  B11111,
};

LiquidCrystal * lcd;
WiFiServer server(80);

void setup() {
  lcd = new LiquidCrystal(13, 12, 14, 27, 26, 25);
  lcd -> createChar(0, boxRight);
  lcd -> createChar(1, boxLeft);
  lcd -> createChar(2, boxMiddle);
  lcd -> createChar(3, boxFillRight);
  lcd -> createChar(4, boxFillLeft);
  lcd -> createChar(5, boxFillMiddle);
  lcd -> begin(16, 2);
  lcd -> clear();
  pinMode(onboardLED, OUTPUT);
  delay(10);
  connectWifi();
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    displayInfo("SCANING...");
    progressBar(0, delayTime);
    HTTPClient http;
    http.begin("https://hsm272e.herokuapp.com/monitor/hsts");
    http.addHeader("Authorization", "Basic eWFzYXNybTphMXMyZDNmNA==");
    http.addHeader("Content-Type", "application/json");
    progressBar(20, delayTime);
    int responseCode = http.GET();
    if (responseCode > 0) {
      progressBar(40, delayTime);
      String payload = http.getString();
      payload.replace(" ", "");
      payload.replace("\n", "");
      payload.trim();
      progressBar(60, delayTime);
      char json[500];
      payload.toCharArray(json, 500);
      StaticJsonDocument<200> doc;
      deserializeJson(doc, json);
      const char* light1 = doc["light1"];
      progressBar(80, delayTime);
      String output1 = String(light1);
      if (output1.equals("N")) {
        digitalWrite(onboardLED, HIGH);
      } else {
        digitalWrite(onboardLED, LOW);
      }
      progressBar(100, delayTime);
      displayInfo("UPDATED!");
    } else {
      displayInfo("API ERROR");
    }
  } else {
    displayInfo("Connection lost");
    delay(5000);
    displayInfo("RETRY...");
    connectWifi();
  }
  delay(10000);
}

void connectWifi() {
  displayInfo("CONNECTING...");
  processingBar(START);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    processingBar(FORWARD);
  }
  server.begin();
  displayInfo("WiFi CONNECTED!");
}

void displayInfo(String info) {
  lcd -> clear();
  lcd -> setCursor(0, 0);
  lcd -> print(info);
  delay(delayTime);
}

void progressBar(int percentage, int processTime) {
  switch (percentage) {
    case 0:
      lcd -> setCursor(0, 1);
      lcd -> write(byte(EMPTY_LEFT));
      lcd -> write(byte(EMPTY_MIDDLE));
      lcd -> write(byte(EMPTY_MIDDLE));
      lcd -> write(byte(EMPTY_MIDDLE));
      lcd -> write(byte(EMPTY_MIDDLE));
      lcd -> write(byte(EMPTY_MIDDLE));
      lcd -> write(byte(EMPTY_MIDDLE));
      lcd -> write(byte(EMPTY_MIDDLE));
      lcd -> write(byte(EMPTY_MIDDLE));
      lcd -> write(byte(EMPTY_RIGHT));
      delay(processTime);
      break;
    case 20:
      lcd -> setCursor(0, 1);
      lcd -> write(byte(FILL_LEFT));
      lcd -> write(byte(FILL_MIDDLE));
      lcd -> write(byte(EMPTY_MIDDLE));
      lcd -> write(byte(EMPTY_MIDDLE));
      lcd -> write(byte(EMPTY_MIDDLE));
      lcd -> write(byte(EMPTY_MIDDLE));
      lcd -> write(byte(EMPTY_MIDDLE));
      lcd -> write(byte(EMPTY_MIDDLE));
      lcd -> write(byte(EMPTY_MIDDLE));
      lcd -> write(byte(EMPTY_RIGHT));
      delay(processTime);
      break;
    case 40:
      lcd -> setCursor(0, 1);
      lcd -> write(byte(FILL_LEFT));
      lcd -> write(byte(FILL_MIDDLE));
      lcd -> write(byte(FILL_MIDDLE));
      lcd -> write(byte(FILL_MIDDLE));
      lcd -> write(byte(EMPTY_MIDDLE));
      lcd -> write(byte(EMPTY_MIDDLE));
      lcd -> write(byte(EMPTY_MIDDLE));
      lcd -> write(byte(EMPTY_MIDDLE));
      lcd -> write(byte(EMPTY_MIDDLE));
      lcd -> write(byte(EMPTY_RIGHT));
      delay(processTime);
      break;
    case 60:
      lcd -> setCursor(0, 1);
      lcd -> write(byte(FILL_LEFT));
      lcd -> write(byte(FILL_MIDDLE));
      lcd -> write(byte(FILL_MIDDLE));
      lcd -> write(byte(FILL_MIDDLE));
      lcd -> write(byte(FILL_MIDDLE));
      lcd -> write(byte(FILL_MIDDLE));
      lcd -> write(byte(EMPTY_MIDDLE));
      lcd -> write(byte(EMPTY_MIDDLE));
      lcd -> write(byte(EMPTY_MIDDLE));
      lcd -> write(byte(EMPTY_RIGHT));
      delay(processTime);
      break;
    case 80:
      lcd -> setCursor(0, 1);
      lcd -> write(byte(FILL_LEFT));
      lcd -> write(byte(FILL_MIDDLE));
      lcd -> write(byte(FILL_MIDDLE));
      lcd -> write(byte(FILL_MIDDLE));
      lcd -> write(byte(FILL_MIDDLE));
      lcd -> write(byte(FILL_MIDDLE));
      lcd -> write(byte(FILL_MIDDLE));
      lcd -> write(byte(FILL_MIDDLE));
      lcd -> write(byte(EMPTY_MIDDLE));
      lcd -> write(byte(EMPTY_RIGHT));
      delay(processTime);
      break;
    case 100:
      lcd -> setCursor(0, 1);
      lcd -> write(byte(FILL_LEFT));
      lcd -> write(byte(FILL_MIDDLE));
      lcd -> write(byte(FILL_MIDDLE));
      lcd -> write(byte(FILL_MIDDLE));
      lcd -> write(byte(FILL_MIDDLE));
      lcd -> write(byte(FILL_MIDDLE));
      lcd -> write(byte(FILL_MIDDLE));
      lcd -> write(byte(FILL_MIDDLE));
      lcd -> write(byte(FILL_MIDDLE));
      lcd -> write(byte(FILL_RIGHT));
      delay(processTime);
      break;
  }
}

void processingBar(int command) {
  if (command == 0) {
    lcd -> setCursor(0, 1);
    lcd -> write(byte(EMPTY_LEFT));
    lcd -> write(byte(EMPTY_MIDDLE));
    lcd -> write(byte(EMPTY_MIDDLE));
    lcd -> write(byte(EMPTY_MIDDLE));
    lcd -> write(byte(EMPTY_MIDDLE));
    lcd -> write(byte(EMPTY_MIDDLE));
    lcd -> write(byte(EMPTY_MIDDLE));
    lcd -> write(byte(EMPTY_MIDDLE));
    lcd -> write(byte(EMPTY_MIDDLE));
    lcd -> write(byte(EMPTY_MIDDLE));
    lcd -> write(byte(EMPTY_MIDDLE));
    lcd -> write(byte(EMPTY_MIDDLE));
    lcd -> write(byte(EMPTY_MIDDLE));
    lcd -> write(byte(EMPTY_MIDDLE));
    lcd -> write(byte(EMPTY_MIDDLE));
    lcd -> write(byte(EMPTY_RIGHT));
    barPosition = 1;
  } else if (command == 1) {
    switch (barPosition) {
      case 1:
        lcd -> setCursor(0, 1);
        lcd -> write(byte(FILL_LEFT));
        lcd -> write(byte(FILL_MIDDLE));
        lcd -> write(byte(FILL_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_RIGHT));
        barPosition++;
        break;
      case 2:
        lcd -> setCursor(0, 1);
        lcd -> write(byte(EMPTY_LEFT));
        lcd -> write(byte(FILL_MIDDLE));
        lcd -> write(byte(FILL_MIDDLE));
        lcd -> write(byte(FILL_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_RIGHT));
        barPosition++;
        break;
      case 3:
        lcd -> setCursor(0, 1);
        lcd -> write(byte(EMPTY_LEFT));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(FILL_MIDDLE));
        lcd -> write(byte(FILL_MIDDLE));
        lcd -> write(byte(FILL_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_RIGHT));
        barPosition++;
        break;
      case 4:
        lcd -> setCursor(0, 1);
        lcd -> write(byte(EMPTY_LEFT));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(FILL_MIDDLE));
        lcd -> write(byte(FILL_MIDDLE));
        lcd -> write(byte(FILL_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_RIGHT));
        barPosition++;
        break;
      case 5:
        lcd -> setCursor(0, 1);
        lcd -> write(byte(EMPTY_LEFT));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(FILL_MIDDLE));
        lcd -> write(byte(FILL_MIDDLE));
        lcd -> write(byte(FILL_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_RIGHT));
        barPosition++;
        break;
      case 6:
        lcd -> setCursor(0, 1);
        lcd -> write(byte(EMPTY_LEFT));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(FILL_MIDDLE));
        lcd -> write(byte(FILL_MIDDLE));
        lcd -> write(byte(FILL_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_RIGHT));
        barPosition++;
        break;
      case 7:
        lcd -> setCursor(0, 1);
        lcd -> write(byte(EMPTY_LEFT));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(FILL_MIDDLE));
        lcd -> write(byte(FILL_MIDDLE));
        lcd -> write(byte(FILL_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_RIGHT));
        barPosition++;
        break;
      case 8:
        lcd -> setCursor(0, 1);
        lcd -> write(byte(EMPTY_LEFT));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(FILL_MIDDLE));
        lcd -> write(byte(FILL_MIDDLE));
        lcd -> write(byte(FILL_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_RIGHT));
        barPosition++;
        break;
      case 9:
        lcd -> setCursor(0, 1);
        lcd -> write(byte(EMPTY_LEFT));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(FILL_MIDDLE));
        lcd -> write(byte(FILL_MIDDLE));
        lcd -> write(byte(FILL_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_RIGHT));
        barPosition++;
        break;
      case 10:
        lcd -> setCursor(0, 1);
        lcd -> write(byte(EMPTY_LEFT));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(FILL_MIDDLE));
        lcd -> write(byte(FILL_MIDDLE));
        lcd -> write(byte(FILL_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_RIGHT));
        barPosition++;
        break;
      case 11:
        lcd -> setCursor(0, 1);
        lcd -> write(byte(EMPTY_LEFT));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(FILL_MIDDLE));
        lcd -> write(byte(FILL_MIDDLE));
        lcd -> write(byte(FILL_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_RIGHT));
        barPosition++;
        break;
      case 12:
        lcd -> setCursor(0, 1);
        lcd -> write(byte(EMPTY_LEFT));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(FILL_MIDDLE));
        lcd -> write(byte(FILL_MIDDLE));
        lcd -> write(byte(FILL_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_RIGHT));
        barPosition++;
        break;
      case 13:
        lcd -> setCursor(0, 1);
        lcd -> write(byte(EMPTY_LEFT));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(FILL_MIDDLE));
        lcd -> write(byte(FILL_MIDDLE));
        lcd -> write(byte(FILL_MIDDLE));
        lcd -> write(byte(EMPTY_RIGHT));
        barPosition++;
        break;
      case 14:
        lcd -> setCursor(0, 1);
        lcd -> write(byte(EMPTY_LEFT));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(FILL_MIDDLE));
        lcd -> write(byte(FILL_MIDDLE));
        lcd -> write(byte(FILL_RIGHT));
        barPosition++;
        break;
      case 15:
        lcd -> setCursor(0, 1);
        lcd -> write(byte(FILL_LEFT));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(FILL_MIDDLE));
        lcd -> write(byte(FILL_RIGHT));
        barPosition++;
        break;
      case 16:
        lcd -> setCursor(0, 1);
        lcd -> write(byte(FILL_LEFT));
        lcd -> write(byte(FILL_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(EMPTY_MIDDLE));
        lcd -> write(byte(FILL_RIGHT));
        barPosition = 1;
        break;
    }
  }
}
