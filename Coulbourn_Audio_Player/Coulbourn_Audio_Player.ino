/************
This script is for playing auditory stimuli on an Arduino using the Sparkfun XXXX Amplifier. 
4 bit inputs are received from Coulbourn via a Coulbourn to Arduino shield. The different word inputs
are programmable for various auditory stimuli: tones, chords, sirens, amplitude modulated tones, white noise, and clicks

**************/


//SIREN
unsigned int fmfreq[60];
int freqcountmax = 60;
int freqcount = 1;
int freqbase = 300;
int sirenfreq = 10;
    
//SINVAL is a list of 60 elements which are sin values from zero to 2 pi multiplied by a thousand
 unsigned int SINVAL[60] = {1000, 1105, 1208, 1309, 1407, 1500, 1588, 1669, 1743, 1809, 1866, 1913, 1951, 1978, 1995, 2000, 
                    1995, 1978, 1951, 1913, 1866, 1809, 1743, 1669, 1588, 1500, 1407, 1309, 1208, 1105, 1000,
                       895, 792, 691, 593, 500, 412, 331, 257, 191, 134, 86, 49, 22, 5, 0,
                        5, 22, 49, 86, 134, 191, 257, 331, 412, 500, 593, 691, 792, 895}; 




//WHITE NOISE
int freqmin = 50;
int freqmax = 8000;

//CLICKS

int ISI = 200; //interstimulus interval in ms

//AMPLITUDE MOD TONE
int T_ISI = 50; //interstimulus interval in ms
int NoT_ISI = 50; //Time stimulus is off in ms
boolean AM_Flag = true; //keeps track of whether tone is on or off
int AMfreq = 8000;


boolean reset = false;

byte cue_input = 0;
byte cue_input_new = 0;
int digins[] = {8,7,6,4};

int Ch1 = 9;
int Ch2 = 11;

unsigned long TimerWN = millis();
unsigned long TimerSiren = millis();
unsigned long TimerClick = millis();
unsigned long TimerAM = millis();

boolean start_stim = true;

#include "newTone.h"
//#include <toneAC.h>


void setup() {
  Serial.begin(9600);
  pinMode(A0,OUTPUT);
  pinMode(A1,OUTPUT);
  pinMode(A2,OUTPUT);
  digitalWrite(A0, LOW);
  digitalWrite(A1, LOW);
  digitalWrite(A2, LOW);
 for (int i = 0; i < 4; i++){ 
 pinMode(digins[i], INPUT_PULLUP);

 //this sets the pull-up resistors high
 //digitalWrite(digins[i], HIGH);
 }
  cue_input = 0;

  for(int i = 1; i <= 60; i++){  
      
   fmfreq[i] = SINVAL[i];
  } 
    
//    for(int i2 = 1; i2 = 100; i2++){  
//      
//     fmfreq[i2] = freqmid + int(freqrange*sin(pi/(double(i2))));
//    }
//    

}


void loop() {
 // Serial.println(!digitalRead(11));
  cue_input = 0;
  for (int i = 0; i < 4; i++){
    cue_input |= !digitalRead(digins[i]) << i;
  }
  //Serial.println(cue_input);
  //if any channels were on, recheck cue to make sure none were missed
  if (cue_input > 0) {
    for (int i = 0; i < 4; i++){
    cue_input |= !digitalRead(digins[i]) << i;
    }
  
    if (cue_input > 0 && !reset) {
      Serial.println(cue_input);
      reset = true;
      }

    switch (cue_input) {
       //reset if there are no cue inputs

        case 1:
          puretone(3000);
        break;
        
        case 2:
          chord(1100, 330);
        break;
        
        case 3:
          whitenoise();
        break;
        case 4: 
          //freqcount = siren(TimerSiren,freqbase,fmfreq,freqcount,freqcountmax);
          siren();
        break;
        case 5: 
         clicks();
        break;
        case 6: 
         //am_tone(AMfreq, T_ISI, NoT_ISI);
         am_tone(AMfreq);
        break;
        case 7: 
  
        break;
        case 8:
        break;
        case 9:
        break;
        case 10:

      break;    
      case 11:

        break;
       case 12: 

        break; 
        case 13:

         break; 
       case 14:

         break; 
         case 15:
          break;
    }
    start_stim = false;
  }
  
  else if (reset) {
          //toneAC(0);
          noTone(Ch1);
          noNewToneA(Ch2);
          reset = false;
          AM_Flag = true;
          start_stim = true;
  }
} 

void puretone(int freq) {
  //toneAC(freq);
  if (start_stim) {
  tone(Ch1,freq);
  }
  //tone(Ch2,freq);
}

void chord(int freq1, int freq2) {
  if (start_stim) {
  tone(Ch1,freq1);
  newToneA(Ch2,freq2);
  }
}

void whitenoise() {
  
    tone(Ch1,random(freqmin, freqmax));
    //newToneA(Ch2,random(freqmin, freqmax));
    //tone(Ch2,curr);

}
//int siren(unsigned long TimerSiren, int freqbase, unsigned int fmfreq[60], int freqcountmax)  {
void siren() {
    if ((millis() - TimerAM) >= sirenfreq){
    if(freqcount == freqcountmax){
      freqcount = 1;}
        
    freqcount = freqcount + 1;
        
    tone(Ch1,freqbase + fmfreq[freqcount]);
    //newToneA(Ch2,freqbase + fmfreq[freqcount]);
    TimerAM = millis();
    }
  
}

//void am_tone(int freq, int T_ISI, int NoT_ISI){//, boolean AM_Flag) {
void am_tone(int freq)  {
  if (AM_Flag)
    if ((millis() - TimerAM) >= NoT_ISI){
      tone(Ch1, freq);
      //newtone(Ch2, freq);
      TimerAM = millis();
      AM_Flag = false;
    }
  if (!AM_Flag) {
    Serial.println('off');
    if ((millis() - TimerAM) >= T_ISI){
      TimerAM = millis();
      AM_Flag = true;
      noTone(Ch1);
      //noNewTone(Ch2);
         
    }
  } 
 
}  
       
void clicks() {
  if (millis() - TimerClick >= ISI){
       TimerClick = millis(); 
       while (millis() - TimerClick < 1) {

        //toneAC(8000,10,1);
        //toneAC(10000,10,1);
        //toneAC(12000,10,1);  
        tone(Ch1,10000);
        newToneA(Ch2,1000);
        tone(Ch1,12000); 
        newToneA(Ch2,12000); 
       //}
       noTone(Ch1);
       noNewToneA(Ch2);
       
       }
    } 
}
