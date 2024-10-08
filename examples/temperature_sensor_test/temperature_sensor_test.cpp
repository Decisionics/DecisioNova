/******************************************************************************
 *  Project: DecisioNova
 *  File: temperature_sensor_test.cpp
 *  Author: Arman Arezoomand
 *  Date Created: Oct. 2024
 *  Description: Testing DecisioNova temperature sensor (BME280)
 *
 *  License:
 *  This work is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.
 *  To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-sa/4.0/ or
 *  send a letter to Creative Commons, PO Box 1866, Mountain View, CA 94042, USA.
 *
 *  Under the following terms:
 *  - Attribution: You must give appropriate credit, provide a link to the license, and indicate if changes were made.
 *  - NonCommercial: You may not use the material for commercial purposes.
 *  - ShareAlike: If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
 *
 *  Copyright (c) 2024 - Arman Arezoomand
 ******************************************************************************/

#include <Arduino.h>
#include <wire.h>
#include <pin_config.h>
#include <Adafruit_BME280.h>

float const seaLevelPressure = 1013.25;

TwoWire I2C = TwoWire(0);
Adafruit_BME280 bme;

void printValues()
{
}

void setup()
{
  Serial.begin(12000000);
  Serial.println(F("BME280 test"));
  I2C.begin(I2C_SDA, I2C_SCL);

  if (!bme.begin(0x77, &I2C))
  {
    while (1)
    {
      Serial.println("failed to connect!");
    }
  }

  Serial.println("connected!");
  Serial.println();
}

void loop()
{
  Serial.printf("Temperature: %.1f ^C\n", bme.readTemperature());
  Serial.printf("Pressure: %.1f hPa\n", bme.readPressure() / 100.0F);
  Serial.printf("Altitude: %.1f m\n", bme.readAltitude(seaLevelPressure));
  Serial.printf("Humidity: %.1f %%\n", bme.readHumidity());
  Serial.println();
}
