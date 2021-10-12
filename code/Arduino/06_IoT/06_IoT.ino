// 06. Προσθήκη πρόσβασης στο διαδίκτυο με το ESP8266-01 WiFi module

#include "PMS.h"
#include "DHT.h"

#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#include <NewPing.h>

#include "SparkFunCCS811.h"

#include "WiFiEsp.h"

#define DHTPIN 13
#define DHTTYPE DHT11

#define MAX_DISTANCE 60 // Η μέγιστη απόσταση (σε cm) που ελέγχουν οι αισθητήρες

#define WINDOW_SENSOR_PIN 7

#define CCS811_ADDR 0x5B //Default I2C Address
//#define CCS811_ADDR 0x5A //Alternate I2C Address

DHT dht(DHTPIN, DHTTYPE);

PMS pms(Serial1);
PMS::DATA data;

unsigned long currentTime;
unsigned long previousTime1 = 0;

NewPing sonar1 = NewPing(9, 8, MAX_DISTANCE);
NewPing sonar2 = NewPing(11, 10, MAX_DISTANCE);

LiquidCrystal_I2C lcd(0x27, 20, 4);  // οθόνη LCD 20 χαρακτήρων και 4 γραμμών στη διεύθυνση 0x27

CCS811 mySensor(CCS811_ADDR);

// Initialize the Ethernet client object
WiFiEspClient client;

int sensor1 = 0;
int sensor2 = 0;

unsigned long t1 = 0;
unsigned long t2 = 0;

int visitors = 0;

int pmsData10 = 0;
int pmsData25 = 0;
int pmsData100 = 0;

int windowState;

int co2 = 0;
int tvoc = 0;

// Κωδικοί WiFi
char ssid[] = "ΧΧΧΧΧΧΧΧ";     // WiFi SSID (name)
char pass[] = "ΧΧΧΧΧΧΧΧ";     // WiFi password
int status = WL_IDLE_STATUS;

// IP του thingspeak.com
char server[] = "184.106.153.149";
// Write API Key του λογαριασμού στο thingspeak
char writeKey[] = "ΧΧΧΧΧΧΧΧΧΧΧΧΧΧΧΧ";

void setup()
{
  dht.begin();

  Serial.begin(115200);
  Serial1.begin(9600);
  Serial.println("hi");

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("1st EPAL of Trikala");
  lcd.setCursor(0,1);
  lcd.print("Visitors:");

  pinMode(WINDOW_SENSOR_PIN, INPUT_PULLUP); // set arduino pin to input pull-up mode

  // ccs811
  Wire.begin(); // Initialize I2C Hardware

  if( mySensor.begin() == false )
  {
    lcd.println("CCS811 Error...");
    while(true);  // τέλος...
  }

  // Αρχικοποίηση του serial3 για το ESP module
  // ΠΡΟΣΟΧΗ: TX->RX3 ΚΑΙ RX->TX3
  Serial2.begin(115200);
  // initialize ESP module
  WiFi.init(&Serial2);

  // έλεγχος για το ESP8266
  if( WiFi.status() == WL_NO_SHIELD )
  {
    Serial.println("WiFi shield not present");
    while(true);  // τέλος...
  }

  // σύνδεση στο WiFi
  while( WiFi.status() != WL_CONNECTED )
  {
    Serial.print("Attempting to connect to: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
  }

  Serial.println("You're connected to the wifi");
  Serial.println();
}

void loop()
{
  currentTime = millis();

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (pms.read(data))
  {
    pmsData10 = data.PM_AE_UG_1_0;
    pmsData25 = data.PM_AE_UG_2_5;
    pmsData100 = data.PM_AE_UG_10_0;
  }  

  windowState = digitalRead(WINDOW_SENSOR_PIN);

  // διάβασμα CCS811
  if( mySensor.dataAvailable() )
  {
    mySensor.readAlgorithmResults();
    co2 = mySensor.getCO2();
    tvoc = mySensor.getTVOC();
  }
  
  if( currentTime - previousTime1 >= 3000 )
  {
    Serial.print("PM 1.0 (ug/m3): ");
    Serial.println(pmsData10);
    Serial.print("PM 2.5 (ug/m3): ");
    Serial.println(pmsData25);
    Serial.print("PM 10.0 (ug/m3): ");
    Serial.println(pmsData100);
    Serial.println();
    
    Serial.print("Temperature: ");
    Serial.println(t);
    Serial.print("Humidity: ");
    Serial.println(h);
    Serial.println();

    if (windowState == HIGH)
    {
      Serial.println("The window is open");
    }
    else
    {
      Serial.println("The window is closed");
    }

    lcd.setCursor(0, 2);
    lcd.print("CO2=");
    lcd.print(co2);
    lcd.setCursor(0, 3);    
    lcd.print("tVOC=");
    lcd.print(tvoc);
    
    Serial.println("--------------------------------");
    
    previousTime1 = currentTime;    
  }

  // Μέτρηση επισκεπτών
  long distance1 = sonar1.ping_cm();
  long distance2 = sonar2.ping_cm();

  if( distance1 > 0 && distance1 < MAX_DISTANCE )
  {
    t1 = millis();
    sensor1 = 1;
  }
  else
  {
    sensor1 = 0;
  }
  
  if( distance2 > 0 && distance2 < MAX_DISTANCE )
  {
    t2 = millis();
    sensor2 = 1;
  }
  else
  {
    sensor2 = 0;
  }

  if( sensor1 == 0 && sensor2 == 0 )
  {
    if( t1 > 0 && t2 > 0 )
    {
      if( t2 > t1 )
      {
        visitors++;
      }
      else if( t1 > t2 )
      {
        if( visitors > 0 )
        {
          visitors--;
        }
      }
      t1 = t2 = 0;
    }
  }
  
  lcd.setCursor(10,1);
  lcd.print("     ");
  lcd.setCursor(10,1);
  lcd.print(visitors);
}
