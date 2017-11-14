
 /*
 *make sure you go back and set a 2 step margin on each side
 *the 'on' switch for motors must be HIGH reading on hot line
 *up/down buttons to adjust/set presets
 *preset buttons
 *shut hot valve if exceeds threshold for safety
 *add sensor on hot side to trigger loop once hot water is present
*/

#define momentary A2
#define onSwitch A0
#define ONE_WIRE_BUS 2 //2 is digital pin number
#define WAKE 13


#include <LiquidCrystal.h>
#include<ButtonV2.h>

ButtonV2 Button1;
ButtonV2 Button2;
ButtonV2 Button3;
ButtonV2 Button4;
ButtonV2 Button5;

const byte tempUp = 11;
const byte tempDown = 10;
const byte M1 = 7;
const byte M2 = 8;
const byte M3 = 9;


int Mem1 = 0;
int Mem2 = 0;
int Mem3 = 0;

int switchState = 0;
int prevSwitchState = 0;

int target =200;

int absPosCold = 50;
int absPosHot = 50;

int stepCounter =0;

int Cold =0;
int Hot = 0;

LiquidCrystal lcd(12, A5, A1, A2, A3, A4);//(RS,E,DB4,DB5,DB6,DB7) don't forget the pins are backward from silkscreen!

#include "max6675.h"

int ktcSO = 22;
int ktcCS = 24;
int ktcCLK = 26;

MAX6675 ktc(ktcCLK, ktcCS, ktcSO);

void setup(){
   // set up the number of columns and rows on the LCD
  lcd.begin(16, 2);
  
  lcd.print("Waiting");
  
  Serial.begin(9600);
  
  pinMode(3, OUTPUT); //Step output on board 2(hot)
  pinMode(4, OUTPUT); //DIR output on board 2(hot)
  pinMode(12,OUTPUT);
  pinMode(5, OUTPUT); //step output on board 1(cold)
  pinMode(6, OUTPUT); //DIR output on board 1(cold)
  pinMode(13, OUTPUT);
  
  pinMode(A1,OUTPUT);
  pinMode(A2,OUTPUT);
  pinMode(A3,OUTPUT);
  pinMode(A4,OUTPUT);
  pinMode(A5,OUTPUT);


  pinMode(tempUp, INPUT_PULLUP);
  pinMode(tempDown, INPUT_PULLUP);
  pinMode(M1, INPUT_PULLUP);
  pinMode(M2, INPUT_PULLUP);
  pinMode(M3, INPUT_PULLUP);

  Button1.SetStateAndTime(LOW);
  Button2.SetStateAndTime(LOW);
  Button3.SetStateAndTime(LOW);
  Button4.SetStateAndTime(LOW);
  Button5.SetStateAndTime(LOW);
  
 
  Serial.println("Button_for_press_cases");
  Serial.println("ready");

//turn on power save mode
digitalWrite(WAKE, 0);


Serial.println("buttons_meet_proto_with_lcd");


  // give the MAX a little time to settle
  delay(500);
}
void loop(){
  
//ktype thermocouple code made more complicated with the telemetry injunction

  
  	float temp = ktc.readFahrenheit();

/*	char temp_text[30];

	dtostrf(temp, 10, 10, temp_text);

	char text[31];
	snprintf(text, 31, "%s", temp_text);
	Serial.println(text);
*/
 
 
  
  
  byte type = Button1.CheckButton(tempUp); // current time and length of time to press the button as many times as you can ie. 1.5 seconds
  switch (type)
   
  {
    case WAITING:
      break;
    case PRESSED:
      Serial.println("tempup pressed 1 time");
      target=target+1;
      break;
    case DOUBLE_PRESSED:
      Serial.println("tempup pressed 2 times");
      target=target+2;
      break;
    case MULTI_PRESSED:
      Serial.println("tempup pressed 3 times");
      target=target+3;
      break;
    case HELD:
      target=target+1;
      break;
     
  }
  
byte type2 = Button2.CheckButton(tempDown);
switch (type2)

  {
    case WAITING:
      break;
    case PRESSED:
      Serial.println("tempdown pressed 1 time");
      target=target-1;
      break;
    case DOUBLE_PRESSED:
      Serial.println("tempdown pressed 2 times");
      target=target-2;
      break;
    case MULTI_PRESSED:
      Serial.println("tempdown pressed 3 times");
      target=target-3;
      break;
    case HELD:
      target=target-1;
      break;
     
  }
    byte type3 = Button3.CheckButton(M1); // current time and length of time to press the button as many times as you can ie. 1.5 seconds
  switch (type3)
   
  {
    case WAITING:
      break;
    case PRESSED:
      Serial.println("M1 pressed 1 time");
      target = Mem1;
      break;
    case DOUBLE_PRESSED:
      Serial.println("M1 pressed 2 times, so what?");
      break;
    case MULTI_PRESSED:
      Serial.println("M1 pressed 3 times, so what?");
      break;
    case HELD:
      Mem1 = temp;
      Serial.print("Mem 1 reset to: ");
      Serial.println(Mem1);
      break;
     
  }
     byte type4 = Button4.CheckButton(M2); // current time and length of time to press the button as many times as you can ie. 1.5 seconds
  switch (type4)
   
  {
    case WAITING:
      break;
    case PRESSED:
      Serial.println("M2 pressed 1 time");
      target = Mem2;
      break;
    case DOUBLE_PRESSED:
      Serial.println("M2 pressed 2 times, so what?");
      break;
    case MULTI_PRESSED:
      Serial.println("M2 pressed 3 times, so what?");
      break;
    case HELD:
      Mem2 = temp;
      Serial.print("Mem 2 reset to: ");
      Serial.println(Mem2);
      break;
     
  }
  
     byte type5 = Button5.CheckButton(M3); // current time and length of time to press the button as many times as you can ie. 1.5 seconds
  switch (type5)
   
  {
    case WAITING:
      break;
    case PRESSED:
      Serial.println("M3 pressed 1 time");
      target = Mem3;
      break;
    case DOUBLE_PRESSED:
      Serial.println("M3 pressed 2 times, so what?");
      break;
    case MULTI_PRESSED:
      Serial.println("M3 pressed 3 times, so what?");
      break;
    case HELD:
      Mem3 = temp;
      Serial.print("Mem 3 reset to: ");
      Serial.println(Mem3);
      break;
     
  }
  
   


 while(absPosCold <20){
   for(absPosCold=19; absPosCold<35; absPosCold++){
    digitalWrite(4, HIGH);
    digitalWrite(3, HIGH);
    delay(1);          
    digitalWrite(3, LOW); 
    delay(200);
    absPosCold=((absPosCold+1));
    Serial.println(absPosCold);
   }
  }
   
 while(absPosHot < 20){
   for (absPosHot =19; absPosHot<35; absPosHot++){
    digitalWrite(6, HIGH);
    digitalWrite(5, HIGH);
    delay(1);          
    digitalWrite(5, LOW); 
    delay(200);
    absPosHot=((absPosHot +1));
    Serial.println(absPosHot);
   }
 }
  
 
   Serial.print("Current Temp:  ");
   Serial.print(temp);
   Serial.print(",  Target:  ");
   Serial.print(target);
   Serial.print(",  Differential:  ");
   Serial.println(target-temp);
   
   
   int switchState=analogRead(onSwitch);
  // Serial.println(switchState); 
   
   if(switchState > 1000){
     
     digitalWrite(WAKE, 1);
     
   //keep 1 degree margin
   if(temp > (target+1)){
    digitalWrite(6, LOW);
    digitalWrite(5, HIGH);
    delay(1);          
    digitalWrite(5, LOW); 
    delay(200);
    absPosHot = (absPosHot -1);
      
    Serial.print ("Hot Valve Position:  ");
    Serial.println(absPosHot);
    Serial.println("");
    }
    
    
    if(temp < (target-1)){ 
    digitalWrite(4, LOW);
    digitalWrite(3, HIGH);
    delay(1);          
    digitalWrite(3, LOW); 
    delay(200);
    absPosCold = (absPosCold -1);
    Serial.print ("Cold Valve Position:  ");
    Serial.println (absPosCold);
    Serial.println("");
    }
  }
  if (switchState <0){
    digitalWrite (WAKE, 0);
  }
    
      
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("Temp:  ");
lcd.print(temp);
lcd.setCursor(0, 1);
lcd.print("Target: ");
lcd.print(target);

delay(180);
}
