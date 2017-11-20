#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306.h" // alias for `#include "SSD1306Wire.h"`
#include <DHT.h>

#include "images.h"

// Initialize the OLED display using brzo_i2c
// D3 -> SDA
// D5 -> SCL
// SSD1306Brzo display(0x3c, D3, D5);

// Initialize the OLED display using Wire library
SSD1306  display(0x3c, D3, D5);

#define DHTPIN  16   //D0
#define DHTTYPE DHT22
#define DEMO_DURATION 3000
typedef void (*Demo)(void);

int demoMode = 0;
int counter = 1;

DHT dht(DHTPIN, DHTTYPE, 15);

void setup() {
  Serial.begin(9600);
  Serial.println();
  Serial.println();

  dht.begin();
  
  // Initialising the UI will init the display too.
  display.init();

  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);

}

void drawFontFaceDemo() {
    String  temp;
    temp = dht.readTemperature();
    String  humi;
    humi = dht.readHumidity(); 

    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_10);
    display.drawString(100, 0, "o");
    display.setFont(ArialMT_Plain_16);
    display.drawString(0, 2, "Temp  :             C");
        display.drawString(56, 2, temp);
    display.drawString(0, 18, "Humid :           %");
    display.drawString(56, 18, humi);
}

Demo demos[] = {drawFontFaceDemo, drawFontFaceDemo};
int demoLength = (sizeof(demos) / sizeof(Demo));
long timeSinceLastModeSwitch = 0;

void loop() {
  // clear the display
  display.clear();
  // draw the current demo method
  demos[demoMode]();

  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  display.drawString(10, 128, String(millis()));
  // write the buffer to the display
  display.display();

  if (millis() - timeSinceLastModeSwitch > DEMO_DURATION) {
    demoMode = (demoMode + 1)  % demoLength;
    timeSinceLastModeSwitch = millis();
  }
  counter++;
  delay(3000);
}
