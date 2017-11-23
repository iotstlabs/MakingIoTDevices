float MeasuredDust=0;
float DustDensity = 0;

int LedPower=16;   // GPIO16 --> Sensor's LED pin (3rd Pin)
int DelayTime=280;
int DelayTime2=40;
float OffTime=9680;

void setup(){
  Serial.begin(9600);
  pinMode(LedPower, OUTPUT);
} 

void loop(){
  float FineDust = 0.0;
    
  digitalWrite(LedPower, LOW); // power on the LED
  delayMicroseconds(DelayTime);

  //먼지 센서를 값을 0.0V~3.3V 을 0~1024 값으로 변경해 줌.
  MeasuredDust=analogRead(A0); // read the dust value 
  delayMicroseconds(DelayTime2);
  digitalWrite(LedPower, HIGH); // turn the LED off
  delayMicroseconds(OffTime);

  // 볼트를 기준으로 미세먼지 값으로 변환해 줌, 데이터 시트을 참고 오차가 존재합니다.
  FineDust = 0.17*MeasuredDust*(3.3/1024)*1000; 
  
  //아날로그로 읽어 들인 0-1024 수치
  Serial.print("Measured Value = ");
  Serial.print(MeasuredDust);

  // 미세먼지 수치 출력
  Serial.print(",  Dust Density(ug/m3) = ");
  Serial.println(FineDust);

  delay(3000);
}

