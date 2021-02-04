int pinUV = A0;
int ValorUV;

void setup()
{
  pinMode(pinUV, INPUT);
  Serial.begin(9600);
}
 
void loop()
{
   ValorUV = analogRead(pinUV);
  Serial.print("Nivel de UV: ");
  Serial.print(ValorUV);
  Serial.println(" nm");
  delay(1000);
}
