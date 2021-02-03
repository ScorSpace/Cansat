/*****************************************************************************/
//	Función:	 Obtener la dirección geográfica del eje X.
//				Norte = 0º
//				Este = 90º
//				Sur = 180º
//				Oeste = 270º
/*****************************************************************************/
// Biblioteca I2C
#include <Wire.h>
// Biblioteca del compás
#include <HMC5883L.h>

//#include "Streaming.h"

// Guardar compás como variable
HMC5883L compass;
// Grabar errores
int error = 0;

MagnetometerScaled valueOffset;


// Configuración del microcontrolador y compás
void setup() {
    // Serial Port
    Serial.begin(115200);
    delay(2000);
    Serial.println("Comenzando la interfaz I2C");
    Wire.begin(); // Comenzar la interfaz I2C

    Serial.println("Construyendo nuevo HMC5883L");

    Serial.println("Escala a +/- 1.3 Ga");
    error = compass.setScale(1.3);
    if (error != 0) { // Si hay uno, imprimir el error
        Serial.println(compass.getErrorText(error));
    }

    Serial.println("Modo de medida a continuo.");
    error = compass.setMeasurementMode(MEASUREMENT_CONTINUOUS);
    if (error != 0) {
        Serial.println(compass.getErrorText(error));
    }

    compassCalibrate();
}

// calibrar la compensación de x, y, z
void compassCalibrate(void) {
    //Serial << ">>>> calibrar el compás\n";
    Serial.println("calibrar el compás");
    MagnetometerScaled valueMax = {0, 0, 0};
    MagnetometerScaled valueMin = {0, 0, 0};

    // calcular la compensación de x, y, z

    //Serial << "por favor gira el compás" << endl;
    Serial.println("por favor gira el compás");
    int xcount = 0;
    int ycount = 0;
    int zcount = 0;
    boolean xZero = false;
    boolean yZero = false;
    boolean zZero = false;
    MagnetometerScaled value;
    while (xcount < 3 || ycount < 3 || zcount < 3) {
        value = compass.readScaledAxis();
        if ((fabs(value.XAxis) > 600) || (fabs(value.YAxis) > 600) || (fabs(value.ZAxis) > 600)) {
            continue;
        }

        if (valueMin.XAxis > value.XAxis) {
            valueMin.XAxis = value.XAxis;
        } else if (valueMax.XAxis < value.XAxis) {
            valueMax.XAxis = value.XAxis;
        }

        if (valueMin.YAxis > value.YAxis) {
            valueMin.YAxis = value.YAxis;
        } else if (valueMax.YAxis < value.YAxis) {
            valueMax.YAxis = value.YAxis;
        }

        if (valueMin.ZAxis > value.ZAxis) {
            valueMin.ZAxis = value.ZAxis;
        } else if (valueMax.ZAxis < value.ZAxis) {
            valueMax.ZAxis = value.ZAxis;
        }


        if (xZero) {
            if (fabs(value.XAxis) > 50) {
                xZero = false;
                xcount++;
            }
        } else {
            if (fabs(value.XAxis) < 40) {
                xZero = true;
            }
        }

        if (yZero) {
            if (fabs(value.YAxis) > 50) {
                yZero = false;
                ycount++;
            }
        } else {
            if (fabs(value.YAxis) < 40) {
                yZero = true;
            }
        }

        if (zZero) {
            if (fabs(value.ZAxis) > 50) {
                zZero = false;
                zcount++;
            }
        } else {
            if (fabs(value.ZAxis) < 40) {
                zZero = true;
            }
        }

        delay(30);
    }

    valueOffset.XAxis = (valueMax.XAxis + valueMin.XAxis) / 2;
    valueOffset.YAxis = (valueMax.YAxis + valueMin.YAxis) / 2;
    valueOffset.ZAxis = (valueMax.ZAxis + valueMin.ZAxis) / 2;
    #if 0
    Serial << "max: " << valueMax.XAxis << '\t' << valueMax.YAxis << '\t' << valueMax.ZAxis << endl;
    Serial << "min: " << valueMin.XAxis << '\t' << valueMin.YAxis << '\t' << valueMin.ZAxis << endl;
    Serial << "offset: " << valueOffset.XAxis << '\t' << valueOffset.YAxis << '\t' << valueOffset.ZAxis << endl;

    Serial << "<<<<" << endl;
    #endif
    Serial.print("max: ");
    Serial.print(valueMax.XAxis);
    Serial.print(valueMax.YAxis);
    Serial.println(valueMax.ZAxis);
    Serial.print("min: ");
    Serial.print(valueMin.XAxis);
    Serial.print(valueMin.YAxis);
    Serial.println(valueMin.ZAxis);
    Serial.print("offset: ");
    Serial.print(valueOffset.XAxis);
    Serial.print(valueOffset.YAxis);
    Serial.println(valueOffset.ZAxis);
}

void loop() {
    // Leer los datos del compás (no a escala)
    MagnetometerRaw raw = compass.readRawAxis();
    // Leer los datos del compás a escala
    MagnetometerScaled scaled = compass.readScaledAxis();

    scaled.XAxis -= valueOffset.XAxis;
    scaled.YAxis -= valueOffset.YAxis;
    scaled.ZAxis -= valueOffset.ZAxis;

    // Se accede a los datos de esta manera:
    int MilliGauss_OnThe_XAxis = scaled.XAxis;// (or YAxis, or ZAxis)

    // Calcular el ángulo cuando el magnetómetro esté nivelado
    float yxHeading = atan2(scaled.YAxis, scaled.XAxis);
    float zxHeading = atan2(scaled.ZAxis, scaled.XAxis);

    float heading = yxHeading;

    // Ahora el "ángulo de declinación", que es el "error" del campo magnetico en el área
    // Encuentra el tuyo aquí: http://www.magnetic-declination.com/
    // El nuestro es +1° 32', o +0,0267617152 rad, utilizaremos 0,02676
    float declinationAngle = 0,02676;
    heading += declinationAngle;

    // Correct for when signs are reversed.
    if (heading < 0) {
        heading += 2 * PI;
    }

    // Check for wrap due to addition of declination.
    if (heading > 2 * PI) {
        heading -= 2 * PI;
    }

    // Convert radians to degrees for readability.
    float headingDegrees = heading * 180 / M_PI;

    float yxHeadingDegrees = yxHeading * 180 / M_PI;
    float zxHeadingDegrees = zxHeading * 180 / M_PI;

    // Output the data via the serial port.
    // Output(raw, scaled, heading, headingDegrees);

    //  Serial << scaled.XAxis << ' ' << scaled.YAxis << ' ' << scaled.ZAxis << endl;
    //  Serial << "arctan y/x: " << yxHeadingDegrees << " \tarctan z/x: " << zxHeadingDegrees << endl;

    Serial.print(scaled.XAxis);
    Serial.print(scaled.YAxis);
    Serial.println(scaled.ZAxis);

    Serial.print("arctan y/x: ");
    Serial.print(yxHeadingDegrees);
    Serial.print("arctan z/x: ");
    Serial.print(zxHeadingDegrees);

    // Normally we would delay the application by 66ms to allow the loop
    // to run at 15Hz (default bandwidth for the HMC5883L).
    // However since we have a long serial out (104ms at 9600) we will let
    // it run at its natural speed.
    delay(1000);//of course it can be delayed longer.
}

// Output the data down the serial port.
void Output(MagnetometerRaw raw, MagnetometerScaled scaled, float heading, float headingDegrees) {
    Serial.print("Raw:\t");
    Serial.print(raw.XAxis);
    Serial.print("   ");
    Serial.print(raw.YAxis);
    Serial.print("   ");
    Serial.print(raw.ZAxis);
    Serial.print("   \tScaled:\t");

    Serial.print(scaled.XAxis);
    Serial.print("   ");
    Serial.print(scaled.YAxis);
    Serial.print("   ");
    Serial.print(scaled.ZAxis);

    Serial.print("   \tHeading:\t");
    Serial.print(heading);
    Serial.print(" Radians   \t");
    Serial.print(headingDegrees);
    Serial.println(" Degrees   \t");
}
