#include <ESP8266WiFi.h>
#include <ThingESP.h>
#include<DHT.h>
#define DHTPIN 5     // corresponding pin D1
#define DHT DHT11 
DHT dht(DHTPIN, DHTTYPE);

ThingESP8266 thing("sharma6354", "smartthing2", "6354814758");

int LED = LED_BUILTIN;
int relay =14;    //corresponding pin D5
unsigned long previousMillis = 0;
const long INTERVAL =10000;  


void setup()
{
  Serial.begin(115200);
  
  pinMode(LED, OUTPUT);
  pinMode(relay,OUTPUT);
  thing.SetWiFi("viveksharma", "6354814");
  thing.initDevice();
  dht.begin();
  delay(800);
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
{  float h = dht.readHumidity();
   float t = dht.readTemperature();
   if (millis() - previousMillis >= INTERVAL) {
    previousMillis = millis() ;

    String temp=String(h);
    String humidity=String(t);
    String result=temp+" "+humidity;
    thing.sendMsg("+916354814759", result);
   delay(800);
 }   
   
 thing.Handle();
}
