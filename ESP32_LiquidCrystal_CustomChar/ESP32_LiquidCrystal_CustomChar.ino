#include <LiquidCrystal.h>

#define START 0
#define FORWARD 1

#define EMPTY_RIGHT 0
#define EMPTY_LEFT 1
#define EMPTY_MIDDLE 2
#define FILL_RIGHT 3
#define FILL_LEFT 4
#define FILL_MIDDLE 5

LiquidCrystal * lcd;

int barPosition;

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

void setup() {
  lcd = new LiquidCrystal(13, 12, 14, 27, 26, 25);
  lcd -> createChar(0, boxRight);
  lcd -> createChar(1, boxLeft);
  lcd -> createChar(2, boxMiddle);
  lcd -> createChar(3, boxFillRight);
  lcd -> createChar(4, boxFillLeft);
  lcd -> createChar(5, boxFillMiddle);
  lcd -> begin(16, 2);
  barPosition = 1;
}

void loop() {
  processingBar(START);
  while (true) {
    processingBar(FORWARD);
    delay(200);
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
