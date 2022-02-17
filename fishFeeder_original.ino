#define BLYNK_PRINT Serial
#define GATE_PIN 12

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Servo.h>

Servo petGate;
int gateStart     = 180;
int gateEnd       = 0;
int gateOpenTime  = 1000;
int gateLoop      = 1;

char auth[] = "";
char ssid[] = "";
char pass[] = "";

BLYNK_CONNECTED()
{
  Blynk.syncAll();
}

BLYNK_WRITE(V0)
{
  if(param.asInt() == 1)
  {
    Serial.print("Feeding");
    for(int i=1;i<=gateLoop;i++)
    {
      Serial.print(".");
      petGate.write(gateEnd);
      delay(gateOpenTime);
      petGate.write(gateStart);
      delay(gateOpenTime);
    }
    Serial.println("Done!!");
  }
}

BLYNK_WRITE(V1)
{
  gateLoop = param.asInt();
  Serial.print("Set loop:");
  Serial.println(gateLoop);
}

void setup()
{
  // Debug console 
  Serial.begin(115200);
  Serial.println("start");
  petGate.attach(GATE_PIN);
  petGate.write(gateStart);
  Blynk.begin(auth, ssid, pass);
}

void loop()
{
  Blynk.run();
}
