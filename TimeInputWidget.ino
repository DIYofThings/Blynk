#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "xxxxx";
char ssid[] = "xxxxx";
char pass[] = "xxxxx";

BLYNK_WRITE(V1) {

  Serial.print("Start Time : ");
  Serial.println(param[0].asLong());
  Serial.print("Stop Time : ");
  Serial.println(param[1].asLong());
  Serial.print("Time Zone : ");
  Serial.println(param[4].asLong());

  
  TimeInputParam t(param);
  // Process start time
  if (t.hasStartTime())
  {
    Serial.println(String("Start: ") +
                   t.getStartHour() + ":" +
                   t.getStartMinute() + ":" +
                   t.getStartSecond());
  }
  else if (t.isStartSunrise())
  {
    Serial.println("Start at sunrise");
  }
  else if (t.isStartSunset())
  {
    Serial.println("Start at sunset");
  }
  else
  {
    // Do nothing
  }

  // Process stop time

  if (t.hasStopTime())
  {
    Serial.println(String("Stop: ") +
                   t.getStopHour() + ":" +
                   t.getStopMinute() + ":" +
                   t.getStopSecond());
  }
  else if (t.isStopSunrise())
  {
    Serial.println("Stop at sunrise");
  }
  else if (t.isStopSunset())
  {
    Serial.println("Stop at sunset");
  }
  else
  {
    // Do nothing: no stop time was set
  }

  // Process timezone
  // Timezone is already added to start/stop time

  Serial.println(String("Time zone: ") + t.getTZ());

  // Get timezone offset (in seconds)
  Serial.println(String("Time zone offset: ") + t.getTZ_Offset());

  // Process weekdays (1. Mon, 2. Tue, 3. Wed, ...)

  for (int i = 1; i <= 7; i++) {
    if (t.isWeekdaySelected(i)) {
      Serial.println(String("Day ") + i + " is selected");
    }
  }

  Serial.println();
}

BLYNK_CONNECTED() {
  //seconds from the start of a day. 0 - min, 86399 - max
  int startAt = 5 * 60; //00:05

  //seconds from the start of a day. 0 - min, 86399 - max
  int stopAt = (60 + 5) * 60; //01:05

  //timezone
  //full list of supported timezones could be found here
  //https://www.mkyong.com/java/java-display-list-of-timezone-with-gmt/
  char tz[] = "Europe/Kiev";

  Blynk.virtualWrite(V1, startAt, stopAt, tz);

  //you may also pass day
  //char days[] = "1"; //Monday
  //Blynk.virtualWrite(V1, startAt, stopAt, tz, days);

  //or days
  //char days[] = "1,2,3"; //Monday, Tuesday, Wednesday
  //Blynk.virtualWrite(V1, startAt, stopAt, tz, days);
}

void setup()
{
  // Debug console
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass);
}

void loop()
{
  Blynk.run();
}
