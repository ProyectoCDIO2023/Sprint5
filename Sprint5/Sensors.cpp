#include "Sensors.h"

#define Offset 0.60        // Offset para el cálculo del pH (ajuste específico según el sensor)
#define samplingInterval 20 // Intervalo de tiempo entre muestras de pH (en milisegundos)
#define printInterval 800  // Intervalo de tiempo para imprimir los resultados (en milisegundos)
#define ArrayLength 50     // Longitud del array de muestras de pH


pHSensor::pHSensor() {}

void pHSensor::begin() {
  ads.begin();
  ads.setGain(GAIN_ONE);
}

float pHSensor::calculatepH(int adcValue) {
  float voltage = adcValue * 0.0001875; // La resolución es 0.1875 mV por bit para GAIN_TWOTHIRDS
  float pHValue = 3.5 * voltage + Offset;
  return pHValue;
}

void pHSensor::measure() {
  if (millis() - samplingTime > samplingInterval) {
    int16_t adcValue;
    adcValue = ads.readADC_SingleEnded(3);
    pHValue = calculatepH(adcValue);
    voltage = adcValue * 0.0001875; // Convertir a voltaje
    pHArray[pHArrayIndex++] = pHValue;
    if (pHArrayIndex == 50) pHArrayIndex = 0;
    samplingTime = millis();
  }
}

void pHSensor::printResults() {
  if (millis() - printTime > printInterval) {
    Serial.print("Voltage: ");
    Serial.print(voltage, 2);
    Serial.print("    pH value: ");
    Serial.println(pHValue, 2);
    printTime = millis();
  }
}

int HumidityTemperatureSensor::readHumidity() {
  int16_t medidaMojado = ads.readADC_SingleEnded(2);
  int humidityValueMojado = map(medidaMojado, 17000, 31000, 100, 0);
  return humidityValueMojado;
}

int HumidityTemperatureSensor::readTemperature() {
  int16_t adcValue = ads.readADC_SingleEnded(1);
  float voltage = (adcValue * 4.096) / 32767;
  int temperature = ((voltage - 0.79) / 0.035);
  return temperature;
}

SalinitySensor::SalinitySensor(int pin) : power_pin(pin) {}

int SalinitySensor::measure() {
  digitalWrite(power_pin, HIGH); // Alimentar la sonda con un tren de pulsos
  delay(100);

  int16_t adc0 = analogRead(A0); // Leer cuando hay un nivel alto
  digitalWrite(power_pin, LOW);
  delay(100);

  float salinidad = map(adc0, 0, 1023, 0, 100); // Realizar el cálculo de la salinidad
  float salinidadReal = 480 + 15.55 * salinidad - 0.31 * pow(salinidad, 2) - 0.012 * pow(salinidad, 3) + 0.0005 * pow(salinidad, 4) - 0.000002 * pow(salinidad, 5);

  return salinidadReal;
}

LightSensor::LightSensor(float darknessThreshold, float shadowThreshold, float ambientLightThreshold, float flashlightThreshold) : umbralOscuridad(darknessThreshold), umbralSombra(shadowThreshold), umbralLuzAmbiente(ambientLightThreshold), umbralLinterna(flashlightThreshold) {}

void LightSensor::measure() {
  int16_t lectura = ads.readADC_SingleEnded(0);
  float voltaje = lectura * 0.002; // Convertir la lectura en milivoltios

  // Comparar la lectura con los umbrales
  if (lectura < umbralOscuridad) {
    Serial.println("Nivel de Luz: Oscuridad");
  } else if (lectura < umbralSombra) {
    Serial.println("Nivel de Luz: Sombra");
  } else if (lectura < umbralLuzAmbiente) {
    Serial.println("Nivel de Luz: Luz ambiente");
  } else if (lectura < umbralLinterna) {
    Serial.println("Nivel de Luz: Nivel alto de iluminación");
  } else {
    Serial.println("Nivel de Luz: Muy iluminado");
  }
}
