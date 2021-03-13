#include <ESP8266WiFi.h>
#include <Filters.h>   // include this libray
#include <ThingESP.h>
#include<DHT.h>  //include this library
#include <Wire.h> //include this library
#define DHTPIN 5     // corresponding pin D1
 
DHT dht(DHTPIN, DHTTYPE);
float testFrequency = 50;                     // test signal frequency (Hz)
float windowLength = 40.0/testFrequency;     // how long to average the signal, for statistist
int Sensor = 0; //Sensor analog input, here it's A0
float intercept = -0.04; // to be adjusted based on calibration testing
float slope = 0.0405; // to be adjusted based on calibration testing
float current_Volts; // Voltage


ThingESP8266 thing("sharma6354", "smartthing2", "6354814758");

int LED = LED_BUILTIN;
int relay =14;    //corresponding pin D5
unsigned long previousMillis = 0;
const long INTERVAL =10000;  


void setup()
{
  Serial.begin(115200);
  dht.begin();
  pinMode(LED, OUTPUT);
  pinMode(relay,OUTPUT);
  thing.SetWiFi("viveksharma", "6354814");
  thing.initDevice();

}


String HandleResponse(String query)
{

  if (query == "led on") {
    digitalWrite(LED, 0);
    return "Done: LED Turned ON";
  }

  else if (query == "led off") {
    digitalWrite(LED, 1);
    return "Done: LED Turned OFF";
  }

  else if (query == "led status")
    return digitalRead(LED) ? "LED is OFF" : "LED is ON";
    else if(query=="relay on")
    {
      digitalWrite(relay, 0);
    return "Done: Relay Turned On";
    }
    else if(query=="relay off")
    {
      digitalWrite(relay, 1);
    return "Done: Relay Turned Off";
    }


  else return "Your query was invalid..";

}


void loop()
{  
   RunningStatistics inputStats;                //Easy life lines, actual calculation of the RMS requires a load of coding
   inputStats.setWindowSecs( windowLength );
   float h = dht.readHumidity();
   float t = dht.readTemperature();
   Sensor = analogRead(A0);  // read the analog in value:
   inputStats.input(Sensor);  // log to Stats function
   current_Volts = intercept + slope * inputStats.sigma(); //Calibartions for offset and amplitude
   current_Volts= current_Volts*(40.3231);                //Further calibrations for the amplitude
      
   if (millis() - previousMillis >= INTERVAL) {
    previousMillis = millis() ;

    String temp=String(h);
    String humidity=String(t);
    String voltage=String(current_Volts);
    String result=temp+" "+humidity+" "+voltage;
    thing.sendMsg("+916354814759", result);
  }   
   
 thing.Handle();
  delay(1600);

}
