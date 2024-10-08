/******************************************************************************
 *  Project: DecisioNova
 *  File: FourWireEIT.cpp
 *  Author: Arman Arezoomand
 *  Date Created: Oct. 2024
 *  Description: This is a simple example of DecisioNova performing Electrical Impedance Tomography (EIT)
 *  using the FourWireImpedance library.
 *  In this example, DecisioNova conducts 4-wire impedance measurements following the 'adjacent electrodes
 *  stimulation and adjacent electrodes measurements' protocol. In the serial monitor, each line represents
 *  208 impedance values measured across 16 electrodes, following the order described in the pyEIT data format
 *  (https://github.com/eitcom/pyEIT/blob/master/doc/pyEIT-data_format.pdf), with trim=True and parser=fmmu.
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
#include <ad5940.h>
#include <pin_config.h>
#include <FourWireImpedance.h>

#define BUFFER_SIZE 256
uint32_t buffer[BUFFER_SIZE];

void setup()
{
  delay(1000);
  Serial.begin(12000000);
  AD5940_MCUResourceInit(0);
  MUX_MCUResourceInit();
  AD5940_AFEResourceInit();
  if (AD5940_WakeUp(10) > 10) // wakeup AFE
    Serial.println("AFE error: wake up failed!");

  AD5940_ClrMCUIntFlag();
}

void loop()
{
  for (int i = 0; i < 16; i++)
  {
    for (int j = 0; j < 13; j++)
    {
      set_mux(i, i + 1, i + 2 + j, i + 3 + j);
      delayMicroseconds(100);

      digitalWrite(AD_TRG_PIN, LOW); // trigger the impedance measurement

      while (AD5940_GetMCUIntFlag() == 0)
      {} // wait for data ready interrupt

      digitalWrite(AD_TRG_PIN, HIGH); // clear trigger pin

      AD5940_ClrMCUIntFlag(); // clear interupts

      AD5940_FIFORd((uint32_t *)buffer, 4);
      Serial.print(DataProcess((int32_t *)buffer));

      if (!(i == 15 && j == 12))
        Serial.print(",");
    }
  }
  Serial.println("");
}
