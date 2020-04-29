#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <ctype.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#define WIFI_SSID "NETWORK_NAME_HERE"
#define WIFI_PASSWORD "NETWORK_PASSWORD_HERE"
#define FIREBASE_HOST "FIREBASEIO_LINK_HERE"
#define FIREBASE_AUTH "AUTH_KEY_HERE"

int count = 1; // Indicates how many values were updated in database
int serial_val_count = 1;
char pump_str[1];
int numBytes;
//const long utcOffsetInSeconds = -28800; // UTC-8:00 PST
const long utcOffsetInSeconds = -25200; // UTC-7:00 PST (for Daylight Savings)

// DELETE THESE 4 VARIABLES (BELOW) LATER!!!
int hum_val = 81;
int pH_val = 2;
int temperature = 99; // in degrees Celsius
int waterLevel_status = 0;

const unsigned long ONE_SECOND                  = 1000UL;
const unsigned long ONE_MINUTE                  = 60UL * ONE_SECOND;
const unsigned long FIVE_MINUTES                = 5UL * 60UL * ONE_SECOND;
const unsigned long FIFTEEN_MINUTES             = 3UL * FIVE_MINUTES;

const uint8_t       pin_WaterPumpRelay          = 14; // GPIO D5 pin
const uint8_t       WATER_PUMP_OFF              = LOW;
const uint8_t       WATER_PUMP_ON               = HIGH;
//const unsigned long WATER_PUMP_TIME_ON          = 5UL  * ONE_MINUTE;
//const unsigned long WATER_PUMP_TIME_NEXT_CYCLE  = TWELVE_HOURS - WATER_PUMP_TIME_ON;

//const long utcOffsetInSeconds = -25200; // UTC-7:00 PST (for Daylight Savings)

int current_hour; // Current hour (in military time)
int current_min; // Current minute
int pump_control; // Current pump control value retrieved from Firebase



// Define NTP Client to get date and time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "north-america.pool.ntp.org", utcOffsetInSeconds);

void setup() {
  //Serial.begin(9600); => WORKS
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

  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH); // Connect to Firebase
  timeClient.begin(); // Connect to NTP server for date and time
}

void loop() {

  timeClient.update(); // Gets current date and time from NTP server  

  Serial.println(getCurrentTime());

  current_hour = getCurrentHour(); // 
  current_min = getCurrentMinutes();
  
  pump_control = fetchDatafromDB("/Controls/Pump"); // Obtain water pump
                                                    // control value
                                                    // from Firebase
  Serial.print("pump_control = ");
  Serial.println(pump_control);
                                                   
  // If user set water pump to be on, then turn it off
  if(pump_control == 1)
    setPumpState(WATER_PUMP_OFF);
  //Check if pump is off
  else if(pump_control == 0){
      // If the time between XX:00 and XX:14 every 4 hours, then
      // keep the water pumps on
      if((current_hour % 4) == 0){
        if(current_min <= 14){
          setPumpState(WATER_PUMP_ON);
        }
      }
  }
  
  // Every 4 hours at 0 minute mark, turn the water pumps on
  if (((current_hour % 4) == 0) && (current_min == 0)) {
    setPumpState(WATER_PUMP_ON);   
    Serial.println("pump_control == 0 => else branch");
  }

  // At 15 minute mark (starting from 0 minute mark)
  if (current_min == 15) {
     // if water pump is on, then turn it off
     if (digitalRead(pin_WaterPumpRelay) == 1) {
        setPumpState(WATER_PUMP_OFF);
        Serial.println("current_min == 15 branch");
     }
  }
  
  // Obtain data from Mega 2560 board every 15 minutes (0, 15, 30, 45 minute mark)
  if (current_min % 15 == 0) {
    //interrupt mega to take sensor data
    // => ASSUMPTION: place SERIAL COMM CODE HERE
    Serial.println("(current_min % 15 == 0 branch) => Need to obtain data from MEGA2560");

    // Check if there is data within the serial communications data buffer
    if (Serial.available()) {
      sendMegaDataToDB(); // Fetch Mega2560 data and send them to database
    }
  }

  Serial.println(" ");
  
  delay(1000);

}



/*
 * Name: Send Mega2560 Data to Database
 * Date Last Updated: April 01, 2020
 * Description: Fetch the data from the Arduino 2560 board in string form with 
 *              delimiters, break the string into individual strings, 
 *              convert the strings into numbers, and push those numbers 
 *              onto the database
 */
void sendMegaDataToDB() {
  numBytes = Serial.available(); // Number of bytes available for reading
                                   // from serial port
  char rx_String[numBytes];
  // Store each byte read into char array
  for (int i = 0; i < numBytes; i++) {
    delay(1);
    rx_String[i] = Serial.read(); // Store each char of string received
  }

  clearIncomingBuffer(); // Empty incoming serial data buffer (from Mega2560)
  
  float rx_Data[4]; // New array to store strings converted to numbers
  int float_index = 0;

  char *ch_ptr_rcv = NULL;
  char *temp_ch_arr_rcv[5];

  Serial.println(rx_String);

  ch_ptr_rcv = strtok(rx_String, ","); // Look for decimal point (,) delimiter
  Serial.println(ch_ptr_rcv);

  rx_Data[float_index] = atof(ch_ptr_rcv); // Convert alphabet to float value (including 
                                 // decimal point)
  float_index++;
  Serial.print("Float #: ");
  Serial.print(rx_Data[0]);
  Serial.println(" ");

  while (float_index < 4) {
    delay(1);
    ch_ptr_rcv = strtok(NULL, ","); // Look for next start of token with comma (,)
    rx_Data[float_index] = atof(ch_ptr_rcv); // Convert alphabet to float value (including 
                                   // decimal point)
    Serial.print("Point: ");
    Serial.print(atof(ch_ptr_rcv) );
    Serial.println(" ");

    float_index++;
    
  }

  // Update date and time
  //timeClient.update(); // Gets current date and time from NTP server
  updateDBValues("/Date&Time/", getCurrentDate());
  
  // Update humidity value
  updateDBValues("/Update/Hum", rx_Data[1]);

  // Update pH value
  updateDBValues("/Update/PH", rx_Data[0]);
  
  // Update temperature value
  updateDBValues("/Update/Temp", rx_Data[2]);

  // Update water level status
  updateDBValues("/Update/WL", rx_Data[3]);


  count++; // Increase number of values pushed to database

  delay(1000);  

}



/*
 * Name: Update Database (DB) Values
 * Date Last Updated: January 13, 2020
 * Description: Update the humidity, pH, and temperature values in the 
 *              database via real-time
 */
void updateDBValues(String db_path, float sensor_value) {
  // set value in Update path of database, e.g. "/Update/Hum"
  Firebase.setFloat(db_path, sensor_value);
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting " + db_path + " failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(100); 

  // If path is for water level (WL), update Water Level value in DB and 
  // then exit function
  if (db_path != "/Update/WL") {
    String newRecord_path = " "; // Path to new value in one of four upper 
                                 // storage sections of database 
                                 // (Humidity, PH, Temperature, or WL)

    if (db_path == "/Update/Hum") {
      newRecord_path = "/Humidity/";
    }
    else if (db_path == "/Update/PH") {
      newRecord_path = "/PH/";
    }
    else if (db_path == "/Update/Temp") {
      newRecord_path = "/Temperature/";
    }
    
    newRecord_path.concat(count);  // Concatenates string with number var "count"
  
    // set new value for desired path in database, e.g. "/Humidity/Hum"
    Firebase.setFloat(newRecord_path, sensor_value);
    // handle error
    if (Firebase.failed()) {
        Serial.print("setting " + newRecord_path + " failed:");
        Serial.println(Firebase.error());  
        return;
    }
  }
  delay(100); 

}


/*
 * Name: Update Database (DB) Values
 * Date Last Updated: April 3, 2020
 * Description: Update the current time and date in the database 
 *              via real-time
 */
void updateDBValues(String db_path, String currentDateTime) {
  int path = 1;

  while (path < 3) {
    String secondary_db_path = " "; // Path below /Date&Time/DT 
    if (path == 1) {
      db_path.concat(count);  // Concatenates string with number var "count"
      secondary_db_path = db_path + "/Date"; // Append secondary path to string
    }
    else if (path == 2) {
      secondary_db_path = db_path + "/Time"; // Append secondary path to string
      currentDateTime = getCurrentTime(); // Obtain current time (hr and min)
    }

    Firebase.setString(secondary_db_path, currentDateTime);
    // handle error
    if (Firebase.failed()) {
        Serial.print("setting " + secondary_db_path + " failed:");
        Serial.println(Firebase.error());  
        return;
    }
    delay(100); 

    path++; // Move to second path /Date&Time/DT#/Time
  }

  delay(100);
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


/*
 * Name: Get Current Date
 * Date Last Updated: April 3, 2020
 * Description: Obtain current date using NTP Client-Server
 */
String getCurrentDate() {
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
  unsigned long epochTime = timeClient.getEpochTime();

  //Get a time structure
  struct tm *ptm = gmtime ((time_t *)&epochTime); 

  int monthDay = ptm->tm_mday; // Day of the month
  int currentMonth = ptm->tm_mon+1; // Current month
  int currentYear = ptm->tm_year+1900;

  // Current date (year XXXX - month XX - day XX format)
  String currentDate = String(currentYear) + "-" + String(currentMonth) 
        + "-" + String(monthDay);
  
  delay(1000);

  return currentDate;
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
 
  delay(1000);

  return current_hours_min;
}


/*
 * Name: Clear Incoming Buffer
 * Date Last Updated: April 16, 2020
 * Description: Empty incoming or received data buffer via 
 *              serial communication
 */
void clearIncomingBuffer() {
  while(Serial.available()) {
      Serial.read();
    }
  delay(100);
}




void setPumpState(uint8_t state)
{
   digitalWrite(pin_WaterPumpRelay, state);
   //delay(1000);
}

/*
 * Name: Get Current Hour
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
