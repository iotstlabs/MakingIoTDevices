#define TrigPin  5   // D1
#define EchoPin  4   // D2
 
void setup() { 
  Serial.begin (9600);
  pinMode(TrigPin, OUTPUT);
  pinMode(EchoPin, INPUT);
}
 
void loop() {
  long duration;
  float distance;
  
  digitalWrite(TrigPin, LOW);  
  delayMicroseconds(2);   
  digitalWrite(TrigPin, HIGH);
  delayMicroseconds(10);   
  digitalWrite(TrigPin, LOW);  
  duration = pulseIn(EchoPin, HIGH);
  distance = duration * 0.034 / 2;
  
  Serial.print(distance);
  Serial.println(" cm");
  delay(500);
}
