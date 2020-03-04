#include "Adafruit_HTU21DF.h"
#include <Wire.h>

int count;
//char rand_str[2];
char str[] = "yo";
//String rand_str = "";
//String new_str = "";
//int temp_int = 123;
float temp_float = 7.4;
char test[30];

//Create an instance of the object
Adafruit_HTU21DF myHumidity;
void setup()
{
  Serial.begin(9600);

  Serial1.begin(9600);
  
  Serial.println("HTU21D Example!");
  Serial.println(myHumidity.begin());
  while(!myHumidity.begin()){
    Serial.println("Not Connected...");
  }
  Serial.println("Humidity and Temperature Sensor Connected");
}


void loop()
{
  int humd = myHumidity.readHumidity();
  int temp = myHumidity.readTemperature();

  //Serial.println("Time:");
  //Serial.println(millis());

  Serial.println("Before sprintf()");

  Serial.println(" Humidity:");
  Serial.println(humd, 1);
  //Serial.println("%");
  //Serial1.write(humd);
  //delay(1000);
  
  Serial.println(" Temperature:");
  Serial.println(temp, 1);
  //Serial.println("C");
  //Serial1.write(temp);
  //delay(1000);
  
  
  // sprintf() with 3 ints (does NOT support float, double, and long long) WORKS
  //sprintf(test, "%03d.%01d,%03d,%03d", (int)temp_float, (int)(temp_float*100)%100, 47, 96);
  sprintf(test, "%03d.%01d,%03d,%03d", (int)temp_float, (int)(temp_float*100)%100, humd, temp);
  Serial.println("After sprintf() & Before Serial.println(test)");
  Serial.println(test);

  Serial.println("Before Serial1.write(test)");
  Serial1.write(test);
  Serial.println("After Serial1.write(test)");


  delay(1000);
  //delay(10000);
}
