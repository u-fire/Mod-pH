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
  if (ph.begin() != true)
  {
    Serial.println("Error: the sensor isn't connected");
    while (1);
  }
}

void loop()
{
  // take a pH measurement
  ph.measurepH();
  switch (ph.status)
  {
    case ph.STATUS_SYSTEM_ERROR:
      Serial.println("  Error: Module not functioning properly");
      break;
    case ph.STATUS_OUTSIDE_LOWER:
      Serial.println("  Error: Measurement outside lower range");
      break;
    case ph.STATUS_OUTSIDE_UPPER:
      Serial.println("  Error: Measurement outside upper range");
      break;
    case ph.STATUS_NO_ERROR:
      Serial.print(ph.pH);
      Serial.println((String)" pH");
      break;
  }

  delay(1000);
}