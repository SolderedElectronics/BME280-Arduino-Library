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
// Reads temperature, pressure and humidity from the chip and prints it 
// to Serial monitor. If any questions, 
// just contact techsupport@e-radionica.com
//==============================================================================


#include <Wire.h>
#include <BME280.h>

BME280 bme;

void setup()
{
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
