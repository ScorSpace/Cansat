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
int pinUV = A0;
int ValorUV;

File archivo;     // objeto archivo del tipo File
File archivo_1;

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
      
  archivo_1 = SD.open("bme280.txt", FILE_WRITE);  // apertura para lectura/escritura de archivo prueba.txt
        
  if (archivo_1) {
    float pressure;
  
    //temperatura
    archivo_1.print("Temperatura: ");
    archivo_1.print(bme280.getTemperature());
    archivo_1.println("C");
        
    //presión atmosférica
    archivo_1.print("Presion: ");
    archivo_1.print(pressure = bme280.getPressure());
    archivo_1.println("Pa");
      
    //altitud
    archivo_1.print("Altitud: ");
    archivo_1.print(bme280.calcAltitude(pressure) + 75);
    archivo_1.println("m");
      
    //humedad
    archivo_1.print("Humedad: ");
    archivo_1.print(bme280.getHumidity());
    archivo_1.println("%");
    Serial.println("Escribiendo en archivo bme280.txt..."); // texto en monitor serie
    archivo_1.close();        // cierre del archivo
    Serial.println("Escritura correcta."); // texto de escritura correcta en monitor serie
    } else {
      Serial.println("Error en apertura de bme280.txt");  // texto de falla en apertura de archivo
    }
    
    archivo_1 = SD.open("bme280.txt");    // apertura de archivo bme280.txt
    if (archivo_1) {
      Serial.println("Contenido de bme280.txt:"); // texto en monitor serie
      while (archivo_1.available()) {   // mientras exista contenido en el archivo
      Serial.write(archivo_1.read());     // lectura de a un caracter por vez
    }
    archivo_1.close();        // cierre de archivo
  } else {
    Serial.println("Error en la apertura de bme280.txt");// texto de falla en apertura de archivo
  }
  
  if(ccs.available()){
    if(!ccs.readData()){
      archivo = SD.open("ccs811.txt", FILE_WRITE);  // apertura para lectura/escritura de archivo prueba.txt
        
      if (archivo) {
        archivo.print("CO2: ");
        archivo.print(ccs.geteCO2());
        archivo.print("ppm, TVOC: ");
        archivo.println(ccs.getTVOC());
        Serial.println("Escribiendo en archivo ccs811.txt..."); // texto en monitor serie
        archivo.close();        // cierre del archivo
        Serial.println("Escritura correcta."); // texto de escritura correcta en monitor serie
      } else {
        Serial.println("Error en apertura de ccs811.txt");  // texto de falla en apertura de archivo
      }
    
      archivo = SD.open("ccs811.txt");    // apertura de archivo ccs811.txt
      if (archivo) {
        Serial.println("Contenido de ccs811.txt:"); // texto en monitor serie
        while (archivo.available()) {   // mientras exista contenido en el archivo
          Serial.write(archivo.read());     // lectura de a un caracter por vez
        }
        archivo.close();        // cierre de archivo
      } else {
        Serial.println("Error en la apertura de ccs811.txt");// texto de falla en apertura de archivo
      }
    }
    else{
      Serial.println("¡ERROR!");
      while(1);
    }
  }
  
  delay(500);
}
