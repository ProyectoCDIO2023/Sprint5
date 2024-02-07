#include <ESP8266WiFi.h>
#include "Sensors.h"

const float umbralOscuridad = 65;    // Voltaje para la oscuridad
const float umbralSombra = 80;       // Voltaje para la sombra
const float umbralLuzAmbiente = 117;  // Voltaje para luz ambiente
const float umbralLinterna = 700;    // Voltaje con linterna del móvil
const int power_pin = 5;

pHSensor pH;
HumidityTemperatureSensor humidityTemperature;
SalinitySensor salinity(power_pin);
LightSensor lightSensor(umbralOscuridad, umbralSombra, umbralLuzAmbiente, umbralLinterna);

void setup() {
  Serial.begin(9600);
  Serial.println("Inicializando...");

  pH.begin();
}

void loop() {
  pH.measure();
  pH.printResults();

  int humidityValueMojado = humidityTemperature.readHumidity();
  int temperature = humidityTemperature.readTemperature();
  Serial.print("Humedad (mojado): ");
  Serial.print(humidityValueMojado);
  Serial.println("%");
  Serial.print("Temperatura: ");
  Serial.print(temperature);
  Serial.println(" °C");

  salinity.measure();

  lightSensor.measure();

  delay(1000); // Pausa para evitar que el loop se ejecute demasiado rápido
}
