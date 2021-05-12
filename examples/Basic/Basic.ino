/*!
   ufire.dev for links to documentation, examples, and libraries
   github.com/u-fire for feature requests, bug reports, and  questions
   questions@ufire.co to get in touch with someone

   Mod-pH hardware version 1, firmware 1
*/

#include <uFire_Mod-pH.h>
uFire::Mod_pH::i2c ph;

void setup()
{
  Serial.begin(9600);
  Wire.begin();

  // start the sensor after Wire.begin()
  ph.begin();
}

void loop()
{
  // get the temperature of the solution
  ph.measureTemp();

  // take a pH measurement
  ph.measurepH(ph.tempC);

  // display the results
  Serial.println((String) ph.pH + " pH");
  Serial.println((String) ph.tempC + " °C");
  Serial.println();

  delay(1000);
}