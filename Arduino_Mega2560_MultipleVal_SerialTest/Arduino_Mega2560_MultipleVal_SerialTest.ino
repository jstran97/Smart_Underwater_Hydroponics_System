int count;
//char rand_str[2];
//char str[] = "yo";
String rand_str = "";
String new_str = "";

void setup() {
  //Serial S Begin at 9600 Baud
  Serial1.begin(9600);

}

void loop() {
  //Write '123' to Serial
  rand_str = "wtf";
  new_str = rand_str + ',' + 123 + ','; // string concatentation
  Serial1.write(new_str); // write string to serial
  //Serial1.write(rand_str);
  //Serial1.write(","); // add delimiter to string (to separate values in serial)
  //Serial1.write(123); // write number to serial
  delay(1000);

  // FAILED METHOD
  new_str = rand_str + String(',') + String(123) + String(','); // string concatentation

  /*
  // put your main code here, to run repeatedly:
  if(Serial.available()){
    String rxString = "";
    String strArr[2]; //Set the size of the array to equal the number of values you will be receiveing.
    
    //Keep looping until there is something in the buffer.
    while (Serial.available()) {
      //Delay to allow byte to arrive in input buffer.
      delay(2);
      //Read a single character from the buffer.
      char ch = Serial.read();
      //Append that single character to a string.
      rxString+= ch;
    }
    
    int stringStart = 0;
    int arrayIndex = 0;
    for (int i=0; i < rxString.length(); i++){
      //Get character and check if it's our "special" character.
      if(rxString.charAt(i) == ','){
        //Clear previous values from array.
        strArr[arrayIndex] = "";
        //Save substring into array.
        strArr[arrayIndex] = rxString.substring(stringStart, i);
        //Set new string starting point.
        stringStart = (i+1);
        arrayIndex++;
      }
    }
    
    //Put values from the array into the variables.
    String value1 = strArr[0];
    String value2 = strArr[1];
    //Convert string to int if you need it.
    int intValue1 = value1.toInt();
  }
  */
}

// Author: RamjetX
// Link: https://forum.arduino.cc/index.php?topic=89143.0
void writeString(String stringData) { // Used to serially push out a String with Serial.write()

  for (int i = 0; i < stringData.length(); i++)
  {
    Serial.write(stringData[i]);   // Push each char 1 by 1 on each loop pass
  }

}// end writeString
