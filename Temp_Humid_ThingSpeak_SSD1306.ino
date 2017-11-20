#include "SSD1306.h" 
#include <DHT.h>
#include <ESP8266WiFi.h>

#define DHTPIN  16   // D0
#define DHTTYPE DHT22

const char* ssid = "IoTStLabs";
const char* password = "fold1108"; 

const char* server = "api.thingspeak.com";
String apiKey = "WL5S2DCJMV021WWG";
 
WiFiClient client;
SSD1306  display(0x3c, D3, D5);
DHT dht(DHTPIN, DHTTYPE, 15); 
 
void setup() {
  Serial.begin(9600);
  Serial.println();
  Serial.println("DHT22/SSD1306");
  Serial.println("ThingSpeak");
  
  dht.begin();

  display.init();
  display.flipScreenVertically();
  display.setTextAlignment(TEXT_ALIGN_LEFT);  
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 10, "DHT22/SSD1306");
  display.drawString(0, 26, "ThingSpeak");  
  display.display();  
 
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected!!"); 
}
 
void loop() {
  float t, h;
  String  temp, humi;
  t = dht.readTemperature();
  h = dht.readHumidity(); 
  temp = t;  
  humi = h;  

  if (isnan(h) || isnan(t)) {
    Serial.println("Fail to read!!");
    display.clear();    
    display.drawString(0, 2, "Fail to read!!");
    display.display();        
    return;
  }
 
  if (client.connect(server,80)) {  //   "184.106.153.149" or api.thingspeak.com
    String postStr = apiKey;
           postStr +="&field1=";
           postStr += String(t);
           postStr +="&field2=";
           postStr += String(h);
           postStr += "\r\n\r\n";
 
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
 
    Serial.print("Temp : ");
    Serial.print(t);
    Serial.print(" ^C,\t Humidity: "); 
    Serial.print(h);
    Serial.println("%");

    display.clear();
    display.setFont(ArialMT_Plain_10);
    display.drawString(100, 0, "o");
    display.setFont(ArialMT_Plain_16);
    display.drawString(0, 2, "Temp  :             C");
    display.drawString(56, 2, temp);  
    display.drawString(0, 18, "Humid :           %");
    display.drawString(56, 18, humi);  
    display.display();      
  }
  client.stop();
 
  Serial.println("Waiting...");
  // ThingSpeak 무료 계정은 업데이트 주기가 15초임 
  delay(12000);
}
