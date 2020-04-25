#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <ctype.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#define WIFI_SSID "NETWORK_NAME_HERE"
#define WIFI_PASSWORD "NETWORK_PASSWORD_HERE"
#define FIREBASE_HOST "FIREBASEIO_LINK_HERE"
#define FIREBASE_AUTH "AUTH_KEY_HERE"

const unsigned long ONE_SECOND                  = 1000UL;
const unsigned long ONE_MINUTE                  = 60UL * ONE_SECOND;
const unsigned long FIVE_MINUTES                = 5UL * 60UL * ONE_SECOND;
const unsigned long FIFTEEN_MINUTES             = 3UL * FIVE_MINUTES;

const uint8_t       pin_WaterPumpRelay          = 2; // GPIO D4 pin
const uint8_t       WATER_PUMP_OFF              = LOW;
const uint8_t       WATER_PUMP_ON               = HIGH;
//const unsigned long WATER_PUMP_TIME_ON          = 5UL  * ONE_MINUTE;
//const unsigned long WATER_PUMP_TIME_NEXT_CYCLE  = TWELVE_HOURS - WATER_PUMP_TIME_ON;

const long utcOffsetInSeconds = -25200; // UTC-7:00 PST (for Daylight Savings)

int current_hour; // Current hour (in military time)
int current_min; // Current minute
int pump_control; // Current pump control value retrieved from Firebase

// Define NTP Client to get date and time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "north-america.pool.ntp.org", utcOffsetInSeconds);



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
  setPumpState(WATER_PUMP_OFF);
  //digitalWrite(pin_WATER_PUMP_RELAY, HIGH);
  //digitalWrite(pin_WaterPumpRelay, HIGH);

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  timeClient.begin();
}

void loop() {

  timeClient.update(); // Gets current date and time from NTP server  

  Serial.println(getCurrentTime());

  current_hour = getCurrentHour();
  current_min = getCurrentMinutes();
  
  
  pump_control = fetchDatafromDB("/Controls/Pump");

  Serial.print("pump_control = ");
  Serial.println(pump_control);
  
/*
  if (pump_control == 1) {
    //setPumpState(WATER_PUMP_ON);
    Serial.println("In pump_con == 1 branch");
    delay(10000);
  }
  else if (pump_control == 0) {
    //setPumpState(WATER_PUMP_OFF);
    Serial.println("In pump_con == 0 branch");
    delay(10000);
  }
  */

  // If manual pump control is 0, continue with automatic water cycle
  if (pump_control == 0){
    // Every 4 hours at 0 minute mark
    if (((current_hour % 4) == 0) && (current_min == 0)) {
       if (pump_control == 1) {
         //setPumpState(WATER_PUMP_OFF);
         Serial.println("pump_control == 1 => if branch");
       }
       else {
          //setPumpState(WATER_PUMP_ON);   
          Serial.println("pump_control == 0 => else branch");
       }
    }

    // At 15 minute mark (starting from 0 minute mark)
    if (current_min == 15) {
       // if water pump is on, then turn it off
       if (digitalRead(pin_WaterPumpRelay) == 1) {
          //setPumpState(WATER_PUMP_OFF);
          Serial.println("current_min == 15 branch");
       }
    }
  }
  
  // Obtain data from Mega 2560 board every 15 minutes (0, 15, 30, 45 minute mark)
  if (current_min % 15 == 0) {
    //interrupt mega to take sensor data
    // => ASSUMPTION: place SERIAL COMM CODE HERE
    Serial.println("(current_min % 15 == 0 branch) => Need to obtain data from MEGA2560");
  }

  Serial.println(" ");

  delay(500); // 1/2 second delay
}


void setPumpState(uint8_t state)
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
 * Date Last Updated: April 24, 2020
 * Description: Obtain current hour (in military time) using NTP Client-Server
 */
int getCurrentHour() {
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
  //String _current_min = String(timeClient.getMinutes());
  delay(100);

  return timeClient.getHours();
}




/*
 * Name: Get Current Minutes
 * Date Last Updated: April 17, 2020
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
