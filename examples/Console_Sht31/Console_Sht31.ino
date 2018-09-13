/*
 * SHT31+7688DUO
 */
#include <Arduino.h>
#include <Wire.h>
#include "Jasper_SHT31.h"
#include <Console.h>
Jasper_SHT31 sht31 = Jasper_SHT31();

void setup() {
  Bridge.begin();
  Console.begin();

  while (!Console)
    delay(10);     // will pause Zero, Leonardo, etc until serial console opens

  Console.println("SHT31 test");
  
  if (! sht31.begin(0x44)) {   // Set to 0x45 for alternate i2c addr
    Console.println("Couldn't find SHT31");
    while (1) delay(1);
  }
}


void loop() {
  float t = sht31.readTemperature();
  float h = sht31.readHumidity();

  if (! isnan(t)) {  // check if 'is not a number'
    Console.print("Temp *C = "); Console.println(t);
  } else { 
    Console.println("Failed to read temperature");
  }
  
  if (! isnan(h)) {  // check if 'is not a number'
    Console.print("Hum. % = "); Console.println(h);
  } else { 
    Console.println("Failed to read humidity");
  }
  Console.println();
  delay(1000);
}
