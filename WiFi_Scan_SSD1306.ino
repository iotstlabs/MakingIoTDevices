#include "SSD1306.h" 
#include "ESP8266WiFi.h"

SSD1306  display(0x3c, D3, D5);

void setup() {
  Serial.begin(9600);
  Serial.println();
  Serial.println("SSD1306/WiFiScan");
    
  display.init();
  display.flipScreenVertically();
  display.setTextAlignment(TEXT_ALIGN_LEFT);  
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 10, "SSD1306 &");
  display.drawString(0, 28, "WiFiScan");
  display.display();  
  display.setFont(ArialMT_Plain_10);  
  delay(2000);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
}

void loop() {
  display.clear();
  display.drawString(0, 1, "scanning WiFi signals!");
  int n = WiFi.scanNetworks();
  delay(20);
  display.drawString(0, 12, "scanning...");
  display.display();
  delay(400);  

  String postStr, noNw;
  display.clear();  
  if(n==0) {
    Serial.println("No WiFi networks found"); 
    display.drawString(0, 2, "No WiFi networks found");
  } else {
    noNw = n;    // 와이파이 네트워크의 개수
    noNw += " networks are found!";
    Serial.println(noNw);
    display.drawString(0, 2, noNw);
    for(int i=0; i<n; i++) {
      postStr = i+1;
      postStr += " : ";
      postStr += WiFi.SSID(i);
      postStr += " (";
      postStr += WiFi.RSSI(i);      
      postStr += ")";
      postStr += (WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*";
      Serial.println(postStr);
      display.drawString(0, i*10+12, postStr);
      delay(10);
    }
  }
  display.display();  
  Serial.println();
  delay(3000);
}
