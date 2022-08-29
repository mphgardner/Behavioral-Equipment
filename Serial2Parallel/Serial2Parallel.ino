/*
 This script receives integer bytes from the serial port (UART) and converts
 these serial inputs to a parallel byte which is sent as an ouput on port A 
 (channels 22-29). A Strobe channel is used to indicate the byte has been written
 this is on channel 37 (PORTC = 1)

 The Serial2Pi_TEST script is a copy of this script but with an additional port read
 on PORTA to be sure the parallel out is working.

 This is currently written for a Mega in order to make parallel byte writing
 simple (PORT A and PORT C on the mega have all 8 channels in order)
 */


int next = 0;
int strobe = 3;
//int check = 19;

void setup() {
  // initialize serial:
  Serial.begin(115200);
  Serial.setTimeout(10000000000);

  //For seeing when an input arrives
  pinMode(LED_BUILTIN, OUTPUT);
  
  //sets pins 22-29 as outputs and then sets all port channels as low
  DDRA = B11111111;
  PORTA = 0;

  //
  delayMicroseconds(50);
  pinMode(strobe,OUTPUT);
  digitalWrite(strobe,HIGH);
  delayMicroseconds(50);
  digitalWrite(strobe,LOW);
  //pinMode(check,INPUT);
  Serial.println(0);
}

void loop() {
  // print the string when a newline arrives:
  delay(1);
  Serial.println("Waiting");
  while (Serial.available()>0) {
  
  //The following can be commented out for faster processing
    digitalWrite(LED_BUILTIN, HIGH);
    
    //Read the serial input
    int next = Serial.parseInt();

    //Print if desired (although this should use a different UART channel)
    Serial.println(next);
    PORTA = next;

    delayMicroseconds(10);
    //Turn on strobe
    digitalWrite(strobe,HIGH);

    //duration the strobe turns on
    delay(100);
    digitalWrite(strobe,LOW);

    //Clear the port
    PORTA = 0;
    
    //depending on the line ending, there may be additional bytes to read, this removes them:
    Serial.read();

    delayMicroseconds(10); //Takes 150 microseconds to reset the pin on Plexon

    //Turn off the LED
    digitalWrite(LED_BUILTIN, LOW);
 
  }
}
  
