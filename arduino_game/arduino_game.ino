
#include <Servo.h> 
 
Servo myServoX;
Servo myServoY;

int ServoXPin = 4;
int ServoYPin = 2;

int ServoXHomePos =103; //small
int ServoYHomePos =134; //big

int ServoXPos =103; //small
int ServoYPos =135; //big

int GoHomePin    = 7;
int GoHomeButton = 0;


int XAxlePin     = 3; //A0
int YAxlePin     = 2; //A1

int XAxleValue = 0;
int YAxleValue = 0;

int Direction = 0;

int range = 12;               // output range of X or Y movement
int center = range/2;         // resting position value
int threshold = range/4;      // resting threshold

void setup() 
{ 
  myServoX.attach(ServoXPin);
  myServoY.attach(ServoYPin);
  
  ServoXPos = ServoXHomePos;
  ServoYPos = ServoYHomePos;
  
  myServoX.write(ServoXPos);
  myServoY.write(ServoYPos);

  pinMode(GoHomePin, INPUT);     
  // turn on the pull-up resistor for the SEL line (see http://arduino.cc/en/Tutorial/DigitalPins)
  digitalWrite(GoHomePin,HIGH);

  Serial.begin(9600);
} 
 
 
void loop() 
{ 
  GoHomeButton = digitalRead(GoHomePin);

  XAxleValue = readAxis(XAxlePin);
  YAxleValue = readAxis(YAxlePin);

  Serial.print(GoHomeButton); 
  Serial.print(" - ");
  Serial.print(XAxleValue,DEC);
  Serial.print(" - ");
  Serial.println(YAxleValue,DEC);
  
  if (XAxleValue>0) { ServoXPos++; myServoX.write(ServoXPos); delay(100*(7-XAxleValue)); }
  if (XAxleValue<0) { ServoXPos--; myServoX.write(ServoXPos); delay(100*(7+XAxleValue)); }

  if (YAxleValue>0) { ServoYPos++; myServoY.write(ServoYPos); delay(100*(7-YAxleValue)); }
  if (YAxleValue<0) { ServoYPos--; myServoY.write(ServoYPos); delay(100*(7+YAxleValue)); }
  
  if (GoHomePin == 0)  {
        ServoXPos = ServoXHomePos;
        ServoYPos = ServoYHomePos;
        
        myServoX.write(ServoXPos); 
        myServoY.write(ServoYPos); 
  }

  //Dont let servos go more than +-20 from their home possitions 
  if (ServoXPos>ServoXHomePos+20) { ServoXPos=ServoXHomePos+20; }
  if (ServoXPos<ServoXHomePos-20)  { ServoXPos= ServoXHomePos-20; }
  
  if (ServoYPos>ServoYHomePos+20) { ServoYPos=ServoYHomePos+20; }
  if (ServoYPos<ServoYHomePos-20)  { ServoYPos= ServoYHomePos-20; }

  
  delay(10);
} 

int readAxis(int thisAxis) {
  // read the analog input:
  int reading = analogRead(thisAxis);

  // map the reading from the analog input range to the output range:
  reading = map(reading, 0, 1023, 0, range);

  // if the output reading is outside from the
  // rest position threshold,  use it:
  int distance = reading - center;

  if (abs(distance) < threshold) {
    distance = 0;
  }

  // return the distance for this axis:
  return distance;
}
