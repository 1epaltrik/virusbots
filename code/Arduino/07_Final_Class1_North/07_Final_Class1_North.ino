// 07. Τελική έκδοση με βελτιώσεις - 1η αίθουσα - βόρεια

#include "PMS.h"
#include "DHT.h"

#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#include <NewPing.h>

#include "SparkFunCCS811.h"

#include "WiFiEsp.h"

#define DHTPIN 13
#define DHTTYPE DHT11

#define MAX_DISTANCE 70 // Η μέγιστη απόσταση (σε cm) που ελέγχουν οι αισθητήρες

#define WINDOW_SENSOR_PIN 7

#define CCS811_ADDR 0x5B //Default I2C Address
//#define CCS811_ADDR 0x5A //Alternate I2C Address

DHT dht(DHTPIN, DHTTYPE);

PMS pms(Serial1);
PMS::DATA data;

unsigned long currentTime;
unsigned long previousTime1 = 0;
unsigned long previousTime2 = 0;

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

int distance1 = 0;
int distance2 = 0;
int visitors = 0;

int pmsData10 = 0;
int pmsData25 = 0;
int pmsData100 = 0;

int windowState;
int winst=0;

int co2 = 0;
int tvoc = 0;

float h = 0.0;
float t = 0.0;

// Κωδικοί WiFi ΑΙΘΟΥΣΑ 1 ΒΟΡΕΙΑ
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

  // Εδώ χρησιμοποιούμε το PMS5003 σε passive mode, έτσι ώστε να διαβάζουμε τις μετρήσεις του όταν απαιτείται
  pms.passiveMode();    // Switch PMS5003 to passive mode
  pms.wakeUp();

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("1st EPAL of Trikala");

  pinMode(WINDOW_SENSOR_PIN, INPUT_PULLUP); // set arduino pin to input pull-up mode

  // ccs811
  Wire.begin(); // Initialize I2C Hardware

  if( mySensor.begin() == false )
  {
    lcd.println("CCS811 Error...");
    while(true);  // τέλος...
  }

  // Αρχικοποίηση του serial2 για το ESP module
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
    lcd.setCursor(0,2);
    lcd.print("Connecting...       ");    
    Serial.print("Attempting to connect to: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
  }

  Serial.println("You're connected to the wifi");
  Serial.println();
  lcd.setCursor(0,3);
  lcd.print("Connected!          ");
  delay(2000);
}

void loop()
{
  currentTime = millis();
  
  // Μέτρηση επισκεπτών
  delay(33);
  distance1 = sonar1.ping_cm();
  delay(33);
  distance2 = sonar2.ping_cm();
  
  if( distance1 > 0 && distance1 < MAX_DISTANCE )
  {
    sensor1 = 1;
    t1 = millis();
  }
  else
  {
    sensor1 = 0;
  }

  if( distance2 > 0 && distance2 < MAX_DISTANCE )
  {
    sensor2 = 1;
    t2 = millis();
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
          visitors--;
      }
      t1 = t2 = 0;
    }
    if( currentTime-t1 > 2000 ) t1=0;
    if( currentTime-t2 > 2000 ) t2=0;
  }
  
  if( currentTime - previousTime1 >= 5000 )
  {
    // διάβασμα CCS811
    if( mySensor.dataAvailable() )
    {
      mySensor.readAlgorithmResults();
      co2 = mySensor.getCO2();
      tvoc = mySensor.getTVOC();
    }

    // διάβασμα DHT11
    h = dht.readHumidity();
    t = dht.readTemperature();

    // διάβασμα PMS5003
    pms.requestRead();  
    if( pms.readUntil(data) )
    {
      pmsData10 = data.PM_AE_UG_1_0;
      pmsData25 = data.PM_AE_UG_2_5;
      pmsData100 = data.PM_AE_UG_10_0;
    }
    else
    {
      //Serial.println("No PMS data.");
    }
    
    Serial.print("PM 1.0 (ug/m3): ");
    Serial.println(pmsData10);
    Serial.print("PM 2.5 (ug/m3): ");
    Serial.println(pmsData25);
    Serial.print("PM 10.0 (ug/m3): ");
    Serial.println(pmsData100);
    Serial.println();
    
    Serial.print("CO2: ");
    Serial.println(co2);
    Serial.print("tVOC: ");
    Serial.println(tvoc);
    Serial.println();   
     
    Serial.print("Temperature: ");
    Serial.println(t);
    Serial.print("Humidity: ");
    Serial.println(h);
    Serial.println();

    windowState = digitalRead(WINDOW_SENSOR_PIN);
    if (windowState == HIGH)
    {
      Serial.println("The window is open");
      winst=1;
    }
    else
    {
      Serial.println("The window is closed");
      winst=0;
    }
    Serial.print("Visitors: ");
    Serial.println(visitors);
    Serial.println();

    lcd.setCursor(0,1);
    lcd.print("PM2.5=");
    lcd.setCursor(6,1);
    lcd.print("    ");
    lcd.setCursor(6,1);
    lcd.print(pmsData25);
    
    lcd.setCursor(10,1);
    lcd.print("PM10=");
    lcd.setCursor(15,1);
    lcd.print("     ");
    lcd.setCursor(15,1);
    lcd.print(pmsData100);

    lcd.setCursor(0, 2);
    lcd.print("CO2=");
    lcd.print("      ");
    lcd.setCursor(4, 2);
    lcd.print(co2);

    lcd.setCursor(10, 2);
    lcd.print("tVOC=");
    lcd.print("    ");    
    lcd.setCursor(15, 2);
    lcd.print(tvoc);
    
    lcd.setCursor(0, 3);
    lcd.print("T=");
    lcd.print("      ");
    lcd.setCursor(2, 3);
    lcd.print(t);
    lcd.print("oC ");
    lcd.setCursor(10, 3);
    lcd.print("H=");
    lcd.print("      ");    
    lcd.setCursor(12, 3);
    lcd.print(h);
    lcd.print("%  ");

    Serial.println("--------------------------------");
    
    previousTime1 = currentTime;
  }
  
  // κάθε 16 sec,
  if( currentTime - previousTime2 >= 16000 )
  {
    // Συνδέσου με το thingspeak για αποστολή των δεδομένων 
    Serial.println("Starting connection to thingspeak..."); 
    client.stop();
    if( client.connect(server, 80) ) 
    { 
      Serial.println("Connected to thingspeak.com"); 
      // Δημιουργία του αιτήματος HTTP, όπως απαιτεί το thingspeak.com 
      String request= "GET /update?key="; 

      request += writeKey; 
      
      request += "&field1="; 
      request += String(pmsData25); 
  
      request += "&field2="; 
      request += String(pmsData100); 
      
      request += "&field3="; 
      request += String(co2); 
      
      request += "&field4="; 
      request += String(tvoc); 
      
      request += "&field5="; 
      request += String(visitors); 
      
      request += "&field6="; 
      request += String(winst);
       
      request += "&field7="; 
      request += String(t); 
  
      request += "&field8="; 
      request += String(h); 
    
      Serial.println(request); 
      // Εμφάνιση στο serial monitor για debugging 
      // Αποστολή του αιτήματος client.println(request); 
      // client.println("GET /asciilogo.txt HTTP/1.1"); 
      client.println(request);
      client.println("Host: 184.106.153.149");
    
      client.println("Connection: close"); 
      client.println(); 
    }
    previousTime2 = currentTime;
  }
}
