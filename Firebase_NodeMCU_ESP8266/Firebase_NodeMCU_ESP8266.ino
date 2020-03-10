#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <ctype.h>

#define WIFI_SSID "will_hs"
#define WIFI_PASSWORD "lagoon2020"
#define FIREBASE_HOST "lagoon-9de60.firebaseio.com"
#define FIREBASE_AUTH "aWjHUYVg9d7ZJEqKVW8tgaRHf4WlRt90FxFgze3H"

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

  if (Serial.available()) {
    numBytes = Serial.available(); // Number of bytes available for reading
                                   // from serial port
    char rx_String[numBytes];
    // Store each byte read into char array
    for (int i = 0; i < numBytes; i++) {
      delay(1);
      rx_String[i] = Serial.read(); 
    }
    
    float rx_Data[4]; // New array to store strings converted to numbers
    int float_index = 0;
    //String temp;

    char *ch_ptr = NULL;
    char *temp_ch_arr[5];
    byte index = 0;
  
    Serial.println("Before rx_String prints");
    Serial.println(rx_String);


    Serial.println("Before 1st ptr = strtok()");
    ch_ptr = strtok(rx_String, ","); // Look for decimal point (.) delimiter
    Serial.println(ch_ptr);

    rx_Data[float_index] = atof(ch_ptr); // Convert alphabet to float value (including 
                                   // decimal point)
    float_index++;
    Serial.print("Float #: ");
    Serial.print(rx_Data[0]);
    Serial.println(" ");


     while (float_index < 3) {
      delay(1);
      ch_ptr = strtok(NULL, ","); // Look for next start of token with comma (,)
      //temp_ch_arr[index] = ch_ptr; // Store char pointer to start of token found
      rx_Data[float_index] = atof(ch_ptr); // Convert alphabet to float value (including 
                                     // decimal point)
      Serial.print("Point: ");
      Serial.print(atof(ch_ptr) );
      Serial.println(" ");
      
      //Serial.print("Float #: ");
      //Serial.print(rx_Data[float_index]); 
      //Serial.println(" "); 

      float_index++;

      //index++; 
      
    }



    // Update humidity value
    //updateDBValues("/Update/Hum", (int)rx_Data[1]);
    updateDBValues("/Update/Hum", rx_Data[0]);
  
    // Update pH value
    //updateDBValues("/Update/PH", (int)rx_Data[0]);
    updateDBValues("/Update/PH", rx_Data[1]);
    
    // Update temperature value
    //updateDBValues("/Update/Temp", (int)rx_Data[2]);
    updateDBValues("/Update/Temp", rx_Data[2]);



    count++; // Increase number of values pushed to database

    delay(1000);
  }

  delay(1000);
  //delay(10000); // 10 sec delay 
  
}


// Update the date & time, humidity, pH, and temperature values in real-time database
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

  // set new value for desired path in database, e.g. "/Humidity/Hum"
  Firebase.setFloat(newRecord_path, sensor_value);
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting " + newRecord_path + " failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(100); 

}
