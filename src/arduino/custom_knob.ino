int lastVal = -1;

void setup() {
  Serial.begin(9600);
}

void loop() {
  
  int potIn = analogRead(A0);
  int midi = map(potIn, 0, 1023, 0, 127);

  if(midi != lastVal){
    lastVal = midi;
    Serial.println(midi);
  }

}
