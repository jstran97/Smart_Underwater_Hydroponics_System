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
#define WIFI_PASSWORD "WIFI_PASSWORD_HERE"
#define FIREBASE_HOST "LINK_HERE"
#define FIREBASE_AUTH "AUTH_HERE"

void setup() {
  Serial.begin(9600);

  // connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

int count = 1;
int hum_val = 50;
int pH_val = 3;
int temperature = 20;
String waterLevel_status = "Red";

void loop() {
  // set value for Humidity
  Firebase.setInt("/Update/Hum", hum_val);
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /Update/Hum failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(100); 

  // set value for pH
  Firebase.setInt("/Update/PH", pH_val);
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /Update/Hum failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(100); 

  // set value for Temperature
  Firebase.setInt("/Update/Temp", temperature);
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /Update/Hum failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(100); 
  
  // set string value for  Water Level
  Firebase.setString("Update/WL", waterLevel_status);
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /Update/WL failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(100);


  String hum_newRecord = "/Humidity/Humidity";  // Path to Humidity value in database
  hum_newRecord.concat(count);  // Concatenates string with number var "count"

  // append a new value to path /Humidity/Humidity
  String name = Firebase.pushInt(hum_newRecord, count++); // 
  // handle error
  if (Firebase.failed()) {
      Serial.print("pushing /Humidity/Humidity failed:");
      Serial.println(Firebase.error());  
      return;
  }
  Serial.print("pushed: /Humidity/Humidity/");
  Serial.println(name);
  delay(100);


  // set value for Humidity
  Firebase.setInt(hum_newRecord, hum_val);
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting " + hum_newRecord + " failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(100); 

  hum_val++;
  pH_val++;
  temperature++;

//  count++; // Increase number of values pushed to database
  
  delay(10000);

/*
  // append a new value to /Update
  String name = Firebase.pushInt("/Update/Hum", n++);
  // handle error
  if (Firebase.failed()) {
      Serial.print("pushing /Update failed:");
      Serial.println(Firebase.error());  
      return;
  }
  Serial.print("pushed: /Update/");
  Serial.println(name);
  delay(5000);

 
  // update value
  Firebase.setFloat("number", 43.0);
  // handle error 
  if (Firebase.failed()) {
      Serial.print("setting /number failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);

  // get value 
  Serial.print("number: ");
  Serial.println(Firebase.getFloat("number"));
  delay(1000);

  // remove value
  Firebase.remove("number");
  delay(1000);

  // set string value
  Firebase.setString("message", "hello world");
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /message failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);
  
  // set bool value
  Firebase.setBool("truth", false);
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /truth failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);
  */
}
