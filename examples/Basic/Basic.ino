/*!
   microfire.co for links to documentation, examples, and libraries
   github.com/u-fire for feature requests, bug reports, and  questions
   questions@microfire.co to get in touch with someone

   Mod-pH hardware version 2, firmware 1
*/

#include <Microfire_Mod-pH.h>
Microfire::Mod_pH::i2c ph;

void setup()
{
  // start Serial and I2C
  Serial.begin(9600);
  Wire.begin();

  // start the sensor
  ph.begin();
}

void loop()
{
  // take a pH measurement
  ph.measurepH(ph.tempC);

  // display the results
  Serial.println((String) ph.pH + " pH");

  delay(1000);
}