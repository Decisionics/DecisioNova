/******************************************************************************
 *  Project: DecisioNova
 *  File: AD5941_ID_test.cpp
 *  Author: Arman Arezoomand
 *  Date Created: Oct. 2024
 *  Description: This file uses the most basic commands to retreive the AD5941 chip ID.
 *  if output (serial monitor) shows 80 80 41 44, it means the SPI bus between MCU and AFE is working properly.
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
  AD5940_MCUResourceInit(0);
}

void loop()
{
  AD5940_RstClr();
  delay(1);
  AD5940_RstSet();

  delay(100);

  unsigned char *setA = new unsigned char[3];
  setA[0] = 0x20;
  setA[1] = 0x04;
  setA[2] = 0x00;
  unsigned char *recA = new unsigned char[3];
  AD5940_CsClr();
  AD5940_ReadWriteNBytes(setA, recA, 3);
  AD5940_CsSet();

  unsigned char *setR = new unsigned char[4];
  setR[0] = 0x6D;
  setR[1] = 0x55;
  setR[2] = 0x55;
  setR[3] = 0x55;
  unsigned char *recR = new unsigned char[4];
  AD5940_CsClr();
  AD5940_ReadWriteNBytes(setR, recR, 4);
  AD5940_CsSet();
  Serial.println("The number bellow must be 80 80 41 44");
  for (int i = 0; i < 4; i++)
  {
    Serial.print(recR[i], HEX);
    Serial.print(" ");
  }
  Serial.println("");
  delay(1);
  // Serial.println(AD5940_GetADIID(), HEX);
}