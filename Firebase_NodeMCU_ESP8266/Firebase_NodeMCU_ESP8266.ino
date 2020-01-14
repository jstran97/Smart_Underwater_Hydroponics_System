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

#define WIFI_SSID "NETWORK_NAME_HERE"
#define WIFI_PASSWORD "NETWORK_PASSWORD_HERE"
#define FIREBASE_HOST "FIREBASEIO_LINK_HERE"
#define FIREBASE_AUTH "AUTH_KEY_HERE"

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
int hum_val = 50;
int pH_val = 3;
int temperature = 20;
String waterLevel_status = "Red";

void loop() {
  // Update humidity value
  updateDBValues("/Update/Hum", 89);

  // Update pH value
  updateDBValues("/Update/PH", 4);
  
  // Update temperature value
  updateDBValues("/Update/Temp", 54);

  count++; // Increase number of values pushed to database

  //delay(10000); // 10 sec delay
  delay(300000); // 5 min delay 
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
    newRecord_path = "/Humidity/Humidity";
  }
  else if (db_path == "/Update/PH") {
    newRecord_path = "/PH/Ph";
  }
  else if (db_path == "/Update/Temp") {
    newRecord_path = "/Temperature/Temp";
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
