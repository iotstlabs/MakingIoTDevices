#include <DHT.h>
#include <ESP8266WiFi.h>

#define DHTPIN  16   // D0
#define DHTTYPE DHT22

const char* ssid = "IoTStLabs";
const char* password = "fold1108"; 

const char* server = "api.thingspeak.com";
String apiKey = "WL5S2DCJMV021WWG";
 
WiFiClient client;
DHT dht(DHTPIN, DHTTYPE, 15); 
 
void setup() {
  Serial.begin(9600);
  Serial.println();
  Serial.println("DHT22/SSD1306");
  Serial.println("ThingSpeak");
  
  dht.begin();
 
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
  t = dht.readTemperature();
  h = dht.readHumidity();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read!!");
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
  }
  client.stop();
 
  Serial.println("Waiting...");
  // ThingSpeak 무료 계정은 업데이트 주기가 15초임 
  delay(15000);
}
