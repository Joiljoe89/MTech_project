int A = 8;
int B = 9;
int C = 10;
int D = 11;
int y1 = 4;
int y2 = 5;
int x2 = 6;
int x1 = 7;
#define ECHOPIN 52 
#define TRIGPIN 53
#include <Servo.h>
Servo myservo1; 
Servo myservo2;
int pos = 30;

void setup() {
  Serial.begin (9600); 
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(D, OUTPUT);
  myservo1.attach(3); // attaches the servo on pin 9 to the servo object
  myservo2.attach(12);
  pinMode(ECHOPIN, INPUT);
  pinMode(TRIGPIN, OUTPUT);
  }
void Print (int R)
{
Serial.print(R);Serial.print(",.");
}
float Distance () {
digitalWrite(TRIGPIN, LOW);
delayMicroseconds(2);
digitalWrite(TRIGPIN, HIGH);
delayMicroseconds(50);
digitalWrite(TRIGPIN, LOW);
float distance = pulseIn(ECHOPIN, HIGH);
distance= distance/58;
if(distance>=300) { 
  distance = 300; }
return(distance);
}
void loop() {
if (Serial.available() > 0) {
if (Serial.read() == 'F'){
   analogWrite(x1, 150);
   analogWrite(x2, 150);
   analogWrite(y1, 150);
   analogWrite(y2, 150);
   front();
   delay(500);
   digitalWrite(A, 0);
   digitalWrite(B, 0);
   digitalWrite(C, 0);
   digitalWrite(D, 0);
   
}
}
if (Serial.available() > 0) {
if (Serial.read() == 'L'){
   analogWrite(x1, 150);
   analogWrite(x2, 150);
   analogWrite(y1, 150);
   analogWrite(y2, 150);
   left();
   delay(500);
   digitalWrite(A, 0);
   digitalWrite(B, 0);
   digitalWrite(C, 0);
   digitalWrite(D, 0);
}
}
if (Serial.available() > 0) {
if (Serial.read() == 'R'){
   analogWrite(x1, 150);
   analogWrite(x2, 150);
   analogWrite(y1, 150);
   analogWrite(y2, 150);
   right();
   delay(500);
   digitalWrite(A, 0);
   digitalWrite(B, 0);
   digitalWrite(C, 0);
   digitalWrite(D, 0);
}
}
if (Serial.available() > 0) {
if (Serial.read() == 'B'){
   analogWrite(x1, 150);
   analogWrite(x2, 150);
   analogWrite(y1, 150);
   analogWrite(y2, 150);
   back();
   delay(500);
   digitalWrite(A, 0);
   digitalWrite(B, 0);
   digitalWrite(C, 0);
   digitalWrite(D, 0);
}
}
if (Serial.available() > 0) {
if (Serial.read() == 'S'){
   digitalWrite(A, 0);
   digitalWrite(B, 0);
   digitalWrite(C, 0);
   digitalWrite(D, 0);
}
}
if (Serial.available() > 0) {
if (Serial.read() == 'A'){
  myservo2.write(110);
myservo1.write(30); // tell servo to go to position in variable 'pos'
delay(1000);
if(pos = 30) {
  Print(Distance()); }
for(pos = 30; pos <= 120; ) 
{ 
myservo1.write(pos); 
if (Serial.read() == 'D'){
Print(Distance());
pos += 10;
}
else
{
delay(15); 
}
}
}  
}
}
void right() {
 digitalWrite(A, 1);
 digitalWrite(B, 0);
 digitalWrite(C, 0);
 digitalWrite(D, 1);
}
void left() {
 digitalWrite(A, 0);
 digitalWrite(B, 1);
 digitalWrite(C, 1);
 digitalWrite(D, 0);
}
void back() {
 digitalWrite(A, 1);
 digitalWrite(B, 0);
 digitalWrite(C, 1);
 digitalWrite(D, 0);
}
void front() {
 digitalWrite(A, 0);
 digitalWrite(B, 1);
 digitalWrite(C, 0);
 digitalWrite(D, 1);
}
