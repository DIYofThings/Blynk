/*
 * DIY of Things 
 * Youtube https://www.youtube.com/watch?v=XGlzRtQ0sV8&list=PL7QMc6hqe_hDbg3i8dbd81He7nyfHcZWF
 * Facebook https://www.facebook.com/DIYofThings
 * Website http://www.diyofthings.com/
 * Copyright DIY of Things 2021
 */

#define BLYNK_PRINT Serial

// define PIN Mapping
#define PIN_RELAY1  D5  // relay 1 HEATER
#define PIN_RELAY2  D6  // relay 2 PUMP
#define PIN_DHT     D7  // dht temperture & humidity sensor
#define ON  LOW
#define OFF HIGH

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#define DHTPIN      PIN_DHT   
#define DHTTYPE     DHT22     // DHT 22 (AM2302)
DHT_Unified dht(DHTPIN, DHTTYPE);
sensors_event_t event;

char auth[] = "";
char ssid[] = "";
char pass[] = "";

float tempSensorData  = 0;
float humiSensorData  = 0;
bool  autoCondition;
int   limitHigh       = 35;
int   limitLow        = 33;
BlynkTimer timer;

void programProcess()
{
  
  if(autoCondition)
  {
    Serial.println(limitHigh);
    Serial.println(limitLow);
    Serial.println(tempSensorData);
    if(tempSensorData < limitLow)
    {
      //เปิด Heater
      Serial.println("Condition 1");
      digitalWrite(PIN_RELAY1,ON);
      Blynk.virtualWrite(V11,HIGH);
    }

    if((tempSensorData >= limitLow) && (tempSensorData <= limitHigh))
    {
      //เปิด Heater
      Serial.println("Condition 1");
      digitalWrite(PIN_RELAY1,ON);
      Blynk.virtualWrite(V11,HIGH);
    }
  
    if(tempSensorData > limitHigh)
    {
      // ปืก Heater
      Serial.println("Condition 2");
      digitalWrite(PIN_RELAY1,OFF);
      Blynk.virtualWrite(V11,LOW);
    }
  }
}

void updateSensor()
{
  // check temperature 
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  }
  else {
    tempSensorData = event.temperature;
    Serial.print(F("Temperature: "));
    Serial.print(tempSensorData);
    Serial.println(F("°C"));

    Blynk.virtualWrite(V0,tempSensorData);
  }
  
  // check humidity
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  }
  else {
    humiSensorData = event.relative_humidity;
    Serial.print(F("Humidity: "));
    Serial.print(humiSensorData);
    Serial.println(F("%"));

    Blynk.virtualWrite(V1,humiSensorData);
  }
}

void updateLCD()
{
    if(autoCondition)
    {
      lcd.setCursor(13,0);
      lcd.print("[A]");
    }
    else
    {
      lcd.setCursor(13,0);
      lcd.print("[M]");
    }
    lcd.setCursor(2,1);
    lcd.print(tempSensorData);
    lcd.setCursor(10,1);
    lcd.print(humiSensorData);
}

BLYNK_WRITE(V2)
{
  limitHigh = param.asInt();
}

BLYNK_WRITE(V3)
{
  limitLow = param.asInt();
}

BLYNK_WRITE(V10)
{
  autoCondition = param.asInt();
}

BLYNK_WRITE(V11)
{
  if(!autoCondition)
  {
    if(param.asInt())
    {
      digitalWrite(PIN_RELAY1,ON);
    }
    else
    {
      digitalWrite(PIN_RELAY1,OFF);
    }
  }
}

BLYNK_WRITE(V12)
{
  if(param.asInt())
  {
    digitalWrite(PIN_RELAY2,LOW);
    Blynk.setProperty(V11, "offLabel", "disable");
  }
  else
  {
    digitalWrite(PIN_RELAY2,HIGH);
  }
}

BLYNK_CONNECTED()
{
  Blynk.syncAll();
}

void setup()
{
  // Set Pin Mode Output
  pinMode(PIN_RELAY1, OUTPUT);     
  pinMode(PIN_RELAY2  , OUTPUT);    
      
  // Set default PIN Value HIG=0,LOW=1
  digitalWrite(PIN_RELAY1 , OFF);
  digitalWrite(PIN_RELAY2 , OFF);
  
  // Debug console
  Serial.begin(9600);
  dht.begin();

  lcd.init();  
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Smart Temp");
  lcd.setCursor(13,0);
  lcd.print("[M]");
  lcd.setCursor(0,1);
  lcd.print("T:      H:   ");
  
  Blynk.begin(auth, ssid, pass);
  updateSensor();
  timer.setInterval(2500L,updateSensor);
  timer.setInterval(5000L,programProcess);
  timer.setInterval(1000L,updateLCD);
}

void loop()
{
  Blynk.run();
  timer.run();
}
