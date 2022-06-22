

#include <BoltDeviceCredentials.h>

#define API_KEY     "80f523b3-033a-401c-9631-2282979ae84c"
#define DEVICE_ID   "BOLT1116598"
#include <boltiot.h>

#define VIN A0 
#define cell A1
#define tempPin A2
int Vo;
float R1 = 10000;
float logR2,R2,T;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;
const float VCC   = 5.0;
const float mvc = 4.47;
float counts=0;
float mv=0;
const int model = 0;   // enter the model number (see below)

float vtg;
float soc;

float sensitivity[] ={
          0.185,// for ACS712ELCTR-05B-T
          0.100,// for ACS712ELCTR-20A-T
          0.066// for ACS712ELCTR-30A-T
     
         }; 


const float QOV =   0.5 * VCC;// set quiescent Output voltage of 0.5V
float voltage;// internal variable for voltage

void setup() {
    boltiot.Begin(Serial);
    pinMode(VIN,INPUT);
    pinMode(cell, INPUT);
    pinMode(tempPin,INPUT);
    Serial.begin(9600);// initialize serial monitor
}

void loop() {

  boltiot.processPushDataCommand(digitalRead(VIN));
  boltiot.processPushDataCommand(digitalRead(cell));
  boltiot.processPushDataCommand(digitalRead(tempPin));
  float voltage_raw =   (5.0 / 1023.0)* analogRead(VIN);// Read the voltage from sensor
  voltage =  voltage_raw - QOV + 0.012 ;// 0.000 is a value to make voltage zero when there is no current
  float current = voltage / sensitivity[model];
   counts = analogRead(cell);
  //Serial.println(String(counts));
  Vo = analogRead(tempPin);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  T = T - 273.15;
  
  mv = counts * mvc;
  vtg = mv/1000;
 
  Serial.print("Voltage: ");
  Serial.println(vtg,2);
  Serial.print("Current: ");
 Serial.print(-current,2); // print the current with 2 decimal places
  Serial.println("A");
  Serial.print("Temperature ");
  Serial.print(T);
  Serial.println("C");
 Serial.print("SoC: ");
  soc = (vtg/4.20)*100;
  Serial.print(soc);
 Serial.println(" %");
delay(1000);
}
