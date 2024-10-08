/******************************************************************************
 *  Project: DecisioNova
 *  File: multiplexer_bus_test.cpp
 *  Author: Arman Arezoomand
 *  Date Created: Oct. 2024
 *  Description: This example tests the SPI bus of the multiplexers. 
 *  It sends a set of characters through SPI and receives the returned numbers. 
 *  If the sent numbers and the returned numbers are the same, the bus is working properly.
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
#include <pin_config.h>

void setup()
{
  Serial.begin(12000000);
  MUX_MCUResourceInit();
  digitalWrite(hspi->pinSS(), LOW);
}

void loop()
{
  delay(1000);
  unsigned char *setR = new unsigned char[8];
  setR[0] = 0x51;
  setR[1] = 0x52;
  setR[2] = 0x53;
  setR[3] = 0x54;
  setR[4] = 0x55;
  setR[5] = 0x56;
  setR[6] = 0x57;
  setR[7] = 0x58;
  unsigned char *recR = new unsigned char[8];
  digitalWrite(hspi->pinSS(), LOW);
  HSPI_ReadWriteNBytes(setR,recR,8);
  digitalWrite(hspi->pinSS(), HIGH);
  Serial.println("The numbers below must be 51 52 53 54 55 56 57 58");
  for (int i = 0; i < 8; i++)
  {
    Serial.print(recR[i], HEX);

    Serial.print(" ");
  }
  Serial.println("");
}