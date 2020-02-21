int data; //Initialized variable to store recieved data
char rx_String[20];

void setup() {
  // Serial communication begins at 9600 baud
  Serial.begin(9600);
}

void loop() {
  //data = Serial.read(); //Read the serial data and store it
  //Serial.println(data);
  /*
    rx_str = Serial.readString(); // Read the incoming data as string
    isMessageReady = true;
    Serial.println(rx_str);
    
    //rx_ch_arr = Serial.read(); // Read the incoming data as string
    //Serial.println(rx_ch_arr);
    delay(1000);
  }
  */


  if(Serial.available()){
    rx_String = '\0'; // Reset cstring
    String strArr[2]; //Set the size of the array to equal the number of values you will be receiveing.
    
    //Keep looping until there is something in the buffer.
    while (Serial.available()) {
      //Delay to allow byte to arrive in input buffer.
      delay(2);
      //Read a single character from the buffer.
      char ch = Serial.read();
      //Append that single character to a string.
      rx_String += ch;

      Serial.println(rx_String);
    }

/*
  if(Serial.available()){
    String rxString = ""; // Empty string
    String strArr[2]; //Set the size of the array to equal the number of values you will be receiveing.
    
    //Keep looping until there is something in the buffer.
    while (Serial.available()) {
      //Delay to allow byte to arrive in input buffer.
      delay(2);
      //Read a single character from the buffer.
      char ch = Serial.read();
      //Append that single character to a string.
      rxString += ch;

      Serial.println(rxString);
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
        //Set new string starting point.1
        stringStart = (i+1);
        arrayIndex++;
      }
    }
    //Put values from the array into the variables.
    String value1 = strArr[0];
    String value2 = strArr[1];
    //Convert string to int if you need it.
    int intValue1 = value1.toInt();

    Serial.println("value1 = " + intValue1);
    //Serial.println("value2 = " + value2);
  }
*/


}
