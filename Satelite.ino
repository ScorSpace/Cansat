/******************************************************************************************************/
// Funcion:   Juntar el codigo de todos los sensores en un solo lugar
/******************************************************************************************************/

#include <SPI.h>    // incluye libreria interfaz SPI
#include <SD.h>     // incluye libreria para tarjetas SD

#define SSpin 10    // Slave Select en pin digital 10

#include "Seeed_BME280.h"
#include <Wire.h>
#include "Adafruit_CCS811.h"

BME280 bme280;
Adafruit_CCS811 ccs;
const int buzzer = 7; // buzzer al pin digital I/O 7
const int led = 6; // LED al pin digital I/O 6
int pinUV = A0;
int ValorUV;

File archivo;     // objeto archivo del tipo File

void setup() {
  Serial.begin(9600);       // inicializa monitor serie a 9600 bps
  Serial.println("Inicializando tarjeta ...");  // texto en ventana de monitor
  if (!SD.begin(SSpin)) {     // inicializacion de tarjeta SD
    Serial.println("fallo en inicializacion !");// si falla se muestra texto correspondiente y
    return;         // se sale del setup() para finalizar el programa
  }
  
  Serial.println("inicializacion correcta");  // texto de inicializacion correcta

  pinMode(buzzer, OUTPUT);
  tone(buzzer, 1000); // Enviar una señal de 1kHz de frecuencia
  
  pinMode(led, OUTPUT);
  
  pinMode(pinUV, INPUT); // Preparar para leer del sensor GUVA-S12SD
  
  if(!bme280.init()){
    Serial.println("¡No se ha encontrado el sensor BME280! Por favor comprueba el cableado.");
    while(1);
  }
  
  if(!ccs.begin()){
    Serial.println("¡No se ha encontrado el sensor CCS811! Por favor comprueba el cableado.");
    while(1);
  }
  
  // Esperar a que el sensor este disponible
  while(!ccs.available());
}

void loop() {

  ValorUV = analogRead(pinUV);
  Serial.print("Indice de UV: ");
  Serial.println(ValorUV);
  
  archivo = SD.open("sat.txt", FILE_WRITE);  // apertura para lectura/escritura de archivo sat.txt
  
  if (archivo) {
    float pressure;

    //temperatura
    Serial.println("Misión Primaria");
    Serial.print("Temperatura: ");
    Serial.print(bme280.getTemperature());
    Serial.println("Cº");//The unit for  Celsius because original arduino don't support special symbols

    //presión atmosférica
    Serial.print("Presión: ");
    Serial.print(pressure = bme280.getPressure());
    Serial.println("Pa");

    //altitud
    Serial.print("Altitud: ");
    Serial.print(bme280.calcAltitude(pressure) + 110);
    Serial.println("m");

    //humedad
    Serial.print("Humedad: ");
    Serial.print(bme280.getHumidity());
    Serial.println("%");

    Serial.println(" ");
    
    //UV
    ValorUV = analogRead(pinUV);
    Serial.println("Misión Secundaria 1");
    Serial.print("Índice de UV: ");
    Serial.println(ValorUV);

    Serial.println(" ");
    
    if(ccs.available()){
      if(!ccs.readData()){
        Serial.println("Misión Secundaria 2");
        Serial.print("CO2: ");
        Serial.print(ccs.geteCO2());
        Serial.print("ppm, TVOC: ");
        Serial.println(ccs.getTVOC());
        Serial.println(" ");
        Serial.println("--------------");
      }

    
    archivo = SD.open("sat.txt");    // apertura de archivo sat.txt
    if (archivo) {
      Serial.println("Contenido de sat.txt:"); // texto en monitor serie
      while (archivo.available()) {   // mientras exista contenido en el archivo
      Serial.write(archivo.read());     // lectura de a un caracter por vez
    }
    archivo.close();        // cierre de archivo
  } else {
    Serial.println("Error en la apertura de sat.txt");// texto de falla en apertura de archivo
  }
      
  archivo = SD.open("sat.txt");    // apertura de archivo sat.txt
  if (archivo) {
    Serial.println("Contenido de sat.txt:"); // texto en monitor serie
    while (archivo.available()) {   // mientras exista contenido en el archivo
    Serial.write(archivo.read());     // lectura de un caracter por vez
  }
    archivo.close();        // cierre de archivo
  } else {
    Serial.println("Error en la apertura de sat.txt");// texto de falla en apertura de archivo
  }
  
  delay(500);
}
