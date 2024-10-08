/******************************************************************************
 *  Project: DecisioNova
 *  File: I2C_bus_scan.cpp
 *  Author: Arman Arezoomand
 *  Date Created: Oct. 2024
 *  Description: Scans the I2C bus for available sensors.
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

void setup()
{
  Wire.begin(I2C_SDA, I2C_SCL);
  Serial.begin(12000000);
  Serial.println("I2C Scanner");
}

void loop()
{
  byte error;

  Serial.println("Scanning...");

  for (int i = 0; i < 16; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      Wire.beginTransmission(i * 8 + j);
      error = Wire.endTransmission();
      if (error == 0)
      {
        Serial.print("0x");
        Serial.print(i * 8 + j, HEX);
        Serial.print("\t");
      }
      else if (error == 4)
      {
        Serial.print("fail\t");
      }
      else
      {
        Serial.print("----\t");
      }
    }
    Serial.print("\n");
  }

  Serial.print("\n\n");

  delay(5000);
}