#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C
//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI

unsigned long delayTime;

void setup() {
  Serial.begin(9600);
  Serial.println(F("BME280 test"));

  bool status;
  
  // configuraciones por defecto
  // (tambien puedes oiber un objeto de la librería Wire como &Wire2)
  status = bme.begin();  
  if (!status) {
    Serial.println("No se ha podido encontrar un sensor BME280 válido, comprueba el cableado!");
    while (1);
  }
  
  Serial.println("-- Default Test --");
  delayTime = 1000;

  Serial.println();
}


void loop() { 
  printValues();
  delay(delayTime);
}


void printValues() {
  Serial.print("Temperatura = ");
  Serial.print(bme.readTemperature());
  Serial.println(" *C");
  
  Serial.print("Presión = ");
  Serial.print(bme.readPressure() / 100.0F);
  Serial.println(" hPa");
  
  Serial.print("Altitud aprox. = ");
  Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.println(" m");
  
  Serial.print("Humedad = ");
  Serial.print(bme.readHumidity());
  Serial.println(" %");
  
  Serial.println();
}
