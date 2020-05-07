int res_val = 0;  // Resolution value of water level sensor
int res_pin = A5; // Analog pin for water level sensor
  
void setup() { 
 
  // start the serial console
  Serial.begin(9600);
} 
  
void loop() { 
   
  res_val = analogRead(res_pin); //Read data from analog pin and store it to resval variable
/*   
  if (res_val<=100){ 
    Serial.println("Water Level: Empty"); 
  } 
  else if (res_val>100 && res_val<=300){ 
    Serial.println("Water Level: Low"); 
  } 
  else if (res_val>300 && res_val<=330){ 
    Serial.println("Water Level: Medium"); 
  }
  else if (res_val>330){ 
    Serial.println("Water Level: High"); 
  }
*/
  if (res_val >= 300) {
    water_level = 0; // Bad/red indicator
  }

  
  delay(1000); 
}
