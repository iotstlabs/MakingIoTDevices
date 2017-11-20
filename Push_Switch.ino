int ledPin = 5; // D1 (gpio5)
int button = 4; // D2 (gpio4)
int buttonState=0;

void setup() {
 pinMode(ledPin, OUTPUT);
 pinMode(button, INPUT);
}
void loop() {
 buttonState=digitalRead(button); // put your main code here, to run repeatedly:
 if (buttonState == 1)
 {
 digitalWrite(ledPin, HIGH); 
 //delay(200);
 }
 if (buttonState==0)
 {
 digitalWrite(ledPin, LOW); 
 //delay(200);
 }
}

