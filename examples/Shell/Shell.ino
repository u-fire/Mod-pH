/*
   This allows you to run various functions in a command-line like interface.
   Enter:
   `config` to see the configuration of the device.
   'reset' to reset all the configuration stored in the device

   Measure pH:
     ph <solution temp [25]>
      `ph` - pH measurement with default parameters
      `ph t` - pH measurement using result of measureTemp() for solution temperature

   Single Point Calibration:
    sin <calibration solution in pH> <solution temp [25]>
      `sin 4.0 t` - Calibrate at 4.0 pH using measureTemp() for solution temp

   Dual Point Calibration:
    low <calibration solution in pH> <solution temp [25]>
      `low 4.0 t` - Calibrate at 4.0 pH using measureTemp() for solution temp

    high <calibration solution in pH> <solution temp [25]>
      `high 10.0 t` - Calibrate at 10.0 pH using measureTemp() for solution temp

   Triple Point Calibration:
    low <calibration solution in pH> <solution temp [25]>
      `low 4.0 t` - Calibrate at 4.0 pH using measureTemp() for solution temp
    mid <calibration solution in pH> <solution temp [25]>
      `mid 7.0 t` - Calibrate at 7.0 pH using measureTemp() for solution temp
    high <calibration solution in pH> <solution temp [25]>
      `high 10.0 t` - Calibrate at 10.0 pH using measureTemp() for solution temp

   Measure Temperature:
    temp

   Change the I2C address:
    i2c 0x0F
*/

#include <Arduino.h>
#include <Wire.h>
#include "uFire_Mod-pH.h"

uFire::Mod_pH::i2c ph;

String buffer, cmd, p1, p2;
float temp_c;
const int fw_compatible = 1;
const int hw_compatible = 1;

void config()
{
  ph.getDeviceInfo();
  Serial.println((String) "uFire Mod-pH Sensor: " + (ph.connected() ? "connected" : "*disconnected*"));
  if (!ph.connected()) return;
  if ((ph.fwVersion != fw_compatible) || (ph.hwVersion != hw_compatible))
  {
    Serial.println("*This version of shell was designed for a different hardware revision or firmware version*");
  }
  Serial.println("calibration:");
  Serial.print("    low reference / read: ");
  if (ph.calibrationLowReference == ph.calibrationLowReference ? Serial.print(ph.calibrationLowReference, 3) : Serial.print("-"));
  Serial.print(" /  ");
  if (ph.calibrationLowReading == ph.calibrationLowReading ? Serial.println(ph.calibrationLowReading, 3) : Serial.println("-"));
  Serial.print("    middle reference / read: ");
  if (ph.calibrationMidReference == ph.calibrationMidReference ? Serial.print(ph.calibrationMidReference, 3) : Serial.print("-"));
  Serial.print(" / ");
  if (ph.calibrationMidReading == ph.calibrationMidReading ? Serial.println(ph.calibrationMidReading, 3) : Serial.println("-"));
  Serial.print("    high reference / read: ");
  if (ph.calibrationHighReference == ph.calibrationHighReference ? Serial.print(ph.calibrationHighReference, 3) : Serial.print("-"));
  Serial.print(" / ");
  if (ph.calibrationHighReading == ph.calibrationHighReading ? Serial.println(ph.calibrationHighReading, 3) : Serial.println("-"));
  Serial.print("    single point: ");
  if (ph.calibrationSingleOffset == ph.calibrationSingleOffset ? Serial.println(ph.calibrationSingleOffset, 3) : Serial.println("-"));
  Serial.print("    calibration temperature: ");
  if (ph.calibrationTemperature == ph.calibrationTemperature ? Serial.println(ph.calibrationTemperature, 3) : Serial.println("-"));
  Serial.print("hardware:firmware version: ");
  Serial.print(ph.hwVersion);
  Serial.print(":");
  Serial.println(ph.fwVersion);
}

void config_reset()
{
  ph.reset();
  config();
}

void temperature()
{
  ph.measureTemp();
  switch (ph.status)
  {
    case ph.STATUS_SYSTEM_ERROR:
      Serial.println("Error: no temperature sensor connected");
      break;
    case ph.STATUS_NO_ERROR:
      Serial.print("C|F: ");
      Serial.print(ph.tempC);
      Serial.print(" | ");
      Serial.println(ph.tempF);
      break;
  }
}

void i2c()
{
  uint8_t i2c_address;
  char * p;

  if (p1.length())
  {
    i2c_address = strtoul(p1.c_str(), & p, 16);
    Serial.println(i2c_address);
    if (i2c_address == 0)
    {
      Serial.println("Error: malformed I2C address");
    }
    else if ((i2c_address <= 0x07) || (i2c_address > 0x7f))
    {
      Serial.println("Error: I2C address not in valid range");
    }
    else
    {
      ph.setI2CAddress(i2c_address);
    }
  }
}

void measure_ph()
{
  if (p1.length() ? temp_c = p1.toFloat() : temp_c = 25.0);
  while (Serial.available() == 0)
  {
    if (p1 == "t")
    {
      temp_c = ph.measureTemp();
    }
    ph.measurepH(temp_c, true);

    switch (ph.status)
    {
      case ph.STATUS_SYSTEM_ERROR:
        Serial.println("Error: Module not functioning properly");
        break;
      case ph.STATUS_OUTSIDE_LOWER:
        Serial.println("Error: Measurement outside lower range");
        break;
      case ph.STATUS_OUTSIDE_UPPER:
        Serial.println("Error: Measurement outside upper range");
        break;
      case ph.STATUS_NO_ERROR:
          Serial.print(ph.pH, 3);
          Serial.println((String)" pH @ " + temp_c + "°C");
        break;
    }
    delay(1000);
  }
}

void low()
{
  if (p2.length() ? temp_c = p2.toFloat() : temp_c = 25.0);

  if (p2 == "t")
  {
    temp_c = ph.measureTemp();
  }
  ph.calibrateLow(p1.toFloat(), temp_c, true);
  switch (ph.status)
  {
    case ph.STATUS_SYSTEM_ERROR:
      Serial.println("Error: Module not functioning properly");
      break;
    case ph.STATUS_OUTSIDE_LOWER:
      Serial.println("Error: Measurement outside lower range");
      break;
    case ph.STATUS_OUTSIDE_UPPER:
      Serial.println("Error: Measurement outside upper range");
      break;
    case ph.STATUS_NO_ERROR:
      config();
      break;
  }
}

void high()
{
  if (p2.length() ? temp_c = p2.toFloat() : temp_c = 25.0);

  if (p2 == "t")
  {
    temp_c = ph.measureTemp();
  }
  ph.calibrateHigh(p1.toFloat(), temp_c, true);
  switch (ph.status)
  {
    case ph.STATUS_SYSTEM_ERROR:
      Serial.println("Error: Module not functioning properly");
      break;
    case ph.STATUS_OUTSIDE_LOWER:
      Serial.println("Error: Measurement outside lower range");
      break;
    case ph.STATUS_OUTSIDE_UPPER:
      Serial.println("Error: Measurement outside upper range");
      break;
    case ph.STATUS_NO_ERROR:
      config();
      break;
  }
}

void mid()
{
  if (p2.length() ? temp_c = p2.toFloat() : temp_c = 25.0);

  if (p2 == "t")
  {
    temp_c = ph.measureTemp();
  }
  ph.calibrateMid(p1.toFloat(), temp_c, true);
  switch (ph.status)
  {
    case ph.STATUS_SYSTEM_ERROR:
      Serial.println("Error: Module not functioning properly");
      break;
    case ph.STATUS_OUTSIDE_LOWER:
      Serial.println("Error: Measurement outside lower range");
      break;
    case ph.STATUS_OUTSIDE_UPPER:
      Serial.println("Error: Measurement outside upper range");
      break;
    case ph.STATUS_NO_ERROR:
      config();
      break;
  }
}

void single()
{
  if (p2.length() ? temp_c = p2.toFloat() : temp_c = 25.0);

  if (p2 == "t")
  {
    temp_c = ph.measureTemp();
  }
  ph.calibrateSingle(p1.toFloat(), temp_c, true);
  switch (ph.status)
  {
    case ph.STATUS_SYSTEM_ERROR:
      Serial.println("Error: Module not functioning properly");
      break;
    case ph.STATUS_OUTSIDE_LOWER:
      Serial.println("Error: Measurement outside lower range");
      break;
    case ph.STATUS_OUTSIDE_UPPER:
      Serial.println("Error: Measurement outside upper range");
      break;
    case ph.STATUS_NO_ERROR:
      config();
      break;
  }
}

void help()
{
  Serial.println(F("List of available commands, parameters separated by spaces, `low 4.0 22.1`, bracketed numbers are defaults if none provided"));
  Serial.println(F("config -or- c : no parameters : Displays all configuration and system information."));
  Serial.println(F("ph            : temp_C[25.0] : Starts a pH measurement."));
  Serial.println(F("high          : calibration_pH, temp_C[25.0] : High-point calibration."));
  Serial.println(F("i2c           : I2C_address : Changes the module's I2C address."));
  Serial.println(F("low           : calibration_pH, temp_C[25.0] : Low-point calibration."));
  Serial.println(F("mid           : calibration_pH, temp_C[25.0] : Mid-point calibration."));
  Serial.println(F("reset -or- r  : no parameters : Returns all configuration information to default values."));
  Serial.println(F("sin           : calibration_pH, temp_C[25.0] : Single-point calibration."));
  Serial.println(F("temp -or- t   : no parameters : Starts a temperature measurement"));
}

void cmd_run()
{
  if ((cmd == "config") || (cmd == "c")) config();
  if ((cmd == "reset") || (cmd == "r")) config_reset();
  if ((cmd == "temp") || (cmd == "t")) temperature();
  if (cmd == "mid") mid();
  if (cmd == "low") low();
  if (cmd == "high") high();
  if (cmd == "sin") single();
  if (cmd == "i2c") i2c();
  if (cmd == "ph") measure_ph();
  if ((cmd == "help") || (cmd == "h")) help();
}

void cli_process()
{
  while (Serial.available())
  {
    char c = Serial.read();

    switch (c)
    {
    case '\n':
      Serial.println();
      cmd = buffer.substring(0, buffer.indexOf(" ", 0));
      cmd.trim();
      buffer.remove(0, buffer.indexOf(" ", 0));
      buffer.trim();
      p1 = buffer.substring(0, buffer.indexOf(" ", 0));
      p1.trim();
      buffer.remove(0, buffer.indexOf(" ", 0));
      buffer.trim();
      p2 = buffer.substring(0, buffer.indexOf(" ", 0));
      p2.trim();
      cmd_run();

      Serial.print("> ");
      buffer = "";
      break;

    case '\b': // backspace
      buffer.remove(buffer.length() - 1);
      Serial.print("\b \b");
      break;

    default: // everything else
      buffer += c;
      Serial.print(c);
    }
  }
}

void setup()
{
  Wire.begin();
  ph.begin();
  Serial.begin(9600);
  Serial.println("Type `help` for a list of commands");
  config();
  Serial.print("> ");
}

void loop()
{
  cli_process();
}
