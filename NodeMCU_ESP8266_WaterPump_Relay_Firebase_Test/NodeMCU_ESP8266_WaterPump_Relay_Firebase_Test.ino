#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <ctype.h>
#include <NTPClient.h>
#include <WiFiUdp.h>


#define WIFI_SSID ""
#define WIFI_PASSWORD ""
#define FIREBASE_HOST "lagoon-9de60.firebaseio.com"
#define FIREBASE_AUTH ""

const unsigned long ONE_SECOND                  = 1000UL;
const unsigned long ONE_MINUTE                  = 60UL * ONE_SECOND;
const unsigned long FIVE_MINUTES                = 5UL * 60UL * ONE_SECOND;
const unsigned long FIFTEEN_MINUTES             = 3UL * FIVE_MINUTES;

//const uint8_t       pinWATER_PUMP_RELAY         = 13;
//const uint8_t       pin_WATER_PUMP_RELAY        = 2;
const uint8_t       pin_WaterPumpRelay          = 2; // GPIO D4 pin

const uint8_t       WATER_PUMP_OFF              = LOW;
const uint8_t       WATER_PUMP_ON               = HIGH;
//const unsigned long WATER_PUMP_TIME_ON          = 5UL  * ONE_MINUTE;
//const unsigned long WATER_PUMP_TIME_NEXT_CYCLE  = TWELVE_HOURS - WATER_PUMP_TIME_ON;

const long utcOffsetInSeconds = -25200; // UTC-7:00 PST (for Daylight Savings)

// Define NTP Client to get date and time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "north-america.pool.ntp.org", utcOffsetInSeconds);

int current_min;
int pump_con;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  //Serial1.begin(115200);

  // connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected: ");
  Serial.println(WiFi.localIP());
  
  //pinMode(pin_WATER_PUMP_RELAY, OUTPUT);
  pinMode(pin_WaterPumpRelay, OUTPUT);
  setPump(WATER_PUMP_OFF);
  //digitalWrite(pin_WATER_PUMP_RELAY, HIGH);
  //digitalWrite(pin_WaterPumpRelay, HIGH);

  timeClient.begin();
}

void loop() {

  pump_con = fetchDatafromDB("/Controls/Pump");

  Serial.print("pump_con = ");
  Serial.println(pump_con);
/*
  timeClient.update(); // Gets current date and time from NTP server  

  current_min = getCurrentMinutes() % 15;

  if (current_min == 0) {
    setPump(WATER_PUMP_ON);   
  }
*/

  if (pump_con == 1) {
    //setPump(WATER_PUMP_ON);
    Serial.println("In pump_con == 1 branch");
    delay(10000);
  }
  else if (pump_con == 0) {
    //setPump(WATER_PUMP_OFF);
    Serial.println("In pump_con == 0 branch");
    delay(10000);
  }

}


void setPump(uint8_t state)
{
   digitalWrite(pin_WaterPumpRelay, state);
   //delay(1000);
}

/*
 * Name: Get Current Time
 * Date Last Updated: April 3, 2020
 * Description: Obtain current time using NTP Client-Server
 */
String getCurrentTime() {
  /*****************************************************************
  *    Title: ESP8266 NodeMCU NTP Client-Server: Get Date and Time
  *           (Arduino IDE)
  *    Author: Random Nerd Tutorials Staff
  *    Date: March 2020
  *    Code version: N/A
  *    Availability: 
  *           https://randomnerdtutorials.com/esp8266-nodemcu-date-time-ntp-client-server-arduino/
  *
  ******************************************************************/
  // Current hour and minutes (in military time)
  String current_hours_min = String(timeClient.getHours()) + ":" +
        String(timeClient.getMinutes());
 
  delay(100);

  return current_hours_min;
}


/*
 * Name: Get Current Minutes
 * Date Last Updated: April 3, 2020
 * Description: Obtain current minutes using NTP Client-Server
 */
int getCurrentMinutes() {
  /*****************************************************************
  *    Title: ESP8266 NodeMCU NTP Client-Server: Get Date and Time
  *           (Arduino IDE)
  *    Author: Random Nerd Tutorials Staff
  *    Date: March 2020
  *    Code version: N/A
  *    Availability: 
  *           https://randomnerdtutorials.com/esp8266-nodemcu-date-time-ntp-client-server-arduino/
  *
  ******************************************************************/
  // Current hour and minutes (in military time)
  //String current_hours_min = String(timeClient.getHours()) + ":" +
  //      String(timeClient.getMinutes());

  //String _current_min = String(timeClient.getMinutes());
  delay(100);

  return timeClient.getMinutes();
}




/*
 * Name: Fetch Data from Database (DB)
 * Date Last Updated: March 21, 2020
 * Description: Retrieve an integer value by using the desired path to 
 *              navigate through the database 
 */
int fetchDatafromDB(String db_path) {
  //if (!Firebase.failed()) {
    int val = Firebase.getInt(db_path); // Obtain integer value from specified DB path

    if (Firebase.failed()) {
      Serial.print("Obtaining value from " + db_path + " failed:");
      Serial.println(Firebase.error());  
      return 0;
    }
    delay(1000);
    
    Serial.print("Value: ");
    Serial.println(val);    

    return val;
    
  //} 
}
