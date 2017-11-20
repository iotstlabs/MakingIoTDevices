#include <ESP8266WiFi.h>
 
const char* ssid = "IoTStLabs";
const char* password = "fold1108"; 
//const char* ssid = "IOT_Center";
//const char* password = "i5tcqriti"; 
 
WiFiServer server(80);
 
void setup() {
  Serial.begin(9600);
  delay(10);
  pinMode(16, OUTPUT);  // D0
  pinMode(4, OUTPUT);   // D2
  pinMode(2, OUTPUT);   // D4
  pinMode(13, OUTPUT);  // D7
  digitalWrite(16, LOW);
  digitalWrite(4, LOW);
  digitalWrite(2, LOW);
  digitalWrite(13, LOW);
 
  // 와이파이 네트워크에 접속
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
 
  // 웹서버 개시
  server.begin();
  Serial.println("Server started!!");
 
  // 접속할 IP 주소 표시
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
  Serial.println(""); 
}
 
void loop() {
  // 클라이언트가 접속되었는지 확인
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
 
  // 클라이언트가 데이터를 보낼 때까지 기다림
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
 
  // 리퀘스트의 첫번째 줄을 읽어들임
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
 
  // 리퀘스트에 따른 LED 제어 
  if (request.indexOf("/LED1on") > 0)  {
    digitalWrite(16, HIGH);
  }
  if (request.indexOf("/LED1off") >0)  {
    digitalWrite(16, LOW);
  }

  if (request.indexOf("/LED2on") > 0)  {
    digitalWrite(4, HIGH);
  }
  if (request.indexOf("/LED2off") >0)  {
    digitalWrite(4, LOW);
  }
  
  if (request.indexOf("/LED3on") >0)  {
    digitalWrite(2, HIGH);
  }
  if (request.indexOf("/LED3off") > 0)  {
    digitalWrite(2, LOW);
  }
  
  if (request.indexOf("/LED4on") > 0)  {
    digitalWrite(13, HIGH);
  }
  if (request.indexOf("/LED4off") > 0)  {
    digitalWrite(13, LOW);
  }
 
  // 클라이언트 리퀘스트에 대한 응답 메시지
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<head>");
  client.println("<meta name='apple-mobile-web-app-capable' content='yes' />");
  client.println("<meta name='apple-mobile-web-app-status-bar-style' content='black-translucent' />");
  client.println("</head>");
  client.println("<hr/><hr>");
  client.println("<h4><center> Web Control of 4 LEDs  </center></h4>");
  client.println("<hr/><hr>");
  
  client.println("<center>");
  client.println("LED 1");
  client.println("<a href=\"/LED1on\"\"><button>Turn On </button></a>");
  client.println("<a href=\"/LED1off\"\"><button>Turn Off </button></a><br />");  
  client.println("</center>");  
   
  client.println("<center>");
  client.println("LED 2");
  client.println("<a href=\"/LED2on\"\"><button>Turn On </button></a>");
  client.println("<a href=\"/LED2off\"\"><button>Turn Off </button></a><br />");  
  client.println("</center>"); 
  
  client.println("<center>");
  client.println("LED 3");
  client.println("<a href=\"/LED3on\"\"><button>Turn On </button></a>");
  client.println("<a href=\"/LED3off\"\"><button>Turn Off </button></a><br />");  
  client.println("</center>"); 
  
  client.println("<center>");
  client.println("LED 4");
  client.println("<a href=\"/LED4on\"\"><button>Turn On </button></a>");
  client.println("<a href=\"/LED4off\"\"><button>Turn Off </button></a><br />");  
  client.println("</center>"); 
  client.println("<center>");
  
  client.println("<table border=\"4\">");
  client.println("<tr>");
  
  if (digitalRead(16)) 
    client.print("<td>LED 1 is ON</td>");
  else
    client.print("<td>LED 1 is OFF</td>");
  client.println("<br />");
             
  if (digitalRead(4)) 
    client.print("<td>LED 2 is ON</td>");
  else
    client.print("<td>LED 2 is OFF</td>");
  client.println("</tr>");

  client.println("<tr>");
  if (digitalRead(2)) 
    client.print("<td>LED 3 is ON</td>");
  else
    client.print("<td>LED 3 is OFF</td>");

  if (digitalRead(13)) 
    client.print("<td>LED 4 is ON</td>");
  else
    client.print("<td>LED 4 is OFF</td>");
  client.println("</tr>");

  client.println("</table>");
  client.println("</center>");
  client.println("</html>"); 
  delay(1);
  Serial.println("Client disonnected!!");
  Serial.println(""); 
}
