/******************************************************************************
 *  Project: DecisioNova
 *  File: external_flash_test.cpp
 *  Author: Arman Arezoomand
 *  Date Created: Oct. 2024
 *  Description: This file tests the MCU connection with the external flash chip.
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

void setup(){
  Serial.begin(12000000);
}

void loop(){
    Serial.printf("ESP32-S3 SDK: %s\n", ESP.getSdkVersion());
    Serial.printf("ESP32-S3 CPU Freq: %i MHz\n", getCpuFrequencyMhz());
    Serial.printf("ESP32-S3 XTAL Freq: %i MHz\n", getXtalFrequencyMhz());
    Serial.printf("ESP32-S3 APB Freq: %i MHz\n", int(getApbFrequency() / 1000000.0));
    Serial.printf("ESP32-S3 flash chip size: %i MB\n", int(ESP.getFlashChipSize() / (1024.0 * 1024)));
    Serial.printf("ESP32-S3 RAM size: %.2f KB\n", ESP.getHeapSize() / 1024.0);
    Serial.printf("ESP32-S3 available heap: %.2f KB\n", ESP.getFreeHeap() / 1024.0);
    Serial.printf("ESP32-S3 Maximum RAM Alloc: %.2f KB\n", ESP.getMaxAllocHeap() / 1024.0);
    Serial.printf("ESP32-S3 Available PSRAM: %.2f KB\n", ESP.getFreePsram() / 1024.0);

    ps_malloc(550 * 1024);
    uint8_t * buf = (uint8_t*)malloc(500 * 1024);
    if (buf == NULL)
        Serial.println("malloc does not work!\n");
    else
        Serial.println("malloc works!");
  
    Serial.println("");
    delay(5000);
}