/*
*Board Model: ESP32 Dev Module
*Press Boot button to upload code when printing 
*"Connecting........___"
*/

int ledPin = 2;
void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  Serial.println("LED ON");
  digitalWrite(ledPin, HIGH);
  delay(500);
  Serial.println("LED OFF");
  digitalWrite(ledPin, LOW);
  delay(500);
}
