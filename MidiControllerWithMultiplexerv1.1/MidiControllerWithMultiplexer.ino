int zahler;
int potiwert[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int switchPort[] = {6,7,8,9};
int i;

int sensor = A0;
int value = 0;
int smoothValue[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int quickSmooth;
bool quickActiv = false;

int prev;
int dive;
int x = 0;

int Final[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int FinalPrev[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

int controlChange = 176; // MIDI Kanal 1
int controllerNummer = 21;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(31250);
  for(i = 0;i<4;i++){
    pinMode(i,OUTPUT);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  for(zahler = 0;zahler < 16; zahler++){
    for(i = 0;i<4;i++){
      digitalWrite(switchPort[i],bitRead(zahler,i));
    }
    
    potiwert[zahler] = analogRead(0);
/*    Serial.print(zahler);
    Serial.print (":");
    Serial.print(potiwert[zahler]);
    Serial.print ("\t");
    delay(1);
    */
//////////////////////////////////////
      value = analogRead(sensor);     
      // Standart Value
      
  //value = trans(0,1024,0,127,value);
  
  smoothValue[zahler] = 0.6 * smoothValue[zahler] + 0.4 * value;
  /*// Saubere aber verzögerte Value
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
  */
  //////////////////////////////////////////////////
  Final[zahler] = map(smoothValue[zahler],0,1023,0,127);            // Finale Varieble
  if(Final[zahler] != FinalPrev[zahler]){
    Serial.write(controlChange);                    //ControllChange ch1
    Serial.write(controllerNummer + zahler);                 //Ch21
    Serial.write(Final[zahler]);                             //value
    
    //Serial.println("zahl:" + String(zahler) + "\t final" +  String(Final[zahler]));
  }
 // prev = value;                           //Pev Reset
  FinalPrev[zahler] = Final[zahler];
  //////////////////////////////////////////
  }
//  Serial.println("e");

  delay(1);
}
