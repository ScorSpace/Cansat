/*
  GY-271 Compas
*/

// I2C Libreria
#include <Wire.h>
// QMC5883L Libreria Compas
#include <QMC5883LCompass.h>

QMC5883LCompass compass;

void setup() {
  // Inicializar el puerto serial
  Serial.begin(9600);
  // Inicializar I2C
  Wire.begin();
  // Inicializar el compas
  compass.init();
}

void loop() {
  int x, y, z;

  // Leer los valores del compas
  compass.read();

  x = compass.getX();
  y = compass.getY();
  z = compass.getZ();

  Serial.print("X: ");
  Serial.print(x);
  Serial.print("   Y: ");
  Serial.print(y);
  Serial.print("   Z: ");
  Serial.println(z);

  delay(300);
}
