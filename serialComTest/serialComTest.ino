int ledred = 13;
int ledgreen = 9;
int ledblue = 10;

void setup() {
  Serial.begin(9600);
  pinMode(ledred, OUTPUT);
  pinMode(ledgreen, OUTPUT);
  pinMode(ledblue, OUTPUT);
}

void loop() {
  if (Serial.available() > 0) {
    char val = Serial.read();
    if (val == '1') {
      digitalWrite(ledred, HIGH);
      digitalWrite(ledgreen, LOW);
      digitalWrite(ledblue, LOW);
      Serial.println("RED LED ON");
    } else if (val == '2') {
      digitalWrite(ledred, LOW);
      digitalWrite(ledgreen, HIGH);
      digitalWrite(ledblue, LOW);
      Serial.println("GREEN LED ON");
    } else if (val == '3') {
      digitalWrite(ledred, LOW);
      digitalWrite(ledgreen, LOW);
      digitalWrite(ledblue, HIGH);
      Serial.println("BLUE LED ON");
    } else if (val == '4') {
      digitalWrite(ledred, LOW);
      digitalWrite(ledgreen, LOW);
      digitalWrite(ledblue, LOW);
      Serial.println("ALL LED OFF");
    }
  }
}
