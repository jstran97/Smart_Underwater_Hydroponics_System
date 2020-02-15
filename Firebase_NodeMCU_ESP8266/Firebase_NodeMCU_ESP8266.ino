//
// Copyright 2015 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

// FirebaseDemo_ESP8266 is a sample that demo the different functions
// of the FirebaseArduino API.

#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#define WIFI_SSID "iPhone69 Hotspot"
#define WIFI_PASSWORD "PACEISCOOL"
//#define WIFI_SSID "iPhone69 Hotspot"
//#define WIFI_PASSWORD "PACEISCOOL"
#define FIREBASE_HOST "lagoon-9de60.firebaseio.com"
#define FIREBASE_AUTH "aWjHUYVg9d7ZJEqKVW8tgaRHf4WlRt90FxFgze3H"

void setup() {
  Serial.begin(9600);

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
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

int count = 1; // Indicates how many values were updated in database
int serial_val_count = 1;
int hum_val = 50;
int pH_val = 3;
int temperature = 20;
String waterLevel_status = "Red";

void loop() {
  if (Serial.available()) {
    //Serial.write(Serial.read());

    if (serial_val_count == 1) {
      // Update humidity value
      updateDBValues("/Update/Hum", Serial.read());
      Serial.println("serial_val_count = 1 " + Serial.read());
      serial_val_count++; // Increase count of received values from serial comm
    }
    //else if (serial_val_count == 2) {
      // Update pH value
      //updateDBValues("/Update/PH", Serial.read());
      serial_val_count++; // Increase count of received values from serial comm
    //}
    else if (serial_val_count == 2) {
      // Update temperature value
      updateDBValues("/Update/Temp", Serial.read());
      serial_val_count == 1; // Reset to 1
      Serial.println("serial_val_count = 2 " + Serial.read());
      
      count++; // Increase number of values pushed to database
    }
    
  }
  
  delay(10000); // 10 sec delay
  //delay(300000); // 5 min delay 
}


// Update the date & time, humidity, pH, and temperature values in real-time database
void updateDBValues(String db_path, int sensor_value) {
  // set value in Update path of database, e.g. "/Update/Hum"
  Firebase.setInt(db_path, sensor_value);
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting " + db_path + " failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(100); 

  String newRecord_path = " "; // Path to new value in one of four upper storage sections
                               // of database (Date&Time, Humidity, PH, or Temperature)

  if (db_path == "/Update/Hum") {
    newRecord_path = "/Humidity/H";
  }
  else if (db_path == "/Update/PH") {
    newRecord_path = "/PH/P";
  }
  else if (db_path == "/Update/Temp") {
    newRecord_path = "/Temperature/T";
  }
  
  newRecord_path.concat(count);  // Concatenates string with number var "count"

/*
  // append a new value to path of new value
  String name = Firebase.pushInt(newRecord_path, count++); // 
  // handle error
  if (Firebase.failed()) {
      Serial.print("pushing " + newRecord_path + " failed:");
      Serial.println(Firebase.error());  
      return;
  }
  Serial.print("pushed: " + newRecord_path);
  Serial.println(name);
  delay(100);
*/
  // set new value for desired path in database, e.g. "/Humidity/Hum"
  Firebase.setInt(newRecord_path, sensor_value);
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting " + newRecord_path + " failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(100); 

}
