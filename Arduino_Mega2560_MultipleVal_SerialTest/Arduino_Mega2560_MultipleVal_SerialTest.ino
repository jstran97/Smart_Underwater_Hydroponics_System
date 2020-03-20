int count;
//char rand_str[2];
char str[] = "yo";
//String rand_str = "";
//String new_str = "";
int temp_int = 123;
float temp_float = 123.4;



char test[30];

void setup() {
  //Serial S Begin at 9600 Baud
  Serial1.begin(9600);
  Serial.begin(9600);
}

void loop() {   


  
  //Write '123' to Serial
  //rand_str = "wtf";
  //new_str = rand_str + ',' + 123 + ','; // string concatentation
  //Serial1.write(new_str); // write string to serial
  //Serial1.write(rand_str);
  //Serial1.write(","); // add delimiter to string (to separate values in serial)
  //Serial1.write(123); // write number to serial

  // WORKING METHOD
  //Serial1.write(str);

  Serial.println("Before sprintf()");
  Serial.println(temp_float);
  Serial.println(47);
  Serial.println(96);

  // sprintf() with 3 ints (does NOT support float, double, and long long) WORKS
  sprintf(test, "%03d.%01d,%03d,%03d,%01d", (int)temp_float, (int)(temp_float*100)%100, 47, 96, 0);
  Serial.println("After sprintf() & Before Serial.println(test)");
  Serial.println(test);

  Serial.println("Before Serial1.write(test)");
  Serial1.write(test);
  Serial.println("After Serial1.write(test)");
  delay(10000);

  // FAILED METHOD
  //new_str = rand_str + String(',') + String(123) + String(','); // string concatentation

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
