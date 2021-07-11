#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "MAX30100_PulseOximeter.h"
const char *ssid =  "****";     
const char *pass =  "****";

#define REPORT_TIME 5000

PulseOximeter oximeter;
uint32_t LastReportedReading = 0;
void onBeatDetected()
{
    Serial.println("Beat!");
}
float BPM;
int SpO2;
void setup() 
{
       
              
       Serial.println("Connecting to ");
       
       Serial.println(ssid); 
 
       WiFi.begin(ssid, pass);  // To connect to WiF
       
       while (WiFi.status() != WL_CONNECTED) 
          {
            delay(500);
            Serial.print(".");
          }
      Serial.println("");
      Serial.println("WiFi connected"); 
      Serial.print("Initializing Pulse Oximeter..");
      if (!oximeter.begin())
      {
         Serial.println("FAILED");
         for(;;);
      }
       else
      {
         Serial.println("SUCCESS");
            
      }
      oximeter.setOnBeatDetectedCallback(onBeatDetected);     
      oximeter.setIRLedCurrent(MAX30100_LED_CURR_24MA);
}
 
void loop() 
{     
 
  oximeter.update();
  BPM = oximeter.getHeartRate();
  SpO2 = oximeter.getSpO2();

  
  
  if (millis() - LastReportedReading > REPORT_TIME)
  {
      Serial.print("Heart rate: ");  Serial.print(BPM);
      Serial.print("\n");
      Serial.print("SpO2: "); Serial.print(SpO2);
      Serial.print("\n");                                          
      LastReportedReading = millis();
      HTTPClient http;
      String data = String(SpO2);
      String b = String(BPM);
      String reqBody = "?Spo2=" + data + "&Bpm=" + b;
      String link = "http://__.__.__.__/conn.php" + reqBody; //Fill your own IP address
      http.begin(link);
      int httpCode = http.GET();
      String payload = http.getString();
      Serial.println(httpCode);
      Serial.println(payload);
      Serial.println("Data Entered Successfully");
      http.end();
  }
 

 
   
  
 
}
