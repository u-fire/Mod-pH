#ifndef Mod_pH_H
#define Mod_pH_H

#include <Arduino.h>
#include <Wire.h>

#define PH_PH_MEASUREMENT_TIME 750
#define PH_TEMP_MEASURE_TIME 750

#define UFIRE_MOD_PH 0x0b

#define MEASURE_PH_TASK 80      /*!< Command to start a pH measure */
#define MEASURE_TEMP_TASK 40    /*!< Command to measure temperature */
#define CALIBRATE_LOW_TASK 20   /*!< Command to calibrate the low point of the probe */
#define CALIBRATE_MID_TASK 10   /*!< Command to calibrate the mid point of the probe */
#define CALIBRATE_HIGH_TASK 8   /*!< Command to calibrate the high point of the probe */
#define CALIBRATE_SINGLE_TASK 4 /*!< Command to calibrate the single point of the probe */
#define I2C_TASK 2              /*!< Command to change the i2c address */

#define HW_VERSION_REGISTER 0               /*!< hardware version register */
#define FW_VERSION_REGISTER 1               /*!< firmware version  register */
#define TASK_REGISTER 2                     /*!< task register */
#define STATUS_REGISTER 3                   /*!< status register */
#define PH_REGISTER 4                       /*!< pH register */
#define TEMP_C_REGISTER 8                   /*!< temperature in C register */
#define MV_REGISTER 12                      /*!< mV register */
#define CALIBRATE_REFLOW_REGISTER 16        /*!< reference low register */
#define CALIBRATE_READLOW_REGISTER 20       /*!< reading high register */
#define CALIBRATE_REFMID_REGISTER 24        /*!< reference low register */
#define CALIBRATE_READMID_REGISTER 28       /*!< reading high register */
#define CALIBRATE_REFHIGH_REGISTER 32       /*!< reference low register */
#define CALIBRATE_READHIGH_REGISTER 36      /*!< reading high register */
#define CALIBRATE_SINGLE_OFFSET_REGISTER 40 /*!< single point offset register */
#define CALIBRATE_TEMPERATURE_REGISTER 44   /*!< calibration temperature register */

namespace Microfire
{
  namespace Mod_pH
  {
    class i2c
    {
    public:
      const float &pH = _pH;
      const float &tempC = _tempC;
      const float &tempF = _tempF;
      const float &mV = _mV;
      const float &calibrationLowReading = _calibrationLowReading;
      const float &calibrationLowReference = _calibrationLowReference;
      const float &calibrationMidReading = _calibrationMidReading;
      const float &calibrationMidReference = _calibrationMidReference;
      const float &calibrationHighReading = _calibrationHighReading;
      const float &calibrationHighReference = _calibrationHighReference;
      const float &calibrationSingleOffset = _calibrationSingleOffset;
      const float &calibrationTemperature = _calibrationTemperature;
      const int &hwVersion = _hwVersion;
      const int &fwVersion = _fwVersion;
      const int &status = _status;

      typedef enum pH_Error_e
      {
        STATUS_NO_ERROR,
        STATUS_OUTSIDE_LOWER,
        STATUS_OUTSIDE_UPPER,
        STATUS_SYSTEM_ERROR
      } pH_Error_t;

      bool begin(TwoWire &wirePort = Wire, uint8_t address = UFIRE_MOD_PH);
      bool connected();
      float calibrateLow(float solution_pH, float tempC = 25.0, bool blocking = true);
      float calibrateMid(float solution_pH, float tempC = 25.0, bool blocking = true);
      float calibrateHigh(float solution_pH, float tempC = 25.0, bool blocking = true);
      float calibrateSingle(float solution_pH, float tempC = 25.0, bool blocking = true);
      void getDeviceInfo();
      float measurepH(float temp = 25.0, bool blocking = true);
      float measureTemp(bool blocking = true);
      void reset();
      void setDeviceInfo(float calibrationLowReading, float calibrationLowReference, float calibrationMidReading, float calibrationMidReference, float calibrationHighReading, float calibrationHighReference, float calibrationSingleOffset, float calibrationTemperature);
      void setI2CAddress(uint8_t i2cAddress);
      void update();

    private:
      uint8_t _address;
      TwoWire *_i2cPort;
      static float _pH;
      static float _tempC;
      static float _tempF;
      static float _mV;
      static float _calibrationLowReading;
      static float _calibrationLowReference;
      static float _calibrationMidReading;
      static float _calibrationMidReference;
      static float _calibrationHighReading;
      static float _calibrationHighReference;
      static float _calibrationSingleOffset;
      static float _calibrationTemperature;
      static int _hwVersion;
      static int _fwVersion;
      static int _status;

      void _updateRegisters();
      void _send_command(uint8_t command);
      void _write_4_bytes(uint8_t reg, float f);
      void _write_byte(uint8_t reg, uint8_t val);
      float _read_4_bytes(uint8_t reg);
      uint8_t _read_byte(uint8_t reg);
    };
  } // namespace Mod_pH
} // namespace uFire
#endif // ifndef Mod_pH_H