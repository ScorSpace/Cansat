/******************************************************************************************************/
// Funcion:   Mediante un modulo lector de memoria MicroSD conectado mediante interfaz SPI
//           crearemos un archivo de texto de prueba y grabaremos una linea en el mismo, 
//           luego haremos la apertura para su lectura y asi verificar el correcto funcionamiemto.
/******************************************************************************************************/

#include <SPI.h>    // incluye libreria interfaz SPI
#include <SD.h>     // incluye libreria para tarjetas SD

#define SSpin 10    // Slave Select en pin digital 10

#include "Adafruit_CCS811.h"

Adafruit_CCS811 ccs;

File archivo;     // objeto archivo del tipo File

void setup() {
  Serial.begin(9600);       // inicializa monitor serie a 9600 bps
  Serial.println("Inicializando tarjeta ...");  // texto en ventana de monitor
  if (!SD.begin(SSpin)) {     // inicializacion de tarjeta SD
    Serial.println("fallo en inicializacion !");// si falla se muestra texto correspondiente y
    return;         // se sale del setup() para finalizar el programa
  }
  
  Serial.println("inicializacion correcta");  // texto de inicializacion correcta

  if(!ccs.begin()){
    Serial.println("¡No se ha encontrado el sensor CCS811! Por favor comprueba el cableado.");
    while(1);
  }
  
  // Esperar a que el sensor este disponible
  while(!ccs.available());
}

void loop() {
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
