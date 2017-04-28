/*
   MIDIUSB_test.ino

   Created: 4/6/2015 10:47:08 AM
   Author: gurbrinder grewal
   Modified by Arduino LLC (2015)
*/
//#define DEBUG false
#ifdef DEBUG
#endif

int zahler;
int potiwert[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int switchPort[] = {6, 7, 8, 9};
int i;

int sensor = A0;
int value = 0;
int smoothValue[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int quickSmooth;
bool quickActiv = false;

int prev;
int dive;
int x = 0;

int Final[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int FinalPrev[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

//int controlChange = 176; // MIDI Kanal 1
int controllerNummer = 21;



#include "MIDIUSB.h"

// First parameter is the event type (0x09 = note on, 0x08 = note off).
// Second parameter is note-on/note-off, combined with the channel.
// Channel can be anything between 0-15. Typically reported to the user as 1-16.
// Third parameter is the note number (48 = middle C).
// Fourth parameter is the velocity (64 = normal, 127 = fastest).

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}

void setup() {
  Serial.begin(115200);
  for (i = 0; i < 4; i++) { //changed by janek
    pinMode(i, OUTPUT);
  }
}

// First parameter is the event type (0x0B = control change).
// Second parameter is the event type, combined with the channel.
// Third parameter is the control number number (0-119).
// Fourth parameter is the control value (0-127).

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}

void loop() {
  // put your main code here, to run repeatedly:
  for (zahler = 0; zahler < 16; zahler++) {
    for (i = 0; i < 4; i++) {
      digitalWrite(switchPort[i], bitRead(zahler, i));
    }

    potiwert[zahler] = analogRead(0);

    value = analogRead(sensor);
#ifdef DEBUG

    Serial.print("    Raw:");
    Serial.print(value);
    Serial.print("\tzahler:    ");
    Serial.print( zahler, BIN);
#endif
    // Standart Value

    //value = trans(0,1024,0,127,value);

    smoothValue[zahler] = 0.6 * smoothValue[zahler] + 0.4 * value;

    //////////////////////////////////////////////////
    Final[zahler] = map(smoothValue[zahler], 0, 1023, 0, 127); // Finale Varieble
    if (Final[zahler] != FinalPrev[zahler]) {
#ifdef DEBUG
      Serial.print(" \tController Nummer:");
      Serial.print(controllerNummer + zahler);        //Ch21
      Serial.print("\tValue:");
      Serial.print(Final[zahler]);

      Serial.println();
#endif
      controlChange(0, (byte)controllerNummer + zahler, (byte)Final[zahler]);                    //changed by janek
      MidiUSB.flush();
    } else {
#ifdef DEBUG
      Serial.println("-");
      delay(100);
#endif
    }
    FinalPrev[zahler] = Final[zahler];
    //////////////////////////////////////////
    //delay(1);
  }
#ifdef DEBUG
  Serial.println();
  Serial.println("-----------------------");
  Serial.println();
  delay(100);
#endif
  delay(1);
  
}
