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

  if (ph.begin() != true)
  {
    Serial.println("Error: the sensor isn't connected");
    while (1);
  }
}

void loop()
{
  // get the temperature of the solution
  ph.measureTemp();

  // check for errors
  if (ph.status)
  {
    Serial.println("Error:");
    switch (ph.status)
    {
      case ph.STATUS_SYSTEM_ERROR:
        Serial.println("  temperature sensor not connected");
        break;
    }
  }

  // take a pH measurement
  ph.measurepH(ph.tempC);
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
      Serial.print(ph.pH, 3);
      Serial.println((String)" pH @ " + ph.tempC + "°C");
      Serial.println();
      break;
  }

  delay(1000);
}