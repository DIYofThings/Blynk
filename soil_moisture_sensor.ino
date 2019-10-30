#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

//WidgetLED soil_led(v1)
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "XXX";
char ssid[] = "XXX";
char pass[] = "XXX";

// กำหนดให้เซอเวออ่านค่า Virtual Pin 0 จะส่งค่า A0 กลับไป
BLYNK_READ(V0)
{
  Blynk.virtualWrite(V0,analogRead(A0));
}

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
}

void loop()
{
  Blynk.run();
}
