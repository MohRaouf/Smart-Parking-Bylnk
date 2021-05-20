#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "NewPing.h" // include NewPing library

char auth[] = "your Token";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "your Wifi Network";
char pass[] = "your Password";

// for ESP32 microcontroller
int trigPin1 = 4;  // trigger1 pin
int echoPin1 = 15; // echo1 pin
int trigPin2 = 19; // trigger2 pin
int echoPin2 = 21; // echo pin2
BlynkTimer timer;

float getDistance(int trig, int echo, int num)
{
  NewPing sonar(trig, echo);
  float distance = sonar.ping_cm(300);
  printf("Distance %d : %f cm \n", num, distance);
  return distance;
}

void myTimerEvent()
{
  float distance1 = getDistance(trigPin1, echoPin1, 1);
  float distance2 = getDistance(trigPin2, echoPin2, 2);

  if (distance1 != 0 && distance2 != 0)
  {
    int parkingValue1 = (distance1 > 10) ? 1 : 2;
    int parkingValue2 = (distance2 > 10) ? 1 : 2;

    Blynk.virtualWrite(V5, parkingValue1);
    Blynk.virtualWrite(V6, parkingValue2);

    if (parkingValue1 == 2 && parkingValue2 == 2)
    {
      Blynk.virtualWrite(V0, "Full");
    }
    else
    {
      Blynk.virtualWrite(V0, "Available");
    }
  }
  else
  {
    printf("Error in Readings !");
  }
}

void setup()
{
  // Debug console
  Serial.begin(115200);

  Blynk.begin(auth, ssid, pass);

  // Setup a function to be called every second
  timer.setInterval(1000L, myTimerEvent);
}

void loop()
{
  Blynk.run();
  timer.run(); // Initiates BlynkTimer
}
