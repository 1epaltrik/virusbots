// 01. Μέτρηση μικροσωματιδίων PM1.0, PM2.5 και PM10.0

#include "PMS.h"

// το PMS5003 συνδέεται στο 2ο hardware serial του Arduino Mega
PMS pms(Serial1);
PMS::DATA data;

void setup()
{
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial.println("hi");
}

void loop()
{
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
}
