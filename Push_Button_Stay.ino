#define ledPin     5  // D1 (GPIO5)
#define BUTTON     4  // D2 (GPIO4)

int buttonState = 0;
int oldButtonState = 0;
int ledState = 0;

void setup() { 
  pinMode(ledPin, OUTPUT);
  pinMode(BUTTON, INPUT);
}

void loop() {
  buttonState = digitalRead(BUTTON); 
  if(buttonState != oldButtonState) {
    oldButtonState = buttonState;
    if (buttonState == HIGH)
      ledState = !ledState;
  }
  if(ledState)
    digitalWrite(ledPin, HIGH);
  else
    digitalWrite(ledPin, LOW); 
}


