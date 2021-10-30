int snspin = 11;
int rlyout = 13;
int ssignal;

void setup() {
  Serial.begin(9600);
  pinMode(snspin, INPUT);
  pinMode(rlyout, OUTPUT);
}

void loop() {
      ssignal = digitalRead(snspin);
      String prints = "0";
      if(ssignal == 1){
        
        }
      prints = "detected";
      Serial.println(prints);
      digitalWrite(rlyout,ssignal);
}
