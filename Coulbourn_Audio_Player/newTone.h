/*
newTone.h describes the newTone() and noNewTone() functions which are necessary for playing overlapping tones and melodies.
I adapted the code from the NewTone Library described below

--Also tacked on a list note-pitch #defines--
*/





// ---------------------------------------------------------------------------
// Created by Tim Eckel - teckel@leethost.com
//
// See "NewTone.h" for purpose, syntax, version history, links, and more.
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// NewTone Library - v1.0 - 01/20/2013
//
// AUTHOR/LICENSE:
// Created by Tim Eckel - teckel@leethost.com
//
// DISCLAIMER:
// This software is furnished "as is", without technical support, and with no 
// warranty, express or implied, as to its usefulness for any purpose.
//
// PURPOSE:
// Almost 1,300 bytes smaller code size than the Tone library. Faster execution
// time. Exclusive use of port registers for fastest and smallest code. Higher
// quality sound output than tone library. Plug-in replacement for Tone. Uses
// timer 1 which may free up conflicts with the tone library.
//
// SYNTAX:
//   NewTone( pin, frequency [, length ] ) - Play a note on pin at frequency in Hz.
//     Parameters:
//       * pin        - Pin speaker is wired to (other wire to ground, be sure to add an inline 100 ohm resistor).
//       * frequency  - Play the specified frequency indefinitely, turn off with noNewTone().
//       * length     - [optional] Set the length to play in milliseconds. (default: 0 [forever], range: 0 to 2^32-1)
//   noNewTone(pin) - Stop playing note (pin is optional, will always stop playing on pin that was last used).
//
// HISTORY:
// 01/20/2013 v1.0 - Initial release.
//
// ---------------------------------------------------------------------------
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58  
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define NOTE_High 8000
#define NOTE_High2 6000

#ifndef newTone_h
  #define newTone_h

  #if defined(ARDUINO) && ARDUINO >= 100
    #include <Arduino.h>
  #else
    #include <WProgram.h>
  #endif

  #if defined(__AVR_ATmega8__) || defined(__AVR_ATmega128__)
    #define TIMSK1 TIMSK
  #endif

  void newToneA(uint8_t pin, unsigned long frequency, unsigned long length = 0);
  void noNewToneA(uint8_t pin = 0);
#endif


//newToneA  newToneA  newToneA  newToneA  newToneA  newToneA  newToneA  newToneA  newToneA  newToneA  newToneA  newToneA  
unsigned long _nt_timeA;       // Time note should end.
uint8_t _pinMaskA = 0;         // Pin bitmask.
volatile uint8_t *_pinOutputA; // Output port register

void newToneA(uint8_t pin, unsigned long frequency,  unsigned long length) {
  uint8_t prescaler = _BV(CS10);                 // Try using prescaler 1 first.
  unsigned long top = F_CPU / frequency / 4 - 1; // Calculate the top.
  if (top > 65535) {                             // If not in the range for prescaler 1, use prescaler 256 (61 Hz and lower @ 16 MHz).
    prescaler = _BV(CS12);                       // Set the 256 prescaler bit.
    top = top / 256 - 1;                         // Calculate the top using prescaler 256.
  }
  
  if (length > 0) _nt_timeA = millis() + length; else _nt_timeA = 0xFFFFFFFF; // Set when the note should end, or play "forever".
  
  if (_pinMaskA == 0) {
    _pinMaskA = digitalPinToBitMask(pin);                    // Get the port register bitmask for the pin.
    _pinOutputA = portOutputRegister(digitalPinToPort(pin)); // Get the output port register for the pin.
    uint8_t *_pinMode = (uint8_t *) portModeRegister(digitalPinToPort(pin)); // Get the port mode register for the pin.
    *_pinMode |= _pinMaskA; // Set the pin to Output mode.
  }

  ICR1    = top;                     // Set the top.
  if (TCNT1 > top) TCNT1 = top;      // Counter over the top, put within range.
  TCCR1B  = _BV(WGM13)  | prescaler; // Set PWM, phase and frequency corrected (ICR1) and prescaler.
  TCCR1A  = _BV(COM1B0);
  TIMSK1 |= _BV(OCIE1A);             // Activate the timer interrupt.
}

void noNewToneA(uint8_t pin) {
  TIMSK1 &= ~_BV(OCIE1A);   // Remove the timer interrupt.
  TCCR1B  = _BV(CS11);      // Default clock prescaler of 8.
  TCCR1A  = _BV(WGM10);     // Set to defaults so PWM can work like normal (PWM, phase corrected, 8bit).
  *_pinOutputA &= ~_pinMaskA; // Set pin to LOW.
  _pinMaskA = 0; // Flag so we know note is no longer playing.
}

ISR(TIMER1_COMPA_vect) { // Timer interrupt vector.
  if (millis() >= _nt_timeA) noNewToneA(*_pinOutputA); // Check to see if it's time for the note to end.
  *_pinOutputA ^= _pinMaskA; // Toggle the pin state.
}
