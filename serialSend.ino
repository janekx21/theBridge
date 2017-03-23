#define BUTA_PIN 2
#define BUTB_PIN 3

char x = 0;
char y = 'V';
char z = 'V';

int ax=512;
int oax = 512;

int ay=512;
int oay=512;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(BUTA_PIN,INPUT);
  pinMode(BUTB_PIN,INPUT);
}

void loop() {
  z = 0x00;

  bitWrite(z,0,digitalRead(BUTA_PIN));
  bitWrite(z,1,digitalRead(BUTB_PIN));
  
  ax = analogRead(A0)*.6 + oax*.4;
  ay = analogRead(A1)*.6 + oay*.4;
  // put your main code here, to run repeatedly:
  x = map(ax,0,1024,0,127);
  y = map(ay,0,1024,0,127);
  Serial.print(x);//x joystick
  Serial.print(y);//y joystick
  Serial.print(z);//8 buttons
  Serial.println("");
  oax = ax;
  oay = ay;
  delay(50);
}
