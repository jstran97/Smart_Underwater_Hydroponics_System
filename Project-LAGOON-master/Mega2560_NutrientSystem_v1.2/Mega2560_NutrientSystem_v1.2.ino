#include "Adafruit_HTU21DF.h"
#include <Wire.h>

const unsigned long ONE_SECOND                  = 1000UL;
const unsigned long ONE_MINUTE                  = 60UL * ONE_SECOND;
const unsigned long FIVE_MINUTES                = 5UL * 60UL * ONE_SECOND;
const unsigned long FIFTEEN_MINUTES             = 3UL * FIVE_MINUTES;
const int analogInPin = A6;  // Analog pin for pH sensor
const int res_pin = A0; // Analog pin for water level sensor
int count;
float temp_float = 5.3;
char tx_Str[30];
int pump_con_val; 
unsigned int avgValue; 
int buf[10];
int temporary;

int res_val = 0;


//Create an instance of the object
Adafruit_HTU21DF myHumidity;

void setup()
{
  Serial.begin(115200);
  Serial1.begin(115200);
  
  Serial.println("HTU21D Example!");
  Serial.println(myHumidity.begin());
  while(!myHumidity.begin()){
    Serial.println("Not Connected...");
  }
  Serial.println("Humidity and Temperature Sensor Connected");
}


void loop()
{
  unsigned long millis_FlushStart = millis();

  int humd = myHumidity.readHumidity(); // Read current humidity
  int temp = myHumidity.readTemperature(); // Read current temperature (in C)
  int water_level = getWaterLevelStatus(); // Get water level status 
                                           // (1 for good, 0 for bad)
  
  float pH_val = obtain_pHValue(); // Read pH measurement from sensor

  // Places all sensor values into a string with a comma (,) delimiter
  sprintf(tx_Str, "%03d.%01d,%03d,%03d,%01d", (int)pH_val, 
    (int)(pH_val*100)%100, humd, temp, water_level);
  
  Serial.println(tx_Str);

  Serial1.write(tx_Str);

  Serial.flush();
  unsigned long millis_FlushStop = millis();
  Serial.print("With flush, void loop() is done in: ");
  Serial.print(millis_FlushStop - millis_FlushStart);
  Serial.println(" milliseconds");
 
  delay(1000);
  //delay(10000); // Delay for 10 seconds
  //delay(FIFTEEN_MINUTES); // Delay for 15 minutes
}


/*
 * Name: Obtain pH Value
 * Date Last Updated: April 10, 2020
 * Description: Measure the pH value using the pH electrode sensor probe
 */
float obtain_pHValue() {
   /*****************************************************************
    *    Title: How to use a pH Sensor with Arduino
    *    Author: David Cervantes Caballero
    *    Date: March 10, 2017
    *    Code version: N/A
    *    Availability: https://scidle.com/how-to-use-a-ph-sensor-with-arduino/
    *
    ******************************************************************/
   // Obtain 10 samples of analog value from pH sensor
   for(int i = 0; i < 10 ; i++) 
   { 
      buf[i]=analogRead(analogInPin);
      delay(10);
   }
  
   // Sort from highest to lowest val
   for(int i=0;i<9;i++)
   {
      for(int j = i+1; j < 10; j++)
      {
         // Swap positions if previous val is smaller than next val
         if(buf[i]>buf[j])
         {
            temporary=buf[i];
            buf[i]=buf[j];
            buf[j]=temporary;
         }
      }
   }
  
   avgValue=0;
  
   // Calcuate mean of 6 lowest values in array
   for(int i = 2; i < 8; i++)
      avgValue+=buf[i];
   float pHVol=(float)avgValue*5.0/1024/6; // Avg voltage read from analog pin
   //float phValue = -5.70 * pHVol + 21.34; // y = (-5.70)x + 21.338
   float pHValue = -4.95 * pHVol + 19.41; // y = (-4.9451)x + 19.412
   Serial.print("sensor = ");
   Serial.println(pHValue);
   
   delay(20);

   return pHValue;
}


/*
 * Name: Get Water Level Status
 * Date Last Updated: May 1, 2020
 * Description: Determine water level detected and return a status indicating 
 *              whether or not the water level is good
 */
int getWaterLevelStatus() {
  res_val = analogRead(res_pin); //Read data from analog pin and store it to resval variable

  // if resolution is greater or equal to 300 (accounting for water condensation),
  // then give a red indicator (bad sign)
  if (res_val >= 300) {
    return 0; // Bad/red indicator
  }
  else {
    return 1; // Good/green indicator
  }

}
