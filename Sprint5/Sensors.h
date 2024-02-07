#ifndef SENSORS_H
#define SENSORS_H

#include <Wire.h>
#include <Adafruit_ADS1X15.h>

class pHSensor {
private:
  Adafruit_ADS1115 ads;
  int pHArrayIndex;
  float pHArray[50];
  unsigned long samplingTime;
  unsigned long printTime;
  float voltage;
  float pHValue;
public:
  pHSensor();
  void begin();
  float calculatepH(int adcValue);
  void measure();
  void printResults();
};

class HumidityTemperatureSensor {
private:
  Adafruit_ADS1115 ads;
public:
  int readHumidity();
  int readTemperature();
};

class SalinitySensor {
private:
  int power_pin;
public:
  SalinitySensor(int pin);
  int measure();
};

class LightSensor {
private:
  Adafruit_ADS1115 ads;
  float umbralOscuridad;
  float umbralSombra;
  float umbralLuzAmbiente;
  float umbralLinterna;
public:
  LightSensor(float darknessThreshold, float shadowThreshold, float ambientLightThreshold, float flashlightThreshold);
  void measure();
};

#endif
