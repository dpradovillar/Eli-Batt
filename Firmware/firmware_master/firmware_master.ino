int led = 13;

void setup() {                
  pinMode(led, OUTPUT);     
}

void writeFor(int led, int state) {
  digitalWrite(led, state);
  delay(1000);
}

void loop() {
  writeFor(led, HIGH);
  writeFor(led, LOW);
  delay(1000);
}
