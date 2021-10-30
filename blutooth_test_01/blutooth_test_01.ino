/*
 * Bluetooth code: 1234567 or 123456789
 * LED PINS: 13, 10, 9
 * Arduino Tx pin needs to go through a voltage divider
 * (2.2k: 2.2K at least)
 * to reduce Tx voltage around to 3.3V before
 * connecting to Bluetooth module Rx pin
 * Created by Yasas Maddumage on 20/10/2021
*/
int ledred = 13;
int ledgreen = 9;
int ledblue = 10;
int tx = 1;
int rx = 0;
char inSerial[15];


void setup() {
  Serial.begin(9600);
  pinMode(ledred, OUTPUT);
  pinMode(ledgreen, OUTPUT);
  pinMode(ledblue, OUTPUT);
  pinMode(tx, OUTPUT);
  pinMode(rx, INPUT);
  allLedOff();
}

void loop() {
  int i = 0;
  delay(500);
  if (Serial.available() > 0) {
    while (Serial.available() > 0) {
      inSerial[i] = Serial.read();
      i++;
    }
    inSerial[i] = '\0';
    Check_Protocol(inSerial);
  }
}

void allLedOff()
{
  digitalWrite(ledred, HIGH);
  digitalWrite(ledgreen, HIGH);
  digitalWrite(ledblue, HIGH);
}

void Check_Protocol(char inStr[]) {
  int m = 0;
  Serial.println(inStr);

  if (inStr[0] == '1') {    //Ledred ON
    allLedOff();
    digitalWrite(ledred, LOW);
    Serial.println("RED LED ON");
    Serial.println("X");
  } else if (inStr[0] == '2') {    //Ledred ON
    allLedOff();
    digitalWrite(ledgreen, LOW);
    Serial.println("GREEN LED ON");
    Serial.println("X");
  } else if (inStr[0] == '3') {    //Ledred ON
    allLedOff();
    digitalWrite(ledblue, LOW);
    Serial.println("BLUE LED ON");
    Serial.println("X");
  } else {
    allLedOff();
    Serial.println("ALL LED OFF");
    Serial.println("X");
    }
}
