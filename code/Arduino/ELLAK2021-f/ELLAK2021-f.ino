// προσθήκη esp8266

#include "PMS.h"
#include "DHT.h"

#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#include <NewPing.h>

#include "SparkFunCCS811.h" // Click here to get the library: http://librarymanager/All#SparkFun_CCS811

#include "WiFiEsp.h"

#define DHTPIN 13
#define DHTTYPE DHT11

#define MAX_DISTANCE 60 // Maximum distance (in cm) to ping.

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

LiquidCrystal_I2C lcd(0x27, 20, 4);  // set the LCD address to 0x27 for a 16 chars and 4 line display

CCS811 mySensor(CCS811_ADDR);

// Initialize the Ethernet client object
WiFiEspClient client;

int sensor1 = 0;
int sensor2 = 0;

int currentState = -1;
int previousState = -1;

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
char ssid[] = "robotlab";     // WiFi SSID (name)
char pass[] = "robot2019";     // WiFi password
int status = WL_IDLE_STATUS;    // the WiFi radio's status

// IP του thingspeak.com
char server[] = "184.106.153.149";
// Write API Key του λογαριασμού στο thingspeak
char writeKey[] = "HZ39B3C7B85LWHZA";

void setup()
{
  dht.begin();
  
  //ARDUINO MEGA
  //
  Serial.begin(115200);   // GPIO1, GPIO3 (TX/RX pin on ESP-12E Development Board) 
  Serial1.begin(9600);  // GPIO2 (D4 pin on ESP-12E Development Board)
  Serial.println("hi");

  lcd.init();                      // initialize the lcd
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("1st EPAL of Trikala");
  lcd.setCursor(0,1);
  lcd.print("Visitors:");

  pinMode(WINDOW_SENSOR_PIN, INPUT_PULLUP); // set arduino pin to input pull-up mode

  // ccs811
  Wire.begin(); //Inialize I2C Harware

  //It is recommended to check return status on .begin(), but it is not
  //required.
//  CCS811Core::status returnCode = mySensor.begin();
  if( mySensor.begin() == false )
  {
    lcd.println("CCS811 Error...");
    while (1); //Hang if there was a problem.
  }

  // initialize serial3 for ESP module
  // ΠΡΟΣΟΧΗ: TX->RX3 ΚΑΙ RX->TX3
  Serial2.begin(115200);
  // initialize ESP module
  WiFi.init(&Serial2);

  // check for the presence of the shield
  if( WiFi.status() == WL_NO_SHIELD )
  {
    Serial.println("WiFi shield not present");
    // don't continue
    while(true);
  }

  // attempt to connect to WiFi network
  while( WiFi.status() != WL_CONNECTED )
  {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  // you're connected now, so print out the data
  Serial.println("You're connected to the wifi");
  Serial.println();
}

void loop()
{
  currentTime = millis();
  
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  if (pms.read(data))
  {
    pmsData10 = data.PM_AE_UG_1_0;
    pmsData25 = data.PM_AE_UG_2_5;
    pmsData100 = data.PM_AE_UG_10_0;
  }  

  windowState = digitalRead(WINDOW_SENSOR_PIN); // read state

  //Check to see if data is ready with .dataAvailable()
  if( mySensor.dataAvailable() )
  {
    //If so, have the sensor read and calculate the results.
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

    lcd.setCursor(0, 2);    //Get them later
    lcd.print("CO2=");
    lcd.print(co2);
//    lcd.print("          ");
    lcd.setCursor(0, 3);    
    //lcd.print(" ");
    lcd.print("tVOC=");
    //Returns calculated TVOC reading
    lcd.print(tvoc);
   // lcd.print("          ");    
    
    Serial.println("--------------------------------");
    
    previousTime1 = currentTime;    
  }

  // visitors count
  long distance1 = sonar1.ping_cm();
  long distance2 = sonar2.ping_cm();

  if( distance1 > 0 && distance1 < 40 )
  {
    t1 = millis();
    sensor1 = 1;
  }
  else
  {
    sensor1 = 0;
  }
  
  if( distance2 > 0 && distance2 < 40 )
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
      else
      {
        visitors--;
        if( visitors<0 )
        {
          visitors=0;
        }     
      }
      t1 = t2 = 0;
    }
  }
  
  lcd.setCursor(10,1);
  lcd.print("     ");
  lcd.setCursor(10,1);
  lcd.print(visitors);
/*  
  lcd.setCursor(0,2);
  lcd.print("               ");
  lcd.setCursor(0,2);
  lcd.print(distance1);
  lcd.setCursor(10,2);
  lcd.print(distance2);
  
  lcd.setCursor(0,3);
  lcd.print("               ");
  lcd.setCursor(0,3);
  lcd.print(currentState);
  lcd.setCursor(7,3);
  lcd.print(sensor1);
  lcd.setCursor(12,3);
  lcd.print(sensor2);
*/  
//  delay(30);
}
