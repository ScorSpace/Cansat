int pinUV = A0;
int ValorUV;

// NOTA: Mirar tabla de indices en el canal sensor-guva-s12sd en discord

void setup()
{
  pinMode(pinUV, INPUT);
  Serial.begin(9600);
}
 
void loop()
{
   ValorUV = analogRead(pinUV);
  Serial.print("Índice de UV: ");
  Serial.println(ValorUV);
  delay(1000);
}
