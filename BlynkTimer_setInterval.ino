#define BLYNK_PRINT Serial
#define SOLENOID 2

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "***";
char ssid[] = "***";
char pass[] = "***";

bool switchStatus = 0; // 0 = manual,1=auto
int soilMoistureLimit = 0;
bool manualSwitch = 0;

BlynkTimer timer;

// sync all status
BLYNK_CONNECTED() {
    Blynk.syncAll();
}

// update switchStatus
BLYNK_WRITE(V0)
{   
  switchStatus = param.asInt(); // Get value as integer
}

// update soilMosture setting
BLYNK_WRITE(V1)
{   
  soilMoistureLimit = param.asInt(); // Get value as integer
}

// update manualSwitch
BLYNK_WRITE(V2)
{
  manualSwitch = param.asInt();
}

void manualAutoAction()
{
  if(switchStatus)
  {
    // auto
    if(analogRead(A0) < soilMoistureLimit)
    {
      // D6 connect relay 
      digitalWrite(SOLENOID,LOW);
    }  
    else
    {
      digitalWrite(SOLENOID,HIGH);
    }
  }
  else
  {
    if(manualSwitch)
    {
      digitalWrite(SOLENOID,LOW);
    }
    else
    {
      digitalWrite(SOLENOID,HIGH);
    }
    // manaul
  }
}

void checkTemp()
{
  Serial.println("CheckTemp");
}

void setup()
{
  // Debug console
  Serial.begin(9600);
  pinMode(SOLENOID,OUTPUT);
  digitalWrite(SOLENOID,HIGH);
  
  Blynk.begin(auth, ssid, pass);

  // key
  timer.setInterval(1000L, manualAutoAction);
  timer.setInterval(10000L, checkTemp);
}

void loop()
{
  Blynk.run();
  timer.run();
}
