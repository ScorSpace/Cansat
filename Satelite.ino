#include "Seeed_BME280.h"
#include "Adafruit_CCS811.h"
#include <Wire.h>
#include <SPI.h>    // incluye libreria interfaz SPI
#include <SD.h>     // incluye libreria para tarjetas SD


#define BUTTON_PIN 2 // boton
#define SSpin 10    // Slave Select en pin digital 10

File archivo;     // objeto archivo del tipo File

int pinUV = A0;
int ValorUV;

int buzzer = 7;
int led = 6;

BME280 bme280;
Adafruit_CCS811 ccs;

void setup(){
  pinMode(buzzer, OUTPUT);
  pinMode(led, OUTPUT);
  
  pinMode(BUTTON_PIN, INPUT);
  digitalWrite(BUTTON_PIN, HIGH); // connect internal pull-up
  
  tone(buzzer, 5000);
  digitalWrite(led, HIGH);
  
  Serial.println("Inicializando tarjeta ...");  // texto en ventana de monitor
  if (!SD.begin(SSpin)) {     // inicializacion de tarjeta SD
    Serial.println("fallo en inicializacion !");// si falla se muestra texto correspondiente y
    return;         // se sale del setup() para finalizar el programa
  }
    
  pinMode(pinUV, INPUT);
  Serial.begin(9600);
  if(!bme280.init()){
    Serial.println("Device error!");
  }

  if(!ccs.begin()){
    Serial.println("¡No se ha encontrado el sensor! Por favot comprueba el cableado.");
    while(1);
  }

  // Esperar a que el sensor este disponible
  while(!ccs.available());
}

void loop(){

  float pressure;

  //get and print temperatures
  Serial.println("Misión Primaria");
  Serial.print("Temperatura: ");
  Serial.print(bme280.getTemperature());
  Serial.println("Cº");//The unit for  Celsius because original arduino don't support special symbols

  //get and print atmospheric pressure data
  Serial.print("Presión: ");
  Serial.print(pressure = bme280.getPressure());
  Serial.println("Pa");

  //get and print altitude data
  Serial.print("Altitud: ");
  Serial.print(bme280.calcAltitude(pressure) + 110);
  Serial.println("m");

  //get and print humidity data
  Serial.print("Humedad: ");
  Serial.print(bme280.getHumidity());
  Serial.println("%");

  Serial.println(" ");

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
    else{
      Serial.println("¡ERROR!");
      while(1);
    }
  }

  archivo = SD.open("scorspace_cansat.txt", FILE_WRITE);  // apertura para lectura/escritura de archivo prueba.txt
  
  //get and print temperatures
  archivo.println("Misión Primaria");
  archivo.print("Temperatura: ");
  archivo.print(bme280.getTemperature());
  archivo.println("Cº");//The unit for  Celsius because original arduino don't support special symbols

  //get and print atmospheric pressure data
  archivo.print("Presión: ");
  archivo.print(pressure = bme280.getPressure());
  archivo.println("Pa");

  //get and print altitude data
  archivo.print("Altitud: ");
  archivo.print(bme280.calcAltitude(pressure) + 110);
  archivo.println("m");

  //get and print humidity data
  archivo.print("Humedad: ");
  archivo.print(bme280.getHumidity());
  archivo.println("%");

  archivo.println(" ");

  ValorUV = analogRead(pinUV);
  archivo.println("Misión Secundaria 1");
  archivo.print("Índice de UV: ");
  archivo.println(ValorUV);

  archivo.println(" ");

  if(ccs.available()){
    if(!ccs.readData()){
      archivo.println("Misión Secundaria 2");
      archivo.print("CO2: ");
      archivo.print(ccs.geteCO2());
      archivo.print("ppm, TVOC: ");
      archivo.println(ccs.getTVOC());
      archivo.println(" ");
      archivo.println("--------------");
    }
    else{
      archivo.println("¡ERROR!");
      while(1);
    }
  }
  archivo.close();        // cierre del archivo

  delay(1000);
}
