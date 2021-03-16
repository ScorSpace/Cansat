/*
  GY-271 Compas
*/

// I2C Library
#include <Wire.h>
// QMC5883L Compass Library
#include <QMC5883LCompass.h>

QMC5883LCompass compass;

void setup() {
  // Inicializar el puerto serial
  Serial.begin(9600);
  // Incializar I2C
  Wire.begin();
  // Inicializae compas
  compass.init();
}
