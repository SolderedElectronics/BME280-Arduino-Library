//==============================================================================
//    E - R A D I O N I C A . C O M,  H.Kolomana 6/A, Djakovo 31400, Croatia
//==============================================================================
// Project   :  BME280 Arduino Library (V1.0)
// File      :  BME280.h
// Author    :  e-radionica.com 2017
// Licence   :  Open-source !
//==============================================================================
//==============================================================================
// Use with any BME280 I2C breakout. Check ours:
//
// If any questions,
// just contact techsupport@e-radionica.com
//==============================================================================

#include <BME280.h>
#include <stdint.h>
#include <math.h>

#include <Wire.h>

//****************************************************************************//
//public
//****************************************************************************//

//****************************************************************************//
BME280::BME280( void )
{
  //defaults, call settings() to change
  address = 0x76;       // default address on e-r BME280 breakout
  mode = 3;             // normal mode
  standby = 0;           // recomeded for normal mode
  filter = 0;            // filter off
  temp_overSample = 1;  // oversampling temperat x1
  humi_overSample = 1;  // oversampling humidity x1
  pres_overSample = 1;  // oversampling pressure x1
}
//****************************************************************************//
//Constructor, sets default settings
//****************************************************************************//

//****************************************************************************//
uint8_t BME280::begin()
{
	uint8_t dataToWrite = 0;
	Wire.begin();

	//Reading all compensation data, range 0x88:A1, 0xE1:E7
	calibration.dig_T1 = ((uint16_t)((readRegister(BME280_DIG_T1_MSB_REG) << 8) + readRegister(BME280_DIG_T1_LSB_REG)));
	calibration.dig_T2 = ((int16_t)((readRegister(BME280_DIG_T2_MSB_REG) << 8) + readRegister(BME280_DIG_T2_LSB_REG)));
	calibration.dig_T3 = ((int16_t)((readRegister(BME280_DIG_T3_MSB_REG) << 8) + readRegister(BME280_DIG_T3_LSB_REG)));

	calibration.dig_P1 = ((uint16_t)((readRegister(BME280_DIG_P1_MSB_REG) << 8) + readRegister(BME280_DIG_P1_LSB_REG)));
	calibration.dig_P2 = ((int16_t)((readRegister(BME280_DIG_P2_MSB_REG) << 8) + readRegister(BME280_DIG_P2_LSB_REG)));
	calibration.dig_P3 = ((int16_t)((readRegister(BME280_DIG_P3_MSB_REG) << 8) + readRegister(BME280_DIG_P3_LSB_REG)));
	calibration.dig_P4 = ((int16_t)((readRegister(BME280_DIG_P4_MSB_REG) << 8) + readRegister(BME280_DIG_P4_LSB_REG)));
	calibration.dig_P5 = ((int16_t)((readRegister(BME280_DIG_P5_MSB_REG) << 8) + readRegister(BME280_DIG_P5_LSB_REG)));
	calibration.dig_P6 = ((int16_t)((readRegister(BME280_DIG_P6_MSB_REG) << 8) + readRegister(BME280_DIG_P6_LSB_REG)));
	calibration.dig_P7 = ((int16_t)((readRegister(BME280_DIG_P7_MSB_REG) << 8) + readRegister(BME280_DIG_P7_LSB_REG)));
	calibration.dig_P8 = ((int16_t)((readRegister(BME280_DIG_P8_MSB_REG) << 8) + readRegister(BME280_DIG_P8_LSB_REG)));
	calibration.dig_P9 = ((int16_t)((readRegister(BME280_DIG_P9_MSB_REG) << 8) + readRegister(BME280_DIG_P9_LSB_REG)));

	calibration.dig_H1 = ((uint8_t)(readRegister(BME280_DIG_H1_REG)));
	calibration.dig_H2 = ((int16_t)((readRegister(BME280_DIG_H2_MSB_REG) << 8) + readRegister(BME280_DIG_H2_LSB_REG)));
	calibration.dig_H3 = ((uint8_t)(readRegister(BME280_DIG_H3_REG)));
	calibration.dig_H4 = ((int16_t)((readRegister(BME280_DIG_H4_MSB_REG) << 4) + (readRegister(BME280_DIG_H4_LSB_REG) & 0x0F)));
	calibration.dig_H5 = ((int16_t)((readRegister(BME280_DIG_H5_MSB_REG) << 4) + ((readRegister(BME280_DIG_H4_LSB_REG) >> 4) & 0x0F)));
	calibration.dig_H6 = ((uint8_t)readRegister(BME280_DIG_H6_REG));

	//Set the oversampling control words.
	//config will only be writeable in sleep mode, so first insure that.
	writeRegister(BME280_CTRL_MEAS_REG, 0x00);

	//Set the config word
	dataToWrite = (standby << 0x5) & 0xE0;
	dataToWrite |= (filter << 0x02) & 0x1C;
	writeRegister(BME280_CONFIG_REG, dataToWrite);

	//Set ctrl_hum first, then ctrl_meas to activate ctrl_hum
	dataToWrite = humi_overSample & 0x07; //all other bits can be ignored
	writeRegister(BME280_CTRL_HUMIDITY_REG, dataToWrite);

	//set ctrl_meas
	//First, set temp oversampling
	dataToWrite = (temp_overSample << 0x5) & 0xE0;
	//Next, pressure oversampling
	dataToWrite |= (pres_overSample << 0x02) & 0x1C;
	//Last, set mode
	dataToWrite |= (mode) & 0x03;
	//Load the byte
	writeRegister(BME280_CTRL_MEAS_REG, dataToWrite);

  //BME280 needs 2ms to start up
  delay(10);

  // sensor ID is 0x60 and it's saved in register offset 0xD0
  // you can use this to check breakout connection
  if (readRegister(0xD0) != 0x60) return 1;
  else return 0;
}
//****************************************************************************//
//Call to update settings
//****************************************************************************//

//****************************************************************************//
void BME280::reset()
{
  writeRegister(BME280_RST_REG, 0xB6);
}
//****************************************************************************//
//Call begin() after reset
//****************************************************************************//

//****************************************************************************//
void BME280::settings(uint8_t _address, uint8_t _mode, uint8_t _standby, uint8_t _filter, uint8_t _temp_overSample, uint8_t _humi_overSample, uint8_t _pres_overSample)
{
  /* ADDRESS
      write 0 for default address
      write 1 when pads changed
  */
  switch (_address) {
    case 0:
      address = 0x76;
      break;

    case 1:
      address = 0x77;
      break;
  }

  /* MODE, check datasheet Table 25
      write 0 for sleep mode  - no operation, all reg accessible, lowest power, selected after startup
      write 1 or 2 for forced mode - perform one measurement, store results and return to sleep mode
      write 3 for normal mode - perpetual cycling of measurements and inactive periods
  */
  mode = _mode;

  /* t_sb setting STANDBY, check datasheet table 27
      write 0 for 0.5ms
      write 1 for 62.5ms
      write 2 for 125ms
      write 3 for 250ms
      write 4 for 500ms
      write 5 for 1000ms
      write 6 for 10ms
      write 7 for 20ms
  */
  standby = _standby;

  /* FILTER SETTING Table 28.
      write 0 for filter off
      write 1 for filter coef 2
      write 2 for filter coef 4
      write 3 for filter coef 8
      write 4 for filter coef 16
  */
  filter = _filter;

  /*HUMIDITY OVERSAMPLINE Table 20, PRESSURE Table 23, TEMP Table 24
      write 0 for skipped
      write 1 for x1
      write 2 for x2
      write 3 for x4
      write 4 for x8
      write 5 for x16
  */
  temp_overSample = _temp_overSample;
}
//****************************************************************************//
//Use to change default settings
//****************************************************************************//

//****************************************************************************//
//module readings
//****************************************************************************//
///---->
//****************************************************************************//
//PRESSURE
//****************************************************************************//
float BME280::readPressure( void )
{
	// Returns pressure in Pa
	int32_t adc_P = ((uint32_t)readRegister(BME280_PRESSURE_MSB_REG) << 12) | ((uint32_t)readRegister(BME280_PRESSURE_LSB_REG) << 4) | ((readRegister(BME280_PRESSURE_XLSB_REG) >> 4) & 0x0F);

	int64_t var1, var2, p_acc;
	var1 = ((int64_t)t_fine) - 128000;
	var2 = var1 * var1 * (int64_t)calibration.dig_P6;
	var2 = var2 + ((var1 * (int64_t)calibration.dig_P5)<<17);
	var2 = var2 + (((int64_t)calibration.dig_P4)<<35);
	var1 = ((var1 * var1 * (int64_t)calibration.dig_P3)>>8) + ((var1 * (int64_t)calibration.dig_P2)<<12);
	var1 = (((((int64_t)1)<<47)+var1))*((int64_t)calibration.dig_P1)>>33;
	if (var1 == 0)
	{
		return 0; // avoid exception caused by division by zero
	}
	p_acc = 1048576 - adc_P;
	p_acc = (((p_acc<<31) - var2)*3125)/var1;
	var1 = (((int64_t)calibration.dig_P9) * (p_acc>>13) * (p_acc>>13)) >> 25;
	var2 = (((int64_t)calibration.dig_P8) * p_acc) >> 19;
	p_acc = ((p_acc + var1 + var2) >> 8) + (((int64_t)calibration.dig_P7)<<4);

	return (float)p_acc / 256.0;
}
//****************************************************************************//

//****************************************************************************//
//ALTITUDE
//****************************************************************************//
float BME280::readAltitude( void )
{
  // Returns Atltitude in m
	float heightOutput = 0;
	heightOutput = ((float)-45846.2)*(pow(((float)readPressure()/(float)101325), 0.190263) - (float)1);
	return heightOutput;
}
//****************************************************************************//

//****************************************************************************//
//HUMIDITY
//****************************************************************************//
float BME280::readHumidity( void )
{
	// Returns humidity in %RH
	int32_t adc_H = ((uint32_t)readRegister(BME280_HUMIDITY_MSB_REG) << 8) | ((uint32_t)readRegister(BME280_HUMIDITY_LSB_REG));

	int32_t var1;
	var1 = (t_fine - ((int32_t)76800));
	var1 = (((((adc_H << 14) - (((int32_t)calibration.dig_H4) << 20) - (((int32_t)calibration.dig_H5) * var1)) +
	((int32_t)16384)) >> 15) * (((((((var1 * ((int32_t)calibration.dig_H6)) >> 10) * (((var1 * ((int32_t)calibration.dig_H3)) >> 11) + ((int32_t)32768))) >> 10) + ((int32_t)2097152)) *
	((int32_t)calibration.dig_H2) + 8192) >> 14));
	var1 = (var1 - (((((var1 >> 15) * (var1 >> 15)) >> 7) * ((int32_t)calibration.dig_H1)) >> 4));
	var1 = (var1 < 0 ? 0 : var1);
	var1 = (var1 > 419430400 ? 419430400 : var1);

	return (float)(var1>>12) / 1024.0;
}
//****************************************************************************//

//****************************************************************************//
//TEMPERATURE
//****************************************************************************//
float BME280::readTemp( void )
{
	// Returns temperature in DegC, resolution is 0.01 DegC
	int32_t adc_T = ((uint32_t)readRegister(BME280_TEMPERATURE_MSB_REG) << 12) | ((uint32_t)readRegister(BME280_TEMPERATURE_LSB_REG) << 4) | ((readRegister(BME280_TEMPERATURE_XLSB_REG) >> 4) & 0x0F);

	int64_t var1, var2;
	var1 = ((((adc_T>>3) - ((int32_t)calibration.dig_T1<<1))) * ((int32_t)calibration.dig_T2)) >> 11;
	var2 = (((((adc_T>>4) - ((int32_t)calibration.dig_T1)) * ((adc_T>>4) - ((int32_t)calibration.dig_T1))) >> 12) *
	((int32_t)calibration.dig_T3)) >> 14;
	t_fine = var1 + var2;
	float output = (t_fine * 5 + 128) >> 8;

	output = output / 100;
	return output;
}
//****************************************************************************//

//****************************************************************************//
//end of public
//****************************************************************************//

//****************************************************************************//
//private
//****************************************************************************//
void BME280::writeRegister(uint8_t offset, uint8_t dataToWrite)
{
		Wire.beginTransmission(address);
		Wire.write(offset);
		Wire.write(dataToWrite);
		Wire.endTransmission();
}

uint8_t BME280::readRegister(uint8_t offset)
{
	uint8_t result;
	uint8_t numBytes = 1;

	Wire.beginTransmission(address);
	Wire.write(offset);
	Wire.endTransmission();

	Wire.requestFrom(address, numBytes);
	while ( Wire.available() )
	{
		result = Wire.read();
	}

  return result;
}
//****************************************************************************//
