// 02. Προσθήκη μέτρησης Θερμοκρασίας και Υγρασίας

#include "PMS.h"
#include "DHT.h"

#define DHTPIN 13
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

PMS pms(Serial1);
PMS::DATA data;

unsigned long currentTime;
unsigned long previousTime1 = 0;

void setup()
{
  dht.begin();
  
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial.println("hi");
}

void loop()
{
  currentTime = millis();
  
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (pms.read(data))
  {
    Serial.print("PM 1.0 (ug/m3): ");
    Serial.println(data.PM_AE_UG_1_0);

    Serial.print("PM 2.5 (ug/m3): ");
    Serial.println(data.PM_AE_UG_2_5);

    Serial.print("PM 10.0 (ug/m3): ");
    Serial.println(data.PM_AE_UG_10_0);

    Serial.println();
  }

  // εμφάνιση μετρήσεων κάθε 3 sec
  if( currentTime - previousTime1 >= 3000 )
  {
    Serial.print("Temperature: ");
    Serial.println(t);
    Serial.print("Humidity: ");
    Serial.println(h);
    Serial.println("--------------------------------");
    previousTime1 = currentTime;    
  }
}
