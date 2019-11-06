#define BLYNK_PRINT Serial
// ขาที่ต่อกับ ultrasonic sensor
#define TRIGGER_PIN  5
#define ECHO_PIN     4

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = ".";
char ssid[] = ".";
char pass[] = ".";
//ตัวแปลสำหรับคำนวนระยะ และ เก็บค่าระยะทาง
long duration, distance;

BLYNK_WRITE(V1)
{
  // ตรวจสอบการกด ว่ากด(1) หรือ ปล่อย(0)  
  if(param.asInt())
  {
    // ทำการเช็คระยะ
    digitalWrite(TRIGGER_PIN, LOW); 
    delayMicroseconds(2); 
    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER_PIN, LOW);
    duration = pulseIn(ECHO_PIN, HIGH);

    // คำนวนระยะ หน่วย CM
    distance = (duration/2) / 29.1;
    
    // แสดงค่าออก Serial
    Serial.print(distance);
    Serial.println(" cm");

    //เขียนค่าไปที่ Blynk V0 Labeled Value Widget
    Blynk.virtualWrite(V0,distance);   
  }
}

void setup()
{
  Serial.begin(9600);
  // กำหนด pin mode
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT); 
  // กำหนเค่าเริ่มต้น Blynk
  Blynk.begin(auth, ssid, pass);
}

void loop()
{
  // เริ่มการทำงาน
  Blynk.run();
}
