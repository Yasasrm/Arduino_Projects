#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <LiquidCrystal.h>
#include <WiFi.h>

#define CONNECTED 1
#define RECEIVING 2
#define NOT_AVAILABLE 3

#define RELAY_1 2
#define RELAY_2 4
#define RELAY_3 16
#define LDR_1 17
#define LDR_2 5
#define LDR_3 18
#define LDR_4 19
#define BUTTON_LCD 15
#define LCD_LED 23
#define BUTTON_1 33
#define BUTTON_2 32
#define BUTTON_3 35

#define START 0
#define FORWARD 1
#define EMPTY_RIGHT 0
#define EMPTY_LEFT 1
#define EMPTY_MIDDLE 2
#define FILL_RIGHT 3
#define FILL_LEFT 4
#define FILL_MIDDLE 5

static unsigned long last_millis = 0;

const char* ssid     = "SLTH";
const char* password = "nicwifi1122";

const char* api_status_check = "/mcu/status_report/101";
const char* bedroom_bulb_on = "/mcu/bedroom_bulb/101/1";
const char* bedroom_bulb_off = "/mcu/bedroom_bulb/101/0";
const char* outdoor_bulb_on = "/mcu/outdoor_bulb/101/1";
const char* outdoor_bulb_off = "/mcu/outdoor_bulb/101/0";
const char* living_room_bulb_on = "/mcu/living_room_bulb/101/1";
const char* living_room_bulb_off = "/mcu/living_room_bulb/101/0";
const char* ambient_light_dark = "/mcu/ambient_light/101/0";
const char* ambient_light_light = "/mcu/ambient_light/101/1";
const char* transmission_end = "/mcu/transmission/101";

int barPosition;
int lcdLed = 0;
int relaySw1 = 0;
int relaySw2 = 0;
int relaySw3 = 0;
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

void IRAM_ATTR lcdLedISR() {
  unsigned long m = millis();
  if (m - last_millis > 200) {
    if (lcdLed == 0) {
      lcdLed = 1;
    } else {
      lcdLed = 0;
    }
    digitalWrite(LCD_LED, lcdLed);
  }
  last_millis = m;
}

void IRAM_ATTR ldrLedISR_1() {
  unsigned long m = millis();
  if (m - last_millis > 200) {
    if (relaySw1 == 0) {
      relaySw1 = 1;
    } else {
      relaySw1 = 0;
    }
    digitalWrite(RELAY_1, relaySw1);
  }
  last_millis = m;
}

void IRAM_ATTR ldrLedISR_2() {
  unsigned long m = millis();
  if (m - last_millis > 200) {
    if (relaySw2 == 0) {
      relaySw2 = 1;
    } else {
      relaySw2 = 0;
    }
    digitalWrite(RELAY_2, relaySw2);
  }
  last_millis = m;
}

void IRAM_ATTR ldrLedISR_3() {
  unsigned long m = millis();
  if (m - last_millis > 200) {
    if (relaySw3 == 0) {
      relaySw3 = 1;
    } else {
      relaySw3 = 0;
    }
    digitalWrite(RELAY_3, relaySw3);
  }
  last_millis = m;
}

void setup() {
  Serial.begin(115200);
  lcd = new LiquidCrystal(13, 12, 14, 27, 26, 25);
  lcd -> createChar(0, boxRight);
  lcd -> createChar(1, boxLeft);
  lcd -> createChar(2, boxMiddle);
  lcd -> createChar(3, boxFillRight);
  lcd -> createChar(4, boxFillLeft);
  lcd -> createChar(5, boxFillMiddle);
  lcd -> begin(16, 2);
  lcd -> clear();
  pinMode(RELAY_1, OUTPUT);
  pinMode(RELAY_2, OUTPUT);
  pinMode(RELAY_3, OUTPUT);
  pinMode(LCD_LED, OUTPUT);
  pinMode(LDR_1, INPUT);
  pinMode(LDR_2, INPUT);
  pinMode(LDR_3, INPUT);
  pinMode(LDR_4, INPUT);
  pinMode(BUTTON_LCD, INPUT);
  pinMode(BUTTON_1, INPUT);
  pinMode(BUTTON_2, INPUT);
  pinMode(BUTTON_3, INPUT);
  attachInterrupt(BUTTON_LCD, lcdLedISR, RISING);
  attachInterrupt(BUTTON_1, ldrLedISR_1, RISING);
  attachInterrupt(BUTTON_2, ldrLedISR_2, RISING);
  attachInterrupt(BUTTON_3, ldrLedISR_3, RISING);
  delay(10);
  connectWifi();
}

void loop() {
  Serial.println("START A LOOP");
  if (WiFi.status() == WL_CONNECTED) {
    displayInfoInst("SCANING...");
    progressBar(0, delayTime);
    Serial.println("CALL API");
    String payload = callAPI(api_status_check);
    Serial.println("RESPONSE");
    Serial.println(payload);
    if (!payload.equals("ERROR")) {
      displayInfoInst("PROCESSING...");
      progressBar(20, delayTime);
      char json[500];
      payload.toCharArray(json, 500);
      StaticJsonDocument<200> doc;
      deserializeJson(doc, json);

      const char* bedroomBulb = doc["bedroomBulb"];
      const char* outdoorBulb = doc["outdoorBulb"];
      const char* livingRoomBulb = doc["livingRoomBulb"];
      const char* action = doc["action"];
      const char* code = doc["code"];
      const char* response_status = doc["status"];

      String rly1 = String(bedroomBulb);
      String rly2 = String(outdoorBulb);
      String rly3 = String(livingRoomBulb);
      String actn = String(action);
      String cd = String(code);
      String rss = String(response_status);

      if (cd.equals("200")) {
        if (actn.equals("1")) {
          displayInfoInst("UPDATING OUTPUTS");
          progressBar(40, delayTime);
          if (rly1.equals("1")) {
            digitalWrite(RELAY_1, HIGH);
            relaySw1 = 1;
          } else if (rly1.equals("0")) {
            digitalWrite(RELAY_1, LOW);
            relaySw1 = 0;
          }

          if (rly2.equals("1")) {
            digitalWrite(RELAY_2, HIGH);
            relaySw2 = 1;
          } else if (rly1.equals("0")) {
            digitalWrite(RELAY_2, LOW);
            relaySw2 = 0;
          }

          if (rly3.equals("1")) {
            digitalWrite(RELAY_3, HIGH);
            relaySw3 = 1;
          } else if (rly1.equals("0")) {
            digitalWrite(RELAY_3, LOW);
            relaySw3 = 0;
          }
          displayInfoInst("CHECKING...");
          progressBar(50, delayTime);
          int ldr1 = digitalRead(LDR_1);
          int ldr2 = digitalRead(LDR_2);
          int ldr3 = digitalRead(LDR_3);
          int ldr4 = digitalRead(LDR_4);

          displayInfoInst("SENDING FEEDBACK");
          progressBar(60, delayTime);
          if (ldr1 == 1) {
            callAPI(bedroom_bulb_on);
          } else {
            callAPI(bedroom_bulb_off);
          }

          if (ldr2 == 1) {
            callAPI(outdoor_bulb_on);
          } else {
            callAPI(outdoor_bulb_off);
          }
          progressBar(80, delayTime);
          if (ldr3 == 1) {
            callAPI(living_room_bulb_on);
          } else {
            callAPI(living_room_bulb_off);
          }

          if (ldr4 == 1) {
            callAPI(ambient_light_light);
          } else {
            callAPI(ambient_light_dark);
          }

          callAPI(transmission_end);
          progressBar(100, delayTime);
        } else {
          int ldr1 = digitalRead(LDR_1);
          int ldr2 = digitalRead(LDR_2);
          int ldr3 = digitalRead(LDR_3);
          int ldr4 = digitalRead(LDR_4);

          displayInfoInst("SENDING FEEDBACK");
          progressBar(60, delayTime);
          if (ldr1 == 1) {
            callAPI(bedroom_bulb_on);
          } else {
            callAPI(bedroom_bulb_off);
          }

          if (ldr2 == 1) {
            callAPI(outdoor_bulb_on);
          } else {
            callAPI(outdoor_bulb_off);
          }
          progressBar(80, delayTime);
          if (ldr3 == 1) {
            callAPI(living_room_bulb_on);
          } else {
            callAPI(living_room_bulb_off);
          }

          if (ldr4 == 1) {
            callAPI(ambient_light_light);
          } else {
            callAPI(ambient_light_dark);
          }

          displayInfoInst("UP-TO-DATE");
          progressBar(100, delayTime);
        }
      } else {
        displayInfo(rss);
      }
    } else {
      displayInfo("API-ERROR");
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
  digitalWrite(LCD_LED, HIGH);
  Serial.println("CONNECTING TO WIFI");
  displayInfo("CONNECTING...");
  processingBar(START);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    processingBar(FORWARD);
  }
  server.begin();
  displayInfo("WiFi CONNECTED!");
  digitalWrite(LCD_LED, LOW);
}

String callAPI(String call) {
  String payload = "";
  HTTPClient http;
  http.begin("https://hsm272e.herokuapp.com" + call);
  http.addHeader("Authorization", "Basic eWFzYXNybTphMXMyZDNmNA==");
  http.addHeader("Content-Type", "application/json");
  int responseCode = http.GET();
  Serial.println(String(responseCode));
  if (responseCode > 0) {
    payload = http.getString();
    payload.replace(" ", "");
    payload.replace("\n", "");
    payload.trim();
  } else {
    displayInfo("API ERROR");
  }
  return payload;
}

void displayInfo(String info) {
  lcd -> clear();
  lcd -> setCursor(0, 0);
  lcd -> print(info);
  delay(delayTime);
}

void displayInfoInst(String info) {
  lcd -> clear();
  lcd -> setCursor(0, 0);
  lcd -> print(info);
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
    case 50:
      lcd -> setCursor(0, 1);
      lcd -> write(byte(FILL_LEFT));
      lcd -> write(byte(FILL_MIDDLE));
      lcd -> write(byte(FILL_MIDDLE));
      lcd -> write(byte(FILL_MIDDLE));
      lcd -> write(byte(FILL_MIDDLE));
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
