/*Voltage regulator into ditigal input_3s pulse output to both laser and LED  */

#include <Wire.h>
#include <Adafruit_MCP4725.h>
#include <math.h>

Adafruit_MCP4725 dac;

int out = 4;
int inputPin = 8;              //ON DAC_BOARD Coulbourn_Cue is DIO channels 6,7,8                    
int Val;         // variable for reading the pushbutton status
boolean flag = false;
unsigned long init_time = 0;
unsigned long ramp_up = 3000; // in milliseconds
unsigned long ramp_down = 3000; //in ms
int startV = 0;
uint32_t outputValue = 0;
double dur_temp = 0;

void setup() {
 Serial.begin(9600);
 Serial.println("go");
 pinMode(inputPin, INPUT_PULLUP);    // Set the pin3 as input
 pinMode(out, OUTPUT);
 digitalWrite(out, true);
  // For Adafruit MCP4725A1 the address is 0x62 (default) or 0x63 (ADDR pin tied to VCC)
  // For MCP4725A0 the address is 0x60 or 0x61
  // For MCP4725A2 the address is 0x64 or 0x65
  //Note that the DAC defaults to 12 bit
  dac.begin(0x62);
  dac.setVoltage(0, false);
 
}


void loop(){
  //for debugging:
  Serial.println(digitalRead(inputPin));
  if (!digitalRead(inputPin)) {
    flag = true;
    init_time = millis();
    
    while (!digitalRead(inputPin)) {

      dur_temp = constrain(millis() - init_time, 0, ramp_up);
      
      outputValue = uint32_t(map(dur_temp, 0, ramp_up, startV, 4095));
      Serial.println(outputValue);
      dac.setVoltage(outputValue, false);
      //Serial.println(outputValue);
      //analogWrite(outputPin1, outputValue); 
      
      delay(1);
    }

    dur_temp = 0;
    flag = false;
    init_time = millis();
    
    while (dur_temp < ramp_down) {

      dur_temp = millis() - init_time;
      
      outputValue = uint32_t(map(ramp_down - dur_temp, 0, ramp_down, startV, 4095));
      
      dac.setVoltage(outputValue, false);
      //Serial.println(outputValue);
      //analogWrite(outputPin1, outputValue); 
      
      delay(1);
    }
    outputValue = uint32_t(0);
    dac.setVoltage(outputValue, false);
  }
   
  
} 
