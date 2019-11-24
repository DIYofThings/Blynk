#define BLYNK_PRINT Serial
#define PIR_PIN D4

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "...";
char ssid[] = "...";
char pass[] = "...";

BlynkTimer timer;
WidgetLED PIR_LED(V0);

bool pir_status = 0;

void detection()
{
  if(digitalRead(PIR_PIN))
  {
    PIR_LED.on();
    if(!pir_status)
    {
      Blynk.notify("ตรวจพบการเคลื่อนไหวภายในพื้นที่!!");
      pir_status = 1;
    }
  }
  else
  {
    PIR_LED.off();
    pir_status = 0;
  }
}

void setup()
{
  Serial.begin(9600);
  pinMode(PIR_PIN,INPUT);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000L,detection);
}

void loop()
{
  // เริ่มการทำงาน
  Blynk.run();
  timer.run();
}
