#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <TimeLib.h> 
#include "SSD1306.h" 

const char* ssid = "IoTStLabs"; 
const char* password = "fold1108"; 

// NTP 서버
IPAddress timeServer(132, 163, 4, 101); // time-a.timefreq.bldrdoc.gov

const int timeZone = 9;         // 표준 시간대를 설정

WiFiUDP Udp;
unsigned int localPort = 8888;  // UDP 패킷을 수신하기 위한 로컬 포트

SSD1306  display(0x3c, D3, D5);

void setup() 
{
  Serial.begin(9600);
  Serial.println();
  Serial.println("SSD1306/NTP_Time");
  Serial.print("Connecting to ");
  Serial.println(ssid);  

  display.init();
  display.flipScreenVertically();
  display.setTextAlignment(TEXT_ALIGN_LEFT);  
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 2, "SSD1306 &");
  display.drawString(0, 20, "NTP_Time");
  display.setFont(ArialMT_Plain_10);  
  display.drawString(0, 42, "Connecting to ");
  display.drawString(0, 54, ssid);  
  display.setFont(ArialMT_Plain_16);  
  display.display();  
    
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Starting UDP!!");
  Udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(Udp.localPort());
  Serial.println("waiting for sync...");
  setSyncProvider(getNtpTime);
}

time_t prevDisplay = 0; // when the digital clock was displayed

void loop()
{ 
  display.clear(); 
  if (timeStatus() != timeNotSet) {
    if (now() != prevDisplay) { // 시간이 바뀌었을 때만 출력 내용을 업데이트
      prevDisplay = now();
      digitalClockDisplay();  
    }
  }
  display.display();
  delay(1000);
}

void digitalClockDisplay(){ 
  String hhmmss, mmddyy;
  
  hhmmss = hour();
  hhmmss += " : ";
  hhmmss += minute();
  hhmmss += " : ";
  hhmmss += second();
  display.drawString(23, 17, hhmmss);

  mmddyy = month();
  mmddyy += " / ";
  mmddyy += day();
  mmddyy += " / ";
  mmddyy += year();
  display.drawString(17, 37, mmddyy);

  Serial.print(hhmmss);
  Serial.print(" <> ");
  Serial.println(mmddyy);   
}

//-------- NTP code ---------- //
const int NTP_PACKET_SIZE = 48; 
byte packetBuffer[NTP_PACKET_SIZE]; 

time_t getNtpTime()
{
  while (Udp.parsePacket() > 0) ; 
  Serial.println("Transmit NTP Request");
  sendNTPpacket(timeServer);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      Serial.println("Receive NTP Response");
      Udp.read(packetBuffer, NTP_PACKET_SIZE);
      unsigned long secsSince1900;

      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL + timeZone * 3600;
    }
  }
  Serial.println("No NTP Response!!");
  return 0; 
}

void sendNTPpacket(IPAddress &address)
{
  memset(packetBuffer, 0, NTP_PACKET_SIZE);

  packetBuffer[0] = 0b11100011;   
  packetBuffer[1] = 0;     
  packetBuffer[2] = 6;     
  packetBuffer[3] = 0xEC;  
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;
             
  Udp.beginPacket(address, 123); 
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}
