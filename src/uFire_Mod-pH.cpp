#include "uFire_Mod-pH.h"

float uFire::Mod_pH::i2c::_pH = -1;
float uFire::Mod_pH::i2c::_tempC = -1;
float uFire::Mod_pH::i2c::_tempF = -1;
float uFire::Mod_pH::i2c::_mV = -1;
float uFire::Mod_pH::i2c::_calibrationLowReading = -1;
float uFire::Mod_pH::i2c::_calibrationLowReference = -1;
float uFire::Mod_pH::i2c::_calibrationMidReading = -1;
float uFire::Mod_pH::i2c::_calibrationMidReference = -1;
float uFire::Mod_pH::i2c::_calibrationHighReading = -1;
float uFire::Mod_pH::i2c::_calibrationHighReference = -1;
float uFire::Mod_pH::i2c::_calibrationSingleOffset = -1;
float uFire::Mod_pH::i2c::_calibrationTemperature = -1;
int uFire::Mod_pH::i2c::_hwVersion = -1;
int uFire::Mod_pH::i2c::_fwVersion = -1;
int uFire::Mod_pH::i2c::_status = -1;

namespace uFire
{
  namespace Mod_pH
  {
    // Initializes library
    bool i2c::begin(TwoWire &wirePort, uint8_t address)
    {
      _address = address;
      _i2cPort = &wirePort;

      return connected();
    }

    // Performs a single-point calibration.
    float i2c::calibrateSingle(float solution_pH, float tempC, bool blocking)
    {
      _write_4_bytes(PH_REGISTER, solution_pH);
      _write_4_bytes(CALIBRATE_TEMPERATURE_REGISTER, tempC);
      _send_command(CALIBRATE_SINGLE_TASK);
      if (blocking)
      {
        delay(PH_PH_MEASUREMENT_TIME);
      }

      getDeviceInfo();
      return status;
    }

    // Performs a high-point calibration. Used in conjuction with calibrateMid or calibrateLow.
    float i2c::calibrateHigh(float solution_pH, float tempC, bool blocking)
    {
      _write_4_bytes(PH_REGISTER, solution_pH);
      _write_4_bytes(CALIBRATE_TEMPERATURE_REGISTER, tempC);
      _send_command(CALIBRATE_HIGH_TASK);
      if (blocking)
        delay(PH_PH_MEASUREMENT_TIME);

      getDeviceInfo();
      return status;
    }

    // Performs a low-point calibration. Used in conjuction with calibrateMid or calibrateHigh.
    float i2c::calibrateLow(float solution_pH, float tempC, bool blocking)
    {
      _write_4_bytes(PH_REGISTER, solution_pH);
      _write_4_bytes(CALIBRATE_TEMPERATURE_REGISTER, tempC);
      _send_command(CALIBRATE_LOW_TASK);
      if (blocking)
        delay(PH_PH_MEASUREMENT_TIME);

      getDeviceInfo();
      return status;
    }

    // Performs a mid-point calibration. Used in conjuction with calibrateLow or calibrateHigh.
    float i2c::calibrateMid(float solution_pH, float tempC, bool blocking)
    {
      _write_4_bytes(PH_REGISTER, solution_pH);
      _write_4_bytes(CALIBRATE_TEMPERATURE_REGISTER, tempC);
      _send_command(CALIBRATE_MID_TASK);
      if (blocking)
        delay(PH_PH_MEASUREMENT_TIME);

      getDeviceInfo();
      return status;
    }

    // Returns true or false if the sensor is connected.
    bool i2c::connected()
    {
      Wire.beginTransmission(_address);
      uint8_t retval = Wire.endTransmission();

      if (retval)
      {
        return false;
      }
      else
      {
        return true;
      }
    }

    // Retrieves all the system information.
    void i2c::getDeviceInfo()
    {
      _calibrationLowReading = _read_4_bytes(CALIBRATE_READLOW_REGISTER);
      _calibrationLowReference = _read_4_bytes(CALIBRATE_REFLOW_REGISTER);
      _calibrationMidReading = _read_4_bytes(CALIBRATE_READMID_REGISTER);
      _calibrationMidReference = _read_4_bytes(CALIBRATE_REFMID_REGISTER);
      _calibrationHighReading = _read_4_bytes(CALIBRATE_READHIGH_REGISTER);
      _calibrationHighReference = _read_4_bytes(CALIBRATE_REFHIGH_REGISTER);
      _calibrationSingleOffset = _read_4_bytes(CALIBRATE_SINGLE_OFFSET_REGISTER);
      _calibrationTemperature = _read_4_bytes(CALIBRATE_TEMPERATURE_REGISTER);
      _hwVersion = _read_byte(HW_VERSION_REGISTER);
      _fwVersion = _read_byte(FW_VERSION_REGISTER);
      _status = _read_byte(STATUS_REGISTER);
    }

    // Measures pH.
    float i2c::measurepH(float tempC, bool blocking)
    {
      _write_4_bytes(TEMP_C_REGISTER, tempC);
      _send_command(MEASURE_PH_TASK);
      if (blocking)
        delay(PH_PH_MEASUREMENT_TIME);

      _updateRegisters();

      return pH;
    }

    // Measures temperature using an optionally connected DS18B20 sensor
    float i2c::measureTemp(bool blocking)
    {
      _send_command(MEASURE_TEMP_TASK);
      if (blocking)
        delay(PH_TEMP_MEASURE_TIME);

      _updateRegisters();

      return tempC;
    }

    // Resets all system calibration information.
    void i2c::reset()
    {
      _write_4_bytes(CALIBRATE_REFHIGH_REGISTER, NAN);
      _write_4_bytes(CALIBRATE_REFLOW_REGISTER, NAN);
      _write_4_bytes(CALIBRATE_READMID_REGISTER, NAN);
      _write_4_bytes(CALIBRATE_REFMID_REGISTER, NAN);
      _write_4_bytes(CALIBRATE_READHIGH_REGISTER, NAN);
      _write_4_bytes(CALIBRATE_READLOW_REGISTER, NAN);
      _write_4_bytes(CALIBRATE_SINGLE_OFFSET_REGISTER, NAN);
      _write_4_bytes(CALIBRATE_TEMPERATURE_REGISTER, NAN);
    }

    // Writes system calibration information.
    void i2c::setDeviceInfo(float calibrationLowReading, float calibrationLowReference, float calibrationMidReading, float calibrationMidReference, float calibrationHighReading, float calibrationHighReference, float calibrationSingleOffset, float calibrationTemperature)
    {
      _write_4_bytes(CALIBRATE_READLOW_REGISTER, calibrationLowReading);
      _write_4_bytes(CALIBRATE_REFLOW_REGISTER, calibrationLowReference);
      _write_4_bytes(CALIBRATE_READMID_REGISTER, calibrationMidReading);
      _write_4_bytes(CALIBRATE_REFMID_REGISTER, calibrationMidReference);
      _write_4_bytes(CALIBRATE_READHIGH_REGISTER, calibrationHighReading);
      _write_4_bytes(CALIBRATE_REFHIGH_REGISTER, calibrationHighReference);
      _write_4_bytes(CALIBRATE_SINGLE_OFFSET_REGISTER, calibrationSingleOffset);
      _write_4_bytes(CALIBRATE_TEMPERATURE_REGISTER, calibrationTemperature);
    }

    // Changes the I2C address.
    void i2c::setI2CAddress(uint8_t i2cAddress)
    {
      _write_4_bytes(PH_REGISTER, i2cAddress);
      _send_command(I2C_TASK);
      _address = i2cAddress;
    }

    // If measurepH was called with blocking = true, this retrieves the latest pH measurement.
    void i2c::update()
    {
      _updateRegisters();
    }

    void i2c::_updateRegisters()
    {
      _status = _read_byte(STATUS_REGISTER);
      _pH = _read_4_bytes(PH_REGISTER);
      _mV = _read_4_bytes(MV_REGISTER);

      _tempC = _read_4_bytes(TEMP_C_REGISTER);
      if (_tempC == -127.0)
      {
        _tempF = -127;
      }
      else
      {
        _tempF = ((tempC * 9) / 5) + 32;
      }
    }

    void i2c::_send_command(uint8_t command)
    {
      _i2cPort->beginTransmission(_address);
      _i2cPort->write(TASK_REGISTER);
      _i2cPort->write(command);
      _i2cPort->endTransmission();
    }

    void i2c::_write_4_bytes(uint8_t reg, float f)
    {
      uint8_t b[5];
      float f_val = f;

      b[0] = reg;
      b[1] = *((uint8_t *)&f_val);
      b[2] = *((uint8_t *)&f_val + 1);
      b[3] = *((uint8_t *)&f_val + 2);
      b[4] = *((uint8_t *)&f_val + 3);
      _i2cPort->beginTransmission(_address);
      _i2cPort->write(b, 5);
      _i2cPort->endTransmission();
    }

    float i2c::_read_4_bytes(uint8_t reg)
    {
      float retval;

      _i2cPort->beginTransmission(_address);
      _i2cPort->write(reg);
      _i2cPort->endTransmission();
      _i2cPort->requestFrom(_address, (uint8_t)4);
      *((uint8_t *)&retval) = _i2cPort->read();
      *((uint8_t *)&retval + 1) = _i2cPort->read();
      *((uint8_t *)&retval + 2) = _i2cPort->read();
      *((uint8_t *)&retval + 3) = _i2cPort->read();
      return retval;
    }

    void i2c::_write_byte(uint8_t reg, uint8_t val)
    {
      uint8_t b[5];

      b[0] = reg;
      b[1] = val;
      _i2cPort->beginTransmission(_address);
      _i2cPort->write(b, 2);
      _i2cPort->endTransmission();
    }

    uint8_t i2c::_read_byte(uint8_t reg)
    {
      uint8_t retval;

      _i2cPort->beginTransmission(_address);
      _i2cPort->write(reg);
      _i2cPort->endTransmission();
      _i2cPort->requestFrom(_address, (uint8_t)1);
      retval = _i2cPort->read();

      return retval;
    }
  } // namespace Mod_pH
} // namespace uFire