#define BLYNK_PRINT Serial
#define GATE_PIN D6

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Servo.h>

Servo petGate;
int gateStart     = 160;
int gateEnd       = 45;
int gateOpenTime  = 500;

char auth[] = "***";
char ssid[] = "***";
char pass[] = "***";

BLYNK_WRITE(V0)
{
  petGate.write(gateEnd);
  delay(gateOpenTime);
  petGate.write(gateStart);
}

void setup()
{
  // Debug console 
  Serial.begin(9600);
  petGate.attach(GATE_PIN);
  petGate.write(gateStart);
  Blynk.begin(auth, ssid, pass);
}

void loop()
{
  Blynk.run();
}
