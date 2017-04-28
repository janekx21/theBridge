
int sensor = A0;
int value = 0;
int prev;
int dive;
int x = 0;
int smoothValue = 0;
int quickSmooth;
bool one;
bool quickActiv = false;
int Final;
int FinalPrev;

int controlChange = 176; // MIDI Kanal 1
int controllerNummer = 21;

bool pressA = false;
bool pressB = false;

bool stateA =false;
bool stateB = false;

void setup() {
  pinMode(3, INPUT);
  pinMode(2,INPUT);
  Serial.begin(31250);
}

void loop() {
  // put your main code here, to run repeatedly:


  value = analogRead(sensor);                       // Standart Value
  //value = trans(0,1024,0,127,value);
  smoothValue = 0.6 * smoothValue + 0.4 * value;    // Saubere aber verzögerte Value
  dive = abs(value - prev);                         // aUnterschied frame davor
  if(dive <= 5 && !quickActiv){                     // wenn die diverrez anders ist
      quickSmooth = 0.6 * smoothValue + 0.4 * value;
  }
  else if(dive <= 5 && quickActiv){
    quickSmooth = value;
    x++;
  }else{
    quickSmooth = value;
    quickActiv = true;  
  }
  if(x >= 10){
    x = 0;
    quickActiv = false;  
  }
  Final = map(quickSmooth,0,1023,0,127);            // Finale Varieble
  if(Final != FinalPrev){
    Serial.write(controlChange);                    //ControllChange ch1
    Serial.write(controllerNummer);                 //Ch21
    Serial.write(Final);                             //value
  }
  prev = value;                           //Pev Reset
  FinalPrev = Final;
  if(!pressA){                              //Buttons 
    stateA = !digitalRead(2);
    if(stateA){
      controllerNummer++;
    }
    pressA = true;
  }
  if(digitalRead(2)){
    pressA = false;
  }

    if(!pressB){
    stateB = !digitalRead(3);
    if(stateB){
      controllerNummer--;
    }
    pressB = true;
  }
  if(digitalRead(3)){
    pressB = false;
  }

  delay(10);
  
}
/*
int trans(float start1,float end1, float start2, float end2, int pos){
  return (end1 - start1  - pos)/(end1 - start1) * (end2 - start2);
}
*/
