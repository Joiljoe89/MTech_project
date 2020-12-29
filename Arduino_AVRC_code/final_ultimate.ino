#include <FuzzyRule.h>
#include <FuzzyComposition.h>
#include <Fuzzy.h>
#include <FuzzyRuleConsequent.h>
#include <FuzzyOutput.h>
#include <FuzzyInput.h>
#include <FuzzyIO.h>
#include <FuzzySet.h>
#include <FuzzyRuleAntecedent.h>

#include<Wire.h>
#define Addr 0x1E 
unsigned long heading;

#include <Servo.h>
Servo myservoleft; 
Servo myservoright; 

int echoPin1 = 52;
int echoPin2 = 48;
int initPin1 = 53;
int initPin2 = 49;
// stores the pulse in Micro Seconds
unsigned long pulseTime1 = 0;
unsigned long pulseTime2 = 0;
// variable for storing the distance (cm)
unsigned long dist1 = 0;
unsigned long dist2 = 0;
unsigned long dist = 0;

#define PIN_SENSOR_OUTPUT      31
unsigned int last_printed_count;
unsigned int count;
unsigned int last_sensor_reading;
unsigned int rpm;
int var = 0;
int var1 = 0;
int A = 8;
int B = 9;
int C = 10;
int D = 11;
int y1 = 4;
int y2 = 5;
int x2 = 6;
int x1 = 7;
unsigned int a = 0;
unsigned int b = 0;
unsigned int c = 0;
unsigned int d = 0;
unsigned int e = 0;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////joe//////////////
Fuzzy* fuzzy = new Fuzzy();

//leftdistant
FuzzySet* nearl = new FuzzySet(0, 0, 10, 30);

FuzzySet* safel = new FuzzySet(30, 40, 60, 140);

FuzzySet* farl = new FuzzySet(70, 145, 150, 150);

//rightdistant
FuzzySet* nearr = new FuzzySet(0, 0, 10, 30);

FuzzySet* safer = new FuzzySet(30, 40, 60, 140);

FuzzySet* farr = new FuzzySet(70, 145, 150, 150);

//heading direction

FuzzySet* east = new FuzzySet(0, 0, 170, 175);
FuzzySet* south = new FuzzySet(175, 180, 180, 185);
FuzzySet* west = new FuzzySet(185, 190, 360, 360);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////joe/////////////////
void setup()
{
  pinMode(PIN_SENSOR_OUTPUT, INPUT);
  digitalWrite(PIN_SENSOR_OUTPUT, HIGH); 
  Serial.begin(9600);
  myservoleft.attach(3);
  myservoright.attach(12);
  pinMode(initPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(initPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  
  Wire.begin();
  Wire.beginTransmission(Addr);
  Wire.write(byte(0x02));
  Wire.write(byte(0x00));
  Wire.endTransmission();
 
  // just starting so last printed count at time zero
  last_printed_count = 0;
  // just starting, preset count to zero
  count = 0; 
  rpm = 0;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////joe//////////////////
 // FuzzyInputleft
  FuzzyInput* distanceleft = new FuzzyInput(1);
  distanceleft->addFuzzySet(nearl);
    
  distanceleft->addFuzzySet(safel);
 
  distanceleft->addFuzzySet(farl);
 
  fuzzy->addFuzzyInput(distanceleft);
  
  // FuzzyInputright
  FuzzyInput* distanceright = new FuzzyInput(2);
  distanceright->addFuzzySet(nearr);
  
  distanceright->addFuzzySet(safer);
 
  distanceright->addFuzzySet(farr);

  fuzzy->addFuzzyInput(distanceright);

  // Fuzzyheading angle
  FuzzyInput* inputAngle = new FuzzyInput(3);
  inputAngle->addFuzzySet(east);
  inputAngle->addFuzzySet(south);
  inputAngle->addFuzzySet(west);
  
  fuzzy->addFuzzyInput(inputAngle);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////joe////////////
  // FuzzyOutput
  FuzzyOutput* outputSpeedleft = new FuzzyOutput(1);

  FuzzySet* slowl = new FuzzySet(0, 0, 80, 90);
  outputSpeedleft->addFuzzySet(slowl);

  FuzzySet* normall = new FuzzySet(90, 100, 150, 160);
  outputSpeedleft->addFuzzySet(normall);

  FuzzySet* fastl = new FuzzySet(150, 160, 225, 225);
  outputSpeedleft->addFuzzySet(fastl);

  fuzzy->addFuzzyOutput(outputSpeedleft);

  // FuzzyOutput
  FuzzyOutput* outputSpeedright = new FuzzyOutput(2);

  FuzzySet* slowr = new FuzzySet(0, 0, 80, 90);
  outputSpeedright->addFuzzySet(slowr);

  FuzzySet* normalr = new FuzzySet(50, 90, 150, 160);
  outputSpeedright->addFuzzySet(normalr);
  
  FuzzySet* fastr = new FuzzySet(160, 170, 225, 225);
  outputSpeedright->addFuzzySet(fastr);

  fuzzy->addFuzzyOutput(outputSpeedright);
  
  
  FuzzyOutput* turning = new FuzzyOutput(3);

  FuzzySet* left = new FuzzySet(0, 0, 1, 10);
  turning->addFuzzySet(left);
  
  FuzzySet* front = new FuzzySet(10, 15, 15, 20);
  turning->addFuzzySet(front);
  
  FuzzySet* right = new FuzzySet(20, 29, 30, 30);
  turning->addFuzzySet(right);
    
  fuzzy->addFuzzyOutput(turning);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////joe rules///////////////////////////
// Building FuzzyRule1
  FuzzyRuleAntecedent* distanceleftfarORDistancerightfar = new FuzzyRuleAntecedent();
  distanceleftfarORDistancerightfar->joinWithOR(farl, farr);
  FuzzyRuleAntecedent* inputsouth = new FuzzyRuleAntecedent();
  inputsouth->joinSingle(south);
  FuzzyRuleAntecedent* ifdistanceleftfarORDistancerightfarORInputsouth = new FuzzyRuleAntecedent();
  ifdistanceleftfarORDistancerightfarORInputsouth->joinWithOR(distanceleftfarORDistancerightfar, inputsouth);
  
  FuzzyRuleConsequent* thenOutputSpeedAnddirection1= new FuzzyRuleConsequent();
  thenOutputSpeedAnddirection1->addOutput(normall);
  thenOutputSpeedAnddirection1->addOutput(normalr);
  thenOutputSpeedAnddirection1->addOutput(front);

  FuzzyRule* fuzzyRule1 = new FuzzyRule(1, ifdistanceleftfarORDistancerightfarORInputsouth, thenOutputSpeedAnddirection1);
  fuzzy->addFuzzyRule(fuzzyRule1);
  
 // Building FuzzyRule2
  FuzzyRuleAntecedent* distanceleftNearORDistancerightSafe = new FuzzyRuleAntecedent();
  distanceleftNearORDistancerightSafe->joinWithOR(nearl, safer);
  FuzzyRuleAntecedent* inputeast = new FuzzyRuleAntecedent();
  inputeast->joinSingle(east);
  FuzzyRuleAntecedent* ifdistanceleftNearORDistancerightSafeORInputeast = new FuzzyRuleAntecedent();
  ifdistanceleftNearORDistancerightSafeORInputeast->joinWithOR(distanceleftNearORDistancerightSafe, inputeast);
  
  FuzzyRuleConsequent* thenOutputSpeedAnddirection2= new FuzzyRuleConsequent();
  thenOutputSpeedAnddirection2->addOutput(fastl);
  thenOutputSpeedAnddirection2->addOutput(slowr);
  thenOutputSpeedAnddirection2->addOutput(right);

  FuzzyRule* fuzzyRule2 = new FuzzyRule(2, ifdistanceleftNearORDistancerightSafeORInputeast, thenOutputSpeedAnddirection2);
  fuzzy->addFuzzyRule(fuzzyRule2);
  
 // Building FuzzyRule3
  FuzzyRuleAntecedent* distanceleftsafeORDistancerightnear = new FuzzyRuleAntecedent();
  distanceleftsafeORDistancerightnear->joinWithOR(safel, nearr);
  FuzzyRuleAntecedent* inputwest = new FuzzyRuleAntecedent();
  inputwest->joinSingle(west);
  FuzzyRuleAntecedent* ifdistanceleftsafeORDistancerightnearORInputwest = new FuzzyRuleAntecedent();
  ifdistanceleftsafeORDistancerightnearORInputwest->joinWithOR(distanceleftsafeORDistancerightnear, inputwest);
  
  FuzzyRuleConsequent* thenOutputSpeedAnddirection3= new FuzzyRuleConsequent();
  thenOutputSpeedAnddirection3->addOutput(slowl);
  thenOutputSpeedAnddirection3->addOutput(fastr);
  thenOutputSpeedAnddirection3->addOutput(left);

  FuzzyRule* fuzzyRule3 = new FuzzyRule(3, ifdistanceleftsafeORDistancerightnearORInputwest, thenOutputSpeedAnddirection3);
  fuzzy->addFuzzyRule(fuzzyRule3);
  
 }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////joe////////////////////////
void loop()
{
  myservoleft.write(62);
  myservoright.write(90);
 for (var1 = 0; var1 < 2000; var1 ++) {
  ultrasonic();
  encoder();
  compass();
 if(dist1 <= 15) {
   back();
   delay(700);
   right();
   delay(1500);
   delay(700);
   right();
   delay(1500);
   front();
   delay(500);
   front();
   delay(500);
   break;
 }
 
 else if(dist2 <= 15) {
   back();
   delay(700);
   left();
   delay(1500);
   back();
   delay(700);
   left();
   delay(1500);
   front();
   delay(500);
   front();
   delay(500);
   break;
 }
 
 else { fuzzystart(); 
 break;}
 }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////joe////////////////////
void fuzzystart() {
  
  fuzzy->setInput(1, dist1);
  fuzzy->setInput(2, dist2);
  fuzzy->setInput(3, heading);
  fuzzy->fuzzify();
  
  float output1 = fuzzy->defuzzify(1);
  float output2 = fuzzy->defuzzify(2);
  float output3 = fuzzy->defuzzify(3);
  
  Serial.print("outputspeedleft: ");
  Serial.print(output1);
  Serial.print(", outputspeedright: ");
  Serial.println(output2);
  Serial.print(", direction: ");
  Serial.println(output3);
  
  analogWrite(x1, output1);
  analogWrite(x2, output1);
  analogWrite(y1, output2);
  analogWrite(y2, output2);
  
  a = output1;
  b = output2;
  c = output3;  
  
  if (c >= 0 && c <= 14) {
    left();}
  else if (c >= 14.1 && c <= 15.9) {
    front(); }
  else if (c >= 16 && c <=30) {
    right(); }
  
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////joe////////////
void ultrasonic() {
  //ultrasonic sensor
   digitalWrite(initPin1, HIGH);
   delayMicroseconds(10);
   digitalWrite(initPin1, LOW);
  pulseTime1 = pulseIn(echoPin1, HIGH);
  
   digitalWrite(initPin2, HIGH);
   delayMicroseconds(10);
   digitalWrite(initPin2, LOW);
  pulseTime2 = pulseIn(echoPin2, HIGH);
  dist1 = pulseTime1/58;
  dist2 = pulseTime2/58;
  if (dist1 >= 140){
    dist1 = 140;
  }
   if (dist2 >= 140){
    dist2 = 140;
  }
  Serial.print("distanceleft=");
  Serial.println(dist1);
  Serial.print("distanceright=");
  Serial.println(dist2); }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////joe/////////////////////////
void compass() {
    // compass
  int x, y, z;
// Initiate communications with compass
Wire.beginTransmission(Addr);
Wire.write(byte(0x03)); // Send request to X MSB register
Wire.endTransmission();
Wire.requestFrom(Addr, 6); // Request 6 bytes; 2 bytes per axis
if(Wire.available() <=6) { // If 6 bytes available
x = Wire.read() << 8 | Wire.read();
z = Wire.read() << 8 | Wire.read();
y = Wire.read() << 8 | Wire.read();
}
if (y > 0 && x > 0)
{
  heading = 180 - atan2(x,y)*180*7 / 22;
}
if (y > 0 && x < 0)
{
  heading = 180 - atan2(x,y)*180*7 / 22;
}
else if (y < 0 && x < 0)
{
  heading = 450 + ( atan2(y,x)*180*7 / 22);
}
else if (y < 0 && x > 0)
{
  heading =   (90 + (atan2(y,x)*180*7 / 22));
}
else if (y = 0 && x < 0)
{
  heading = 180.0;
}
else if (y = 0 && x > 0)
{
  heading = 0.0;
}
else
{
  // do Thing C
}
Serial.print("heading=");
Serial.println(heading); }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////joe/////////////////////
void encoder() {
  //encoder   
  for (var = 0; var < 25; var ++)
  {
   int hall_effect_sensor_reading;
  // 1 = no magnet present
  // 0 = magnet detected
  hall_effect_sensor_reading = digitalRead(PIN_SENSOR_OUTPUT);
 
  if ((hall_effect_sensor_reading == 0) && (last_sensor_reading == 1))
  {
    count++;  
  }
  
  last_sensor_reading = hall_effect_sensor_reading;
  
  if( millis() > last_printed_count + 4000) {
     
    rpm = 15 * count;
    Serial.print("rpm:\t");
    Serial.println(rpm);
    count = 0;
    last_printed_count =  millis();
    break;
  }
  }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////joe////////////////////
void right() {
 analogWrite(A, 225);
 analogWrite(B, 0);
 analogWrite(C, 0);
 analogWrite(D, 225);
 Serial.println("right");
}

void smallright() {
 analogWrite(A, 225);
 analogWrite(B, 0);
 analogWrite(C, 0);
 analogWrite(D, 225);
 Serial.println("smallright");
 delay(500);
 stoped();
}

void bigright() {
 analogWrite(A, 225);
 analogWrite(B, 0);
 analogWrite(C, 0);
 analogWrite(D, 225);
 Serial.println("bigright");
 delay(1000);
 stoped();
}

void left() {
 analogWrite(A, 0);
 analogWrite(B, 225);
 analogWrite(C, 225);
 analogWrite(D, 0);
 Serial.println("left");
}

void smallleft() {
 analogWrite(A, 0);
 analogWrite(B, 225);
 analogWrite(C, 225);
 analogWrite(D, 0);
 Serial.println("smallleft");
 delay(500);
 stoped();
}

void bigleft() {
 analogWrite(A, 0);
 analogWrite(B, 225);
 analogWrite(C, 225);
 analogWrite(D, 0);
 Serial.println("bigleft");
 delay(1000);
 stoped();
}

void front() {
 analogWrite(A, 0);
 analogWrite(B, 225);
 analogWrite(C, 0);
 analogWrite(D, 225);
 Serial.println("front");
 
}

void back() {
 analogWrite(A, 225);
 analogWrite(B, 0);
 analogWrite(C, 225);
 analogWrite(D, 0);
 Serial.println("back");
}

void stoped() {
 analogWrite(A, 0);
 analogWrite(B, 0);
 analogWrite(C, 0);
 analogWrite(D, 0);
 Serial.println("stop");
}
