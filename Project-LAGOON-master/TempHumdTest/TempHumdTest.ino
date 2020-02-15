#include "Adafruit_HTU21DF.h"
#include <Wire.h>

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

  Serial.println(" Humidity:");
  Serial.println(humd, 1);
  //Serial.println("%");
  Serial1.write(humd);
  delay(1000);
  
  Serial.println(" Temperature:");
  Serial.println(temp, 1);
  //Serial.println("C");
  Serial1.write(temp);
  //delay(1000);
  
  
  delay(10000);
}
