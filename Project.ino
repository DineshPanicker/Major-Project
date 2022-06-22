#include "LiquidCrystal.h"         
const int rs = 12, en = 11, d4 = 10, d5 = 9, d6 = 8, d7 = 7;
const int currentPin = A1;
int THERMISTORPIN = A0,  BCOEFFICIENT = 3380 ;
float THERMISTORNOMINAL = 10000 , TEMPERATURENOMINAL = 25 , SERIESRESISTOR = 10000;
int sample[5];
int sensitivity = 66;
int adcValue= 0;
int offsetVoltage = 490;
double adcVoltage = 0;
double currentValue = 0;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
float temp,temp2,vtg;           
int t,v;
void setup() {
  Serial.begin(9600);
  analogReference(INTERNAL);
  lcd.begin (16,2);    
  pinMode(6,OUTPUT);                                                                           
}
void loop() {
  int i;
  float average;
 
  // take N samples in a row, with a slight delay
  for (i=0; i< 5; i++) {
   sample[i] = analogRead(THERMISTORPIN);
   delay(10);
  }
  // average all the samples out
  average = 0;
  
  for (i=0; i< 5; i++) {
     average += sample[i];
     
  }
  average /= 5;
   // convert the value to resistance
  average = 1023 / average - 1;
  average = SERIESRESISTOR / average;
 float steinhart;
  steinhart = average / THERMISTORNOMINAL;     // (R/Ro)
  steinhart = log(steinhart);                  // ln(R/Ro)
  steinhart /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
  steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  steinhart = 1.0 / steinhart;                 // Invert
  steinhart -= 273.15;                         // convert to C

  lcd.print("Temp  = ");
  lcd.print(steinhart);   
  lcd.println(" C"); 

  delay(2000);            
  adcValue = analogRead(currentPin);
  adcVoltage = (adcValue / 1024.0) * 1100;
  currentValue = ((adcVoltage - offsetVoltage) / sensitivity);
  lcd.setCursor(0,0);
  lcd.print("Voltage: ");
  lcd.print(adcVoltage,1);
  lcd.print("mV");
  delay(500);
  lcd.setCursor(0,1);
  lcd.print("Current: ");
  lcd.print(currentValue,3);
  lcd.print("A");
  delay(2000);
   if(steinhart >= 50.0 || steinhart <= 10.0)
    {
      digitalWrite(6,HIGH);
    }
    
  else
  {
    digitalWrite(6,LOW);
  }
   
  lcd.clear();                                           //Let system settle
 
    
}
