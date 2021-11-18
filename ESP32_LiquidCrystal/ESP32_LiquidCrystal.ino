#include <LiquidCrystal.h>

LiquidCrystal * lcd;

void setup() {
  lcd = new LiquidCrystal(13, 12, 14, 27, 26, 25);
  lcd -> begin(16, 2);
  lcd -> clear();
}

void loop() {
  lcd -> setCursor(0, 0);
  lcd -> print("HomeAutomation");
  lcd -> setCursor(0, 1);
  lcd -> print("]]]]]]");
}
