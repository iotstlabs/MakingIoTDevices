/*
 *  This sketch sends data via HTTP GET requests to data.sparkfun.com service.
 *
 *  You need to get streamId and privateKey at data.sparkfun.com and paste them
 *  below. Or just customize this script to talk to other HTTP servers.
 *
 */

#include <ESP8266WiFi.h>
#include <DHT.h>

#define DHTPIN  2       // SDA 핀의 설정 2 = D4
#define DHTTYPE DHT22   // DHT22 (AM2302) 센서종류 설정

const char* ssid     = "IoTStLabs";
const char* password = "fold1108";

const char* host = "184.106.153.149";  //thingspeak.com

DHT dht(DHTPIN, DHTTYPE, 15);

void setup() {
  Serial.begin(9600);
  delay(10);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  
  dht.begin();

  // We start by connecting to a WiFi network
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
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


int toggle = 0;

void loop() {
  int err;
  float temp = dht.readTemperature();
  float humi = dht.readHumidity(); 

  Serial.print("connecting to ");
  Serial.println(host);
  
  WiFiClient client;         // Use WiFiClient class to create TCP connections
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  // We now create a URI for the request
  String url1 = "/update?key=WXO7XB19WARG05&field1=";
  
  // This will send the request to the server
  client.print(String("GET ") + url1 + temp + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
               
  Serial.print("Requesting URL: ");
  Serial.print(url1);
  Serial.print(temp);
  Serial.println("^C ");
  digitalWrite(4, HIGH);
  delay(1000);
  digitalWrite(4, LOW);

  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
   
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  
  Serial.println();   
  Serial.println("closing connection");
  Serial.println();
  delay(15000);


    
  Serial.print("connecting to ");
  Serial.println(host);

  // For humidity data
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  // We now create a URI for the request
  String url2 = "/update?key=WXO7XB1ASYRG05&field2=";
  
  // This will send the request to the server
  client.print(String("GET ") + url2 + humi + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");

  Serial.print("Requesting URL: ");
  Serial.print(url2);
  Serial.print(humi);
  Serial.println("% ");
  digitalWrite(5, HIGH);
  delay(1000);
  digitalWrite(5, LOW);
               
  unsigned long timeout2 = millis();
  while (client.available() == 0) {
    if (millis() - timeout2 > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }  

  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  
  Serial.println();  
  Serial.println("closing connection");
  Serial.println();

  delay(15000);
  //delay(285000);
}

