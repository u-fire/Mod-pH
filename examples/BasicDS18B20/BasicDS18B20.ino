/*!
   microfire.co for links to documentation, examples, and libraries
   github.com/u-fire for feature requests, bug reports, and  questions
   questions@microfire.co to get in touch with someone

   Mod-pH hardware version 2, firmware 1
*/
#include <Microfire_Mod-pH.h>
#include <OneWire.h>              // Click here to install the library: http://librarymanager/All#OneWire
#include <DallasTemperature.h>    // Click here to install the library: http://librarymanager/All#DallasTemperature

// what pin is the temperature sensor's signal pin connected to?
#define ONE_WIRE_BUS 2

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature temp(&oneWire);
Microfire::Mod_pH::i2c ph;

void setup()
{
  // start Serial and I2C
  Serial.begin(9600);
  Wire.begin();

  // start the OneWire and Dallas library
  temp.begin();

  // start the sensor
  ph.begin();
}

void loop()
{
  // take a temperature measurement
  temp.requestTemperatures();
  float temp_c = temp.getTempCByIndex(0);

  // take a pH measurement
  ph.measurepH(temp_c);

  // display the results
  Serial.println((String) ph.pH + " pH @ " + temp_c + "°C");
    
  delay(1000);
}