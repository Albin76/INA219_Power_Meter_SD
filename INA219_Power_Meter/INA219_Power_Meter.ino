// Power logger that logs to SD-card. Approx 6-7ms between samples for 29 then one with 12-13ms. 

#include <Wire.h>
#include <Adafruit_INA219.h>
#include <SPI.h>
#include "SdFat.h"
SdFat SD;

#define OLED_RESET 4
Adafruit_INA219 ina219;
unsigned long currentMillis =0;
unsigned long previousMillis = 0;
unsigned long interval = 3;  // var 100 fb, 10 ger ca 30
unsigned long counter =0;
const int chipSelect = 10;
float shuntvoltage = 0;
float busvoltage = 0;
float current_mA = 0;
float loadvoltage = 0;
float energy = 0;

SdFile myFile;

void setup() {
  ina219.begin();
  SD.begin(chipSelect);
  /*
  if (!SD.begin(chipSelect, SPI_HALF_SPEED)){
    SD.initErrorHalt();
    }
  if (!myFile.open("test2.txt", O_RDWR | O_CREAT | O_AT_END)){
    SD.errorHalt ("Failed to open file");
    }
  */ 
  myFile.open("logg.csv", O_RDWR | O_CREAT | O_AT_END);
  delay(1000);  // Can be lowered. LED on arduino blinks fast when logging.
  }


/* 
void loop(){
   
   for (int i = 0 ; i < 10 ; i++){
   myFile.print(micros());
   myFile.print("  ,   ");
   myFile.print(analogRead(A0));
   myFile.print("  ,   ");
   myFile.print(analogRead(A8));
   myFile.print("  ,   ");
   myFile.println(analogRead(A15));
   }
   myFile.flush();

 
 
  }

*/

void loop() {
  currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
   ina219values();
   previousMillis = currentMillis;
   
   myFile.print(currentMillis);
   myFile.print(", ");
   myFile.print(loadvoltage);
   myFile.print(", ");
   myFile.println(current_mA);
   counter++;
   
   // 16 bytes per line. 
   //32 or 64 best size. Max 512 but little gain. https://forum.arduino.cc/index.php?topic=383212.0   
   
   if(counter>=30){
   myFile.flush();
   counter=0;
   }
  }
}

void ina219values() {
  shuntvoltage = ina219.getShuntVoltage_mV();
  busvoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  loadvoltage = busvoltage + (shuntvoltage / 1000);
  energy = energy + loadvoltage * current_mA / 3600;  // This is not logged so can be excluded.
}
