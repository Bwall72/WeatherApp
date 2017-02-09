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

    //Reset variables
    sumHumidities = 0;
    lowTemperature = 1000;
    numReadings = 0;
    startTime = millis();
  }
  
}

