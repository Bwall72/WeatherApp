//Send HTTPPost with lowest temperature and average humidity data

#include <WiFiManager.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>
#include <dummy.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_AM2315.h>
#include <dht.h>
#include <time.h>

float lowTemperature = 1000;//Higert than the DHT can read, the lowest temperature will always be less than 1000;
float sumHumidities = 0;//Sum of humidities, used to take average
float temperature;
int numReadings = 0;//For average
int startTime;

int readingSeconds = 15;//Number of seconds that readings will be taken before resetting

#define DHTPin D8//Remove D to run on Arduino UNO


dht DHT;

void setup(){
  /*
   * Code for connecting to wifi left out
   * 
   */
  Serial.begin(115200);
  Serial.print("Results posted every ");
  Serial.print(readingSeconds);
  Serial.println(" seconds.");
  delay(2000);//top prevent inaccurate sensor readings. Without this delay, the first temperature reading will be 0
  startTime = millis();
}
void loop()
{
  if(millis() - startTime < readingSeconds * 1000)
  {
    //Take readings for given number of seconds before reporting
    DHT.read22(DHTPin);//read the sensor
    temperature = DHT.temperature;
    if(temperature < lowTemperature)
      lowTemperature = temperature;
    sumHumidities += DHT.humidity;
    numReadings++;
    delay(2000);
  }
  else
  {
    //After given number of seconds has passed, display results and reset all values
    Serial.print("Lowest Temperature = ");
    Serial.print(lowTemperature);
    Serial.print("C Average Humidity = ");
    sumHumidities /= (float)numReadings;//Take the average of all the humidity readings
    Serial.println(sumHumidities);    
    HTTPPost();
    //Reset variables
    sumHumidities = 0;
    lowTemperature = 1000;
    numReadings = 0;
    startTime = millis();
  }
  
}

void HTTPPost()
{
  HTTPClient http;
  http.begin("http://temperaturewebapp.azurewebsites.net/api/values");
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  //Fields that are expected in post method in ASP Application
  String post = "LowTemp=" + String(lowTemperature) + "&AvgHumidity=" + String(sumHumidities);
  http.POST(post);
  //Uncomment next 2 lines for diagnostic purposes
  //Serial.println(post);
  //Serial.writeToStream(&Serial);//Prints the response from Application to post. There will be no response if the post is correct
  http.end();
}
