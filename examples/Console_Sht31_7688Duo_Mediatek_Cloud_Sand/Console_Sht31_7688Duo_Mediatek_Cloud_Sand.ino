/*
 * SHT31+7688DUO+MCS
 * https://mcs.mediatek.com/public/devices/DpgvR9Rj
 */
#include <Arduino.h>
#include <Wire.h>
#include "Jasper_SHT31.h"
#include <Bridge.h>
#include <HttpClient.h>
#include <BridgeClient.h>
#include <Console.h>

Jasper_SHT31 sht31 = Jasper_SHT31();
BridgeClient bc;

#define DEVICEID "XXXXXXXX" //8 characters from your MCS Device
#define DEVICEKEY "XXXXXXXXXXXXXXXX" //16 characters from your MCS Device
#define SITE_URL "api.mediatek.com"
#define LED 13

static unsigned long beat = 0;
static String commandServer;
static int commandPort = 0;
static unsigned long measure = 0;

int ret;

void setup() {
  Bridge.begin();
  Console.begin();

  getCommandServer();
  beat = millis();
  measure = millis();

  while (!Console)
    delay(10);     // will pause Zero, Leonardo, etc until serial console opens

  Console.println("SHT31 test");
  
  if (! sht31.begin(0x44)) {   // Set to 0x45 for alternate i2c addr
    Console.println("Couldn't find SHT31");
    while (1) delay(1);
  }
}

void getCommandServer()
{
  // Prepara los cabezales para la auntentificacion de la API de MCS
  String header = "deviceKey: ";
  header += DEVICEKEY;
  header += "\r\n";
  header += "Connection: close";

  HttpClient c;
  c.setHeader(header);
  c.get(SITE_URL "/mcs/v2/devices/" DEVICEID "/connections.csv");
  c.setTimeout(1000);

  const String resp = c.readString();
  const int sep = resp.indexOf(',');
  if (-1 == sep)
  {
    digitalWrite(LED, LOW);
    return;
  }
  digitalWrite(LED, HIGH);
}

void heartBeat(Client &c) {
  static const char* heartbeat = DEVICEID "," DEVICEKEY ",0";
  c.println(heartbeat);
  c.println();
  delay(100);
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

void upload_measure1(int temp) {

  String str_temp = String(temp);
  String upload_temp = "tempature,," + str_temp;
  
  String header = "Content-Type: text-csv\r\n";
  header += "deviceKey: ";
  header += DEVICEKEY;
  header += "\r\n";
  header += "Connection: close";

  HttpClient http;
  http.setHeader(header);
  http.post(SITE_URL "/mcs/v2/devices/" DEVICEID "/datapoints.csv", upload_temp.c_str());
  http.setTimeout(1000);

  delay(300);
}

void upload_measure2(int hum) {

  String str_hum = String(hum);
  String upload_hum = "humidity,," + str_hum;

  String header = "Content-Type: text-csv\r\n";
  header += "deviceKey: ";
  header += DEVICEKEY;
  header += "\r\n";
  header += "Connection: close";

  HttpClient http;
  http.setHeader(header);
  http.post(SITE_URL "/mcs/v2/devices/" DEVICEID "/datapoints.csv", upload_hum.c_str());
  http.setTimeout(1000);

  delay(300);
}
