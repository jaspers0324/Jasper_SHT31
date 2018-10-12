/*
 * Source from https://www.adafruit.com/products/2857
 * 
 */
 
#include <Arduino.h>
#include <Wire.h>
#include "Jasper_SHT31.h"

Jasper_SHT31 sht31 = Jasper_SHT31();

void setup() {
  Serial.begin(9600);

  //while (!Serial)
    //delay(10);     // will pause Zero, Leonardo, etc until serial console opens

  //Serial.println("SHT31 test");
  if (! sht31.begin(0x44)) {   // Set to 0x45 for alternate i2c addr
    Serial.println("Couldn't find SHT31");
    while (1) delay(1);
  }
}


void loop() {
  float t = sht31.readTemperature();
  float h = sht31.readHumidity();

  if (! isnan(t)) {  // check if 'is not a number'
    //Serial.print("Temp *C = "); 
    Serial.print(t);
  } else { 
    Serial.println("Failed to read temperature");
  }

  Serial.print(',');
  
  if (! isnan(h)) {  // check if 'is not a number'
    //Serial.print("Hum. % = "); 
    Serial.print(h);
  } else { 
    Serial.println("Failed to read humidity");
  }
  Serial.println();
  delay(1000);
}
