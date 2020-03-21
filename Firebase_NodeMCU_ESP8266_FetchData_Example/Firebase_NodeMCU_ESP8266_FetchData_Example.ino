#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <ctype.h>


#define WIFI_SSID "NETWORK_NAME_HERE"
#define WIFI_PASSWORD "NETWORK_PASSWORD_HERE"
#define FIREBASE_HOST "FIREBASEIO_LINK_HERE"
#define FIREBASE_AUTH "AUTH_KEY_HERE"


int count = 1; // Indicates how many values were updated in database
int serial_val_count = 1;
int hum_val = 50;
int pH_val = 3;
int temperature = 20;
int waterLevel_status = 0;

int numBytes;

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

void loop() {

  String path = "/Update/WL";
  int val = Firebase.getInt(path);

  Serial.print("Value: ");
  Serial.print(val);
  Serial.println(" ");

  delay(10000);
}
