/*
 * SHT30-TemperatureSensor
 * By : Ferry Manders
 * https://github.com/ferrymanders
 * 
 * Requirements :
 * - WEMOS_SHT3x_Arduino_Library : https://github.com/wemos/WEMOS_SHT3x_Arduino_Library
 * 
 */
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <WEMOS_SHT3X.h>

// Below are my custom settings, if you want to use this script remove the next include
#include <WiFiSettings.h>

// Uncomment lines below to setup wifi
// const char* ssid     = "SSID";       // WiFi SSID
// const char* password = "WiFiPassword";  // WiFi Password

const uint8_t sht30_address = 0x45; // Hardware address of the module, you can usually find this on the board

const int cTempOffset = 0; // Temperature offset. Due to being close to the ESP8266 chip the sensor can be a few degrees off, you can correct this with this offset setting.

// ***** Do not edit below this line ***************

SHT3X sht30(sht30_address); 
ESP8266WebServer server(80);
 
float humidity, temp_f, temp_c;
String webString="";     

void setup() 
{
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  WiFi.mode(WIFI_STA);
  Serial.print("\n\r \n\rWorking to connect");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  server.on("/", [](){
    getTemperature();
    webString = "{\"temp\":\"" + String(temp_c,2) + "\",\"humidity\":\"" + String(humidity,2) + "\"}";
    server.send(200, "application/json", webString);
  });

  server.begin();
  Serial.println("HTTP server started");
}
 
void loop(void)
{
  server.handleClient();
} 
 
void getTemperature() {
  if(sht30.get()==0){
    temp_c = sht30.cTemp + cTempOffset;
    temp_f = sht30.fTemp;
    humidity = sht30.humidity;
  }else{
    Serial.println("temperature reading failed");
  }
}
