include <Wire.h>
#include "Adafruit_VEML6070.h"
 
Adafruit_VEML6070 uv = Adafruit_VEML6070();
 
void setup()
{
Serial.begin(9600);
Serial.println("VEML6070 Test");
uv.begin(VEML6070_1_T); // pasar la constante de tiempo de integración
}
 
void loop()
{
Serial.print("Nivel de luz UV: ");
Serial.println(uv.readUV());
delay(1000);
}
