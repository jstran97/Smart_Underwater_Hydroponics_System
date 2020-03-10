#include <ctype.h>

int data; //Initialized variable to store recieved data
//char* rx_String[20];

int numBytes;


void setup() {
  // Serial communication begins at 9600 baud
  Serial.begin(9600);
}

void loop() {
  
  //data = Serial.read(); //Read the serial data and store it
  //Serial.println(data);
  

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
    byte float_index = 0;
    //String temp;

    char *ch_ptr = NULL;
    char *temp_ch_arr[5];
    byte index = 0;
    int temp[3];
  
    //Serial.println("Before rx_String prints");
    //Serial.println(rx_String);

 

    
    //Serial.println("Before 1st ptr = strtok()");
    ch_ptr = strtok(rx_String, ","); // Look for decimal point (,) delimiter
    Serial.println(ch_ptr);
    temp_ch_arr[index] = ch_ptr; // Store char pointer to start of token found

    rx_Data[float_index] = atof(ch_ptr); // Convert alphabet to float value (including 
                                   // decimal point)
    float_index++;
    Serial.print("Float #: ");
    Serial.print(rx_Data[0]);
    Serial.println(" ");

/*
    int temp_count = 0;

    if (

    temp_count++;
*/

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
  


    
/* CHAR ARRAY METHOD (for DATABASE)
    // Store contents of broken-up string into a new char array
    for (int i = 0; i < index; i++) {
      int len = strlen(temp_ch_arr[i]);
      char ch_Data[len]; 

      for (int j = 0; j < len; j++) {
        ch_Data[j] = *temp_ch_arr[i];
        *temp_ch_arr[i]++;
      }

      // Print strings
      Serial.print("Strings: ");
      Serial.print(ch_Data);
      Serial.println(" ");
    }
*/

   /* for(int i = 0; i < (sizeof(rx_Data)-1); i++){
      Serial.print("rx_Data: ");
      Serial.print(rx_Data[i]);
      Serial.println(" ");
    }*/
  

    //Serial.println("Before rx_String prints");
    //Serial.println(rx_String);
    //Serial.println("After rx_String prints");
  }

  delay(1000);


















  

  /*
    rx_str = Serial.readString(); // Read the incoming data as string
    isMessageReady = true;
    Serial.println(rx_str);
    
    //rx_ch_arr = Serial.read(); // Read the incoming data as string
    //Serial.println(rx_ch_arr);
    delay(1000);
  }
  */

  
  /*
    rx_str = Serial.readString(); // Read the incoming data as string
    isMessageReady = true;
    Serial.println(rx_str);
    
    //rx_ch_arr = Serial.read(); // Read the incoming data as string
    //Serial.println(rx_ch_arr);
    delay(1000);
  }
  */
/*
  if (Serial.available()) {
    numBytes = Serial.available(); // Number of bytes available for reading
                                   // from serial port
    char rx_String[numBytes];
    // Store each byte read into char array
    for (int i = 0; i < numBytes; i++) {
      delay(1);
      rx_String[i] = Serial.read(); 
    }
    
    float rx_Data[6]; // New array to store strings converted to numbers
    int counter = 0;
    int index = 0;
    String temp;

    Serial.println("Before rx_String prints");
    Serial.println(rx_String);
    
    for(int i = 0; i < sizeof(rx_String)-1; i++) {
       delay(1000);
       if(rx_String[i] == ','){ // Check if current char is a comma
           temp = String(rx_String).substring(index, i-1); // Create a substring from the char 
                                                           // array using the comma as the cutting point
           //rx_Data[counter] = temp.toFloat(); // Convert string to float
           index = i+1; // Set beginning of next substring
           counter++; // Increment counter for next float value to be stored

           Serial.println("rx_Data: ");
           Serial.println(rx_Data[counter]);
           
            Serial.println(temp);
       }
      
       
    }

    for(int i = 0; i < (sizeof(rx_Data)-1); i++){
      Serial.print("rx_Data: ");
      Serial.print(rx_Data[i]);
      Serial.println(" ");
    }
  

    //Serial.println("Before rx_String prints");
    //Serial.println(rx_String);
    //Serial.println("After rx_String prints");
  }

  delay(1000);
*/
  
/*
  if(Serial.available()){
    rx_String[0] = '\0'; // Reset cstring
    String strArr[2]; //Set the size of the array to equal the number of values you will be receiveing.
    char* rx_Ch; // Empty char

    //strcpy(rx_String, rx_Ch);
    
    //Keep looping until there is something in the buffer.
    while (Serial.available()) {
      //Delay to allow byte to arrive in input buffer.
      delay(2);
      
      //Read a single character from the buffer.
      rx_Ch = Serial.read();
      
      //Append that single character to a string.
      //rx_String += rx_Ch;
      strcat(rx_String, *rx_Ch);

      Serial.println(rx_String);
    }
*/
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
