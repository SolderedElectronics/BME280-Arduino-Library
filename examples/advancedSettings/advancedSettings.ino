//==============================================================================
//    E - R A D I O N I C A . C O M,  H.Kolomana 6/A, Djakovo 31400, Croatia
//==============================================================================
// Project   :  BME280 Arduino Library
// File      :  (BME280) advancedSettings
// Author    :  e-radionica.com 2017
// Licence   :  Open-source ! 
//==============================================================================
//==============================================================================
// Use with any BME280 chip. Check ours: 
// https://e-radionica.com/en/sht21-humidity-and-temperature-sensor.html
// Enables advanced control of chip reading. Check code for more details.
// If any questions, 
// just contact techsupport@e-radionica.com
//==============================================================================


#include <Wire.h>
#include <BME280.h>

///*****************************************************************************
//BME280 advanced settings
///*****************************************************************************
  /* ADDRESS
      write 0 for default address
      write 1 when pads changed
  */

  /* MODE, check datasheet Table 25
      write 0 for sleep mode  - no operation, all reg accessible, lowest power, selected after startup
      write 1 or 2 for forced mode - perform one measurement, store results and return to sleep mode
      write 3 for normal mode - perpetual cycling of measurements and inactive periods
  */

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

  /* FILTER SETTING Table 28.
      write 0 for filter off
      write 1 for filter coef 2
      write 2 for filter coef 4
      write 3 for filter coef 8
      write 4 for filter coef 16
  */

  /*HUMIDITY OVERSAMPLINE Table 20, PRESSURE Table 23, TEMP Table 24
      write 0 for skipped
      write 1 for x1
      write 2 for x2
      write 3 for x4
      write 4 for x8
      write 5 for x16
  */
///*****************************************************************************
uint8_t address = 0;          // default address on e-r BME280 breakout
uint8_t mode = 3;             // normal mode
uint8_t standby = 0;          // recomeded for normal mode
uint8_t filter = 0;           // filter off
uint8_t temp_overSample = 1;  // oversampling temperat x1
uint8_t humi_overSample = 1;  // oversampling humidity x1
uint8_t pres_overSample = 1;  // oversampling pressure x1

BME280 bme;

void setup()
{
  //Call settings before begin()
  bme.settings(address, mode, standby, filter, temp_overSample, humi_overSample, pres_overSample);

  bme.begin();
  Serial.begin(9600);
}

void loop()
{
  Serial.print("Temperature: ");
  Serial.print(bme.readTemp(), 2);
  Serial.println(" degrees C");

  Serial.print("Pressure: ");
  Serial.print(bme.readPressure(), 2);
  Serial.println(" Pa");

  Serial.print("Altitude: ");
  Serial.print(bme.readAltitude(), 2);
  Serial.println("m");

  Serial.print("%RH: ");
  Serial.print(bme.readHumidity(), 2);
  Serial.println(" %");

  Serial.println();

  delay(1000);
}
