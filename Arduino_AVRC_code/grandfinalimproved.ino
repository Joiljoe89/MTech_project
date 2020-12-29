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
unsigned long dista = 0;
unsigned long distb = 0;

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

Fuzzy* fuzzy = new Fuzzy();

//leftdistant
FuzzySet* nearl1 = new FuzzySet(0, 7, 14, 21);
FuzzySet* nearl2 = new FuzzySet(14, 21, 28, 35);
FuzzySet* nearl3 = new FuzzySet(21, 35, 42, 49);

FuzzySet* safel1 = new FuzzySet(42, 49, 56, 63);
FuzzySet* safel2 = new FuzzySet(56, 63, 70, 77);
FuzzySet* safel3 = new FuzzySet(70, 77, 84, 91);

FuzzySet* farl1 = new FuzzySet(84, 91, 98, 105);
FuzzySet* farl2 = new FuzzySet(98, 105, 112, 119);
FuzzySet* farl3 = new FuzzySet(112, 119, 126, 133);

//rightdistant
FuzzySet* nearr1 = new FuzzySet(0, 7, 14, 21);
FuzzySet* nearr2 = new FuzzySet(14, 21, 28, 35);
FuzzySet* nearr3 = new FuzzySet(21, 35, 42, 49);

FuzzySet* safer1 = new FuzzySet(42, 49, 56, 63);
FuzzySet* safer2 = new FuzzySet(56, 63, 70, 77);
FuzzySet* safer3 = new FuzzySet(70, 77, 84, 91);

FuzzySet* farr1 = new FuzzySet(84, 91, 98, 105);
FuzzySet* farr2 = new FuzzySet(98, 105, 112, 119);
FuzzySet* farr3 = new FuzzySet(112, 119, 126, 133);

//RPM

FuzzySet* rpm1 = new FuzzySet(0, 3, 6, 9);
FuzzySet* rpm2 = new FuzzySet(6, 9, 12, 15);
FuzzySet* rpm3 = new FuzzySet(12, 15, 18, 21);
FuzzySet* rpm4 = new FuzzySet(18, 21, 24, 27);
FuzzySet* rpm5 = new FuzzySet(24, 27, 30, 33);
FuzzySet* rpm6 = new FuzzySet(30, 33, 36, 39);
FuzzySet* rpm7 = new FuzzySet(36, 39, 42, 45);
FuzzySet* rpm8 = new FuzzySet(42, 45, 48, 51);
FuzzySet* rpm9 = new FuzzySet(48, 51, 54, 57);


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
  
 // FuzzyInputleft
  FuzzyInput* distanceleft = new FuzzyInput(1);
  distanceleft->addFuzzySet(nearl1);
  distanceleft->addFuzzySet(nearl2);
  distanceleft->addFuzzySet(nearl3);
    
  distanceleft->addFuzzySet(safel1);
  distanceleft->addFuzzySet(safel2);
  distanceleft->addFuzzySet(safel3);
  
  distanceleft->addFuzzySet(farl1);
  distanceleft->addFuzzySet(farl2);
  distanceleft->addFuzzySet(farl3);
  
  fuzzy->addFuzzyInput(distanceleft);
  
  // FuzzyInputright
  FuzzyInput* distanceright = new FuzzyInput(2);
  distanceright->addFuzzySet(nearr1);
  distanceright->addFuzzySet(nearr2);
  distanceright->addFuzzySet(nearr3);
 
  distanceright->addFuzzySet(safer1);
  distanceright->addFuzzySet(safer2);
  distanceright->addFuzzySet(safer3);
  
  distanceright->addFuzzySet(farr1);
  distanceright->addFuzzySet(farr2);
  distanceright->addFuzzySet(farr3);
 
  fuzzy->addFuzzyInput(distanceright);

  // FuzzyInputrpm
  FuzzyInput* inputSpeed = new FuzzyInput(3);
  inputSpeed->addFuzzySet(rpm1);
  inputSpeed->addFuzzySet(rpm2);
  inputSpeed->addFuzzySet(rpm3);
  inputSpeed->addFuzzySet(rpm4);
  inputSpeed->addFuzzySet(rpm5);
  inputSpeed->addFuzzySet(rpm6);
  inputSpeed->addFuzzySet(rpm7);
  inputSpeed->addFuzzySet(rpm8);
  inputSpeed->addFuzzySet(rpm9);
    
  fuzzy->addFuzzyInput(inputSpeed);

  // FuzzyOutput
  FuzzyOutput* outputSpeedleft = new FuzzyOutput(1);

  FuzzySet* stopedl = new FuzzySet(0, 0, 0, 0);
  outputSpeedleft->addFuzzySet(stopedl);
  
  FuzzySet* slowl1 = new FuzzySet(90, 92, 98, 100);
  outputSpeedleft->addFuzzySet(slowl1);
  FuzzySet* slowl2 = new FuzzySet(98, 102, 108, 110);
  outputSpeedleft->addFuzzySet(slowl2);
  FuzzySet* slowl3 = new FuzzySet(108, 112, 118, 120);
  outputSpeedleft->addFuzzySet(slowl3);
  FuzzySet* normall1 = new FuzzySet(118, 122, 128, 130);
  outputSpeedleft->addFuzzySet(normall1);
  FuzzySet* normall2 = new FuzzySet(128, 132, 138, 140);
  outputSpeedleft->addFuzzySet(normall2);
  FuzzySet* fastl1 = new FuzzySet(138, 140, 166, 168);
  outputSpeedleft->addFuzzySet(fastl1);
  FuzzySet* fastl2 = new FuzzySet(166, 168, 194, 196);
  outputSpeedleft->addFuzzySet(fastl2);
  FuzzySet* fastl3 = new FuzzySet(194, 196, 220, 225);
  outputSpeedleft->addFuzzySet(fastl3);
  
  fuzzy->addFuzzyOutput(outputSpeedleft);

  // FuzzyOutput
  FuzzyOutput* outputSpeedright = new FuzzyOutput(2);

   FuzzySet* stopedr = new FuzzySet(0, 0, 0, 0);
  outputSpeedright->addFuzzySet(stopedr);
  
  FuzzySet* slowr1 = new FuzzySet(90, 92, 98, 100);
  outputSpeedright->addFuzzySet(slowr1);
  FuzzySet* slowr2 = new FuzzySet(98, 102, 108, 110);
  outputSpeedright->addFuzzySet(slowr2);
  FuzzySet* slowr3 = new FuzzySet(108, 112, 118, 120);
  outputSpeedright->addFuzzySet(slowr3);
  FuzzySet* normalr1 = new FuzzySet(118, 122, 128, 130);
  outputSpeedright->addFuzzySet(normalr1);
  FuzzySet* normalr2 = new FuzzySet(128, 132, 138, 140);
  outputSpeedright->addFuzzySet(normalr2);
  FuzzySet* fastr1 = new FuzzySet(138, 140, 166, 168);
  outputSpeedright->addFuzzySet(fastr1);
  FuzzySet* fastr2 = new FuzzySet(166, 168, 194, 196);
  outputSpeedright->addFuzzySet(fastr2);
  FuzzySet* fastr3 = new FuzzySet(194, 196, 220, 225);
  outputSpeedright->addFuzzySet(fastr3);

  fuzzy->addFuzzyOutput(outputSpeedright);
  
  
  FuzzyOutput* turning = new FuzzyOutput(3);

  FuzzySet* stopedout = new FuzzySet(0, 0, 0, 0);
  turning->addFuzzySet(stopedout);
  
  FuzzySet* backout = new FuzzySet(1, 2, 9, 10);
  turning->addFuzzySet(backout);
  
  FuzzySet* right1out = new FuzzySet(10, 11, 19, 20);
  turning->addFuzzySet(right1out);
  FuzzySet* right2out = new FuzzySet(20, 21, 29, 30);
  turning->addFuzzySet(right2out);
  FuzzySet* right3out = new FuzzySet(30, 31, 39, 40);
  turning->addFuzzySet(right3out);
  
  FuzzySet* left1out = new FuzzySet(40, 41, 49, 50);
  turning->addFuzzySet(left1out);
  FuzzySet* left2out = new FuzzySet(50, 51, 59, 60);
  turning->addFuzzySet(left2out);
  FuzzySet* left3out = new FuzzySet(60, 61, 69, 70);
  turning->addFuzzySet(left3out);
  
  FuzzySet* front1out = new FuzzySet(70, 71, 79, 80);
  turning->addFuzzySet(front1out);
  FuzzySet* front2out = new FuzzySet(80, 81, 89, 90);
  turning->addFuzzySet(front2out);
    
  fuzzy->addFuzzyOutput(turning);

// Building FuzzyRule1
  FuzzyRuleAntecedent* distanceleftNearl1ORDistancerightNearr1 = new FuzzyRuleAntecedent();
  distanceleftNearl1ORDistancerightNearr1->joinWithOR(nearl1, nearr1);
  FuzzyRuleAntecedent* inputspeedrpm9 = new FuzzyRuleAntecedent();
  inputspeedrpm9->joinSingle(rpm9);
  FuzzyRuleAntecedent* ifdistanceleftNearl1ORDistancerightNearr1ORInputspeedrpm9 = new FuzzyRuleAntecedent();
  ifdistanceleftNearl1ORDistancerightNearr1ORInputspeedrpm9->joinWithOR(distanceleftNearl1ORDistancerightNearr1, inputspeedrpm9);
  
  FuzzyRuleConsequent* thenOutputSpeedAnddirection1= new FuzzyRuleConsequent();
  thenOutputSpeedAnddirection1->addOutput(stopedl);
  thenOutputSpeedAnddirection1->addOutput(stopedr);
  thenOutputSpeedAnddirection1->addOutput(backout);

  FuzzyRule* fuzzyRule1 = new FuzzyRule(1, ifdistanceleftNearl1ORDistancerightNearr1ORInputspeedrpm9, thenOutputSpeedAnddirection1);
  fuzzy->addFuzzyRule(fuzzyRule1);
  
 // Building FuzzyRule2
  FuzzyRuleAntecedent* distanceleftNearl2ORDistancerightSafer1 = new FuzzyRuleAntecedent();
  distanceleftNearl2ORDistancerightSafer1->joinWithOR(nearl2, safer1);
  FuzzyRuleAntecedent* inputspeedrpm8 = new FuzzyRuleAntecedent();
  inputspeedrpm8->joinSingle(rpm8);
  FuzzyRuleAntecedent* ifdistanceleftNearl2ORDistancerightSafer1ORInputspeedrpm8 = new FuzzyRuleAntecedent();
  ifdistanceleftNearl2ORDistancerightSafer1ORInputspeedrpm8->joinWithOR(distanceleftNearl2ORDistancerightSafer1, inputspeedrpm8);
  
  FuzzyRuleConsequent* thenOutputSpeedAnddirection2= new FuzzyRuleConsequent();
  thenOutputSpeedAnddirection2->addOutput(normall1);
  thenOutputSpeedAnddirection2->addOutput(slowr1);
  thenOutputSpeedAnddirection2->addOutput(right1out);

  FuzzyRule* fuzzyRule2 = new FuzzyRule(2, ifdistanceleftNearl2ORDistancerightSafer1ORInputspeedrpm8, thenOutputSpeedAnddirection2);
  fuzzy->addFuzzyRule(fuzzyRule2);
  
 // Building FuzzyRule3
  FuzzyRuleAntecedent* distanceleftNearl3ORDistancerightFarr1 = new FuzzyRuleAntecedent();
  distanceleftNearl3ORDistancerightFarr1->joinWithOR(nearl3, farr1);
  FuzzyRuleAntecedent* inputspeedrpm7 = new FuzzyRuleAntecedent();
  inputspeedrpm7->joinSingle(rpm7);
  FuzzyRuleAntecedent* ifdistanceleftNearl3ORDistancerightFarr1ORInputspeedrpm7 = new FuzzyRuleAntecedent();
  ifdistanceleftNearl3ORDistancerightFarr1ORInputspeedrpm7->joinWithOR(distanceleftNearl3ORDistancerightFarr1, inputspeedrpm7);
  
  FuzzyRuleConsequent* thenOutputSpeedAnddirection3= new FuzzyRuleConsequent();
  thenOutputSpeedAnddirection3->addOutput(fastl1);
  thenOutputSpeedAnddirection3->addOutput(slowr2);
  thenOutputSpeedAnddirection3->addOutput(right2out);

  FuzzyRule* fuzzyRule3 = new FuzzyRule(3, ifdistanceleftNearl3ORDistancerightFarr1ORInputspeedrpm7, thenOutputSpeedAnddirection3);
  fuzzy->addFuzzyRule(fuzzyRule3);
  
 // Building FuzzyRule4
  FuzzyRuleAntecedent* distanceleftSafel1ORDistancerightNearr2 = new FuzzyRuleAntecedent();
  distanceleftSafel1ORDistancerightNearr2->joinWithOR(safel1, nearr2);
  FuzzyRuleAntecedent* inputspeedrpm6 = new FuzzyRuleAntecedent();
  inputspeedrpm6->joinSingle(rpm6);
  FuzzyRuleAntecedent* ifdistanceleftSafel1ORDistancerightNearr2ORInputspeedrpm6 = new FuzzyRuleAntecedent();
  ifdistanceleftSafel1ORDistancerightNearr2ORInputspeedrpm6->joinWithOR(distanceleftSafel1ORDistancerightNearr2, inputspeedrpm6);
  
  FuzzyRuleConsequent* thenOutputSpeedAnddirection4= new FuzzyRuleConsequent();
  thenOutputSpeedAnddirection4->addOutput(slowl1);
  thenOutputSpeedAnddirection4->addOutput(normalr1);
  thenOutputSpeedAnddirection4->addOutput(left1out);

  FuzzyRule* fuzzyRule4 = new FuzzyRule(4, ifdistanceleftSafel1ORDistancerightNearr2ORInputspeedrpm6, thenOutputSpeedAnddirection4);
  fuzzy->addFuzzyRule(fuzzyRule4);
  
 // Building FuzzyRule5
  FuzzyRuleAntecedent* distanceleftSafel3ORDistancerightSafer3 = new FuzzyRuleAntecedent();
  distanceleftSafel3ORDistancerightSafer3->joinWithOR(safel1, nearr2);
  FuzzyRuleAntecedent* inputspeedrpm5 = new FuzzyRuleAntecedent();
  inputspeedrpm5->joinSingle(rpm5);
  FuzzyRuleAntecedent* ifdistanceleftSafel3ORDistancerightSafer3ORInputspeedrpm5 = new FuzzyRuleAntecedent();
  ifdistanceleftSafel3ORDistancerightSafer3ORInputspeedrpm5->joinWithOR(distanceleftSafel3ORDistancerightSafer3, inputspeedrpm5);
  
  FuzzyRuleConsequent* thenOutputSpeedAnddirection5= new FuzzyRuleConsequent();
  thenOutputSpeedAnddirection5->addOutput(normall2);
  thenOutputSpeedAnddirection5->addOutput(normalr2);
  thenOutputSpeedAnddirection5->addOutput(front1out);

  FuzzyRule* fuzzyRule5 = new FuzzyRule(5, ifdistanceleftSafel1ORDistancerightNearr2ORInputspeedrpm6, thenOutputSpeedAnddirection5);
  fuzzy->addFuzzyRule(fuzzyRule5);
 
 // Building FuzzyRule6
  FuzzyRuleAntecedent* distanceleftSafel2ORDistancerightFarr2 = new FuzzyRuleAntecedent();
  distanceleftSafel2ORDistancerightFarr2->joinWithOR(safel2, farr2);
  FuzzyRuleAntecedent* inputspeedrpm4 = new FuzzyRuleAntecedent();
  inputspeedrpm4->joinSingle(rpm4);
  FuzzyRuleAntecedent* ifdistanceleftSafel2ORDistancerightFarr2ORInputspeedrpm4 = new FuzzyRuleAntecedent();
  ifdistanceleftSafel2ORDistancerightFarr2ORInputspeedrpm4->joinWithOR(distanceleftSafel2ORDistancerightFarr2, inputspeedrpm4);
  
  FuzzyRuleConsequent* thenOutputSpeedAnddirection6= new FuzzyRuleConsequent();
  thenOutputSpeedAnddirection6->addOutput(fastl2);
  thenOutputSpeedAnddirection6->addOutput(slowr3);
  thenOutputSpeedAnddirection6->addOutput(right3out);

  FuzzyRule* fuzzyRule6 = new FuzzyRule(6, ifdistanceleftSafel2ORDistancerightFarr2ORInputspeedrpm4, thenOutputSpeedAnddirection6);
  fuzzy->addFuzzyRule(fuzzyRule6); 
  
 // Building FuzzyRule7
  FuzzyRuleAntecedent* distanceleftFarl1ORDistancerightNearr3 = new FuzzyRuleAntecedent();
  distanceleftFarl1ORDistancerightNearr3->joinWithOR(farl1, nearr2);
  FuzzyRuleAntecedent* inputspeedrpm3 = new FuzzyRuleAntecedent();
  inputspeedrpm3->joinSingle(rpm3);
  FuzzyRuleAntecedent* ifdistanceleftFarl1ORDistancerightNearr3ORInputspeedrpm3 = new FuzzyRuleAntecedent();
  ifdistanceleftFarl1ORDistancerightNearr3ORInputspeedrpm3->joinWithOR(distanceleftFarl1ORDistancerightNearr3, inputspeedrpm3);
  
  FuzzyRuleConsequent* thenOutputSpeedAnddirection7= new FuzzyRuleConsequent();
  thenOutputSpeedAnddirection7->addOutput(slowl2);
  thenOutputSpeedAnddirection7->addOutput(fastr1);
  thenOutputSpeedAnddirection7->addOutput(left2out);

  FuzzyRule* fuzzyRule7 = new FuzzyRule(7, ifdistanceleftFarl1ORDistancerightNearr3ORInputspeedrpm3, thenOutputSpeedAnddirection7);
  fuzzy->addFuzzyRule(fuzzyRule7); 
  
 // Building FuzzyRule8
  FuzzyRuleAntecedent* distanceleftFarl2ORDistancerightSafer2 = new FuzzyRuleAntecedent();
  distanceleftFarl2ORDistancerightSafer2->joinWithOR(farl2, safer2);
  FuzzyRuleAntecedent* inputspeedrpm2 = new FuzzyRuleAntecedent();
  inputspeedrpm2->joinSingle(rpm2);
  FuzzyRuleAntecedent* ifdistanceleftFarl1ORDistancerightNearr3ORInputspeedrpm2 = new FuzzyRuleAntecedent();
  ifdistanceleftFarl1ORDistancerightNearr3ORInputspeedrpm2->joinWithOR(distanceleftFarl1ORDistancerightNearr3, inputspeedrpm2);
  
  FuzzyRuleConsequent* thenOutputSpeedAnddirection8= new FuzzyRuleConsequent();
  thenOutputSpeedAnddirection8->addOutput(slowl3);
  thenOutputSpeedAnddirection8->addOutput(fastr2);
  thenOutputSpeedAnddirection8->addOutput(left3out);

  FuzzyRule* fuzzyRule8 = new FuzzyRule(8, ifdistanceleftFarl1ORDistancerightNearr3ORInputspeedrpm2, thenOutputSpeedAnddirection8);
  fuzzy->addFuzzyRule(fuzzyRule8); 
  
 // Building FuzzyRule9
  FuzzyRuleAntecedent* distanceleftFarl3ORDistancerightFarr3 = new FuzzyRuleAntecedent();
  distanceleftFarl3ORDistancerightFarr3->joinWithOR(farl3, farr3);
  FuzzyRuleAntecedent* inputspeedrpm1 = new FuzzyRuleAntecedent();
  inputspeedrpm1->joinSingle(rpm1);
  FuzzyRuleAntecedent* ifdistanceleftFarl3ORDistancerightFarr3ORInputspeedrpm1 = new FuzzyRuleAntecedent();
  ifdistanceleftFarl3ORDistancerightFarr3ORInputspeedrpm1->joinWithOR(distanceleftFarl3ORDistancerightFarr3, inputspeedrpm1);
  
  FuzzyRuleConsequent* thenOutputSpeedAnddirection9= new FuzzyRuleConsequent();
  thenOutputSpeedAnddirection9->addOutput(fastl3);
  thenOutputSpeedAnddirection9->addOutput(fastr3);
  thenOutputSpeedAnddirection9->addOutput(front2out);

  FuzzyRule* fuzzyRule9 = new FuzzyRule(9, ifdistanceleftFarl3ORDistancerightFarr3ORInputspeedrpm1, thenOutputSpeedAnddirection9);
  fuzzy->addFuzzyRule(fuzzyRule9); 
  
 }

void loop()
{
  myservoleft.write(65);
  myservoright.write(83);
 for (var1 = 0; var1 < 2000; var1 ++) {
  delay(1000);
  ultrasonic();
  encoder();
  compass();
 if(dist1 <= 35 && dist2 <= 35)
 { 
   back();
   analogWrite(x1, 0);
   analogWrite(x2, 0);
   analogWrite(y1, 0);
   analogWrite(y2, 0);
   delay(400);
   myservoleft.write(25);
   myservoright.write(135);
   delay(800);
   ultrasonic();
   delay(300);
   if (dist1 <= 10) {
   analogWrite(x1, 112);
   analogWrite(x2, 112);
   analogWrite(y1, 112);
   analogWrite(y2, 112);
   back();
      }
   else if (dist2 <= 10) {
   analogWrite(x1, 112);
   analogWrite(x2, 112);
   analogWrite(y1, 112);
   analogWrite(y2, 112);
   back();
      } 
   else if(dist1 > dist2) {
   Serial.println("left");
   analogWrite(x1, 90);
   analogWrite(x2, 90);
   analogWrite(y1, 112);
   analogWrite(y2, 112);
   left2(); }
   else if (dist2 > dist1) {
   Serial.println("right");
   analogWrite(x1, 112);
   analogWrite(x2, 112);
   analogWrite(y1, 90);
   analogWrite(y2, 90);
   right2(); }
   else if (dist1 == dist2) {
   analogWrite(x1, 112);
   analogWrite(x2, 112);
   analogWrite(y1, 112);
   analogWrite(y2, 112);
   back();
      }
   break;
 } 
 else if(dist1 >= 30 && dist2 <= 30)
 { analogWrite(x1, 100);
   analogWrite(x2, 100);
   analogWrite(y1, 100);
   analogWrite(y2, 100);
   back();
   delay(700);
   analogWrite(x1, 85);
   analogWrite(x2, 85);
   analogWrite(y1, 100);
   analogWrite(y2, 100);
   left2();
   delay(300);
   break;
 } 
 else if(dist1 <= 30 && dist2 >= 30)
 { analogWrite(x1, 100);
   analogWrite(x2, 100);
   analogWrite(y1, 100);
   analogWrite(y2, 100);
   back();
   delay(700);
   right2();
   analogWrite(x1, 100);
   analogWrite(x2, 100);
   analogWrite(y1, 85);
   analogWrite(y2, 85);
   delay(300);
   break;
 } 
 else if(rpm >= 0 && rpm <= 57 && heading >= 170 && heading <= 190)
 {
   fuzzystart();
 }
 else if(heading >= 91 && heading <= 169)
 {
   analogWrite(x1, 120);
   analogWrite(x2, 120);
   analogWrite(y1, 90);
   analogWrite(y2, 90);
   right2();
   delay(200);
   break;
 }
 else if(heading >= 191 && heading <= 270)
 {
   analogWrite(x1, 95);
   analogWrite(x2, 95);
   analogWrite(y1, 120);
   analogWrite(y2, 120);
   left2();
   delay(200);
   break;
 }
 else if(heading >= 0 && heading <= 90)
 {
   analogWrite(x1, 112);
   analogWrite(x2, 112);
   analogWrite(y1, 112);
   analogWrite(y2, 112);
   right3();
   delay(200);
   break;
 }
 else if(heading >= 271 && heading <= 360)
 {
   analogWrite(x1, 120);
   analogWrite(x2, 120);
   analogWrite(y1, 120);
   analogWrite(y2, 120);
   left3();
   delay(200);
   break;
 }
 else { Serial.println("condition not met !! "); }
}
}

void fuzzystart() {
  
  fuzzy->setInput(1, dist1);
  fuzzy->setInput(2, dist2);
  fuzzy->setInput(3, rpm);
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
  
  a = output3;
  b = output3;
  c = output3;
  d = output3;
  if(a >= 1 && a <= 10){
    back1(); }
  else if (b >= 10 && b <= 40) {
    right1(); }
  else if(c >= 40 && c <= 70) {
    left1(); }
  else if(d >= 70 && d <= 90) {
    front(); }
  else {
     }
}

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
  if (dist1 >= 130){
    dist1 = 130;
  }
   if (dist2 >= 130){
    dist2 = 130;
  }
  Serial.print("distanceleft=");
  Serial.println(dist1);
  Serial.print("distanceright=");
  Serial.println(dist2); }
  
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
     
    rpm = count * 15;
    Serial.print("rpm:\t");
    Serial.println(rpm);
    count = 0;
    last_printed_count =  millis();
    break;
  }
  }
}

void right1() {
 analogWrite(A, 225);
 analogWrite(B, 0);
 analogWrite(C, 0);
 analogWrite(D, 225);
 
}
void right2() {
 analogWrite(A, 225);
 analogWrite(B, 0);
 analogWrite(C, 0);
 analogWrite(D, 225);
 
}
void right3() {
 analogWrite(A, 225);
 analogWrite(B, 0);
 analogWrite(C, 0);
 analogWrite(D, 225);
 
}
void left1() {
 analogWrite(A, 0);
 analogWrite(B, 225);
 analogWrite(C, 225);
 analogWrite(D, 0);
 
}
void left2() {
 analogWrite(A, 0);
 analogWrite(B, 225);
 analogWrite(C, 225);
 analogWrite(D, 0);
 
}
void left3() {
 analogWrite(A, 0);
 analogWrite(B, 225);
 analogWrite(C, 225);
 analogWrite(D, 0);
 
}
void back() {
 analogWrite(A, 225);
 analogWrite(B, 0);
 analogWrite(C, 225);
 analogWrite(D, 0);
 delay(500);
 analogWrite(x1, 0);
 analogWrite(x2, 0);
 analogWrite(y1, 0);
 analogWrite(y2, 0);
}
void back1() {
 analogWrite(A, 225);
 analogWrite(B, 0);
 analogWrite(C, 225);
 analogWrite(D, 0);
  }
void front() {
 analogWrite(A, 0);
 analogWrite(B, 225);
 analogWrite(C, 0);
 analogWrite(D, 225);
 delay(400);
 analogWrite(x1, 0);
 analogWrite(x2, 0);
 analogWrite(y1, 0);
 analogWrite(y2, 0);
}
