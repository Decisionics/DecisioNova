/******************************************************************************
 *  Project: DecisioNova
 *  File: BLE_test.cpp
 *  Author: Arman Arezoomand
 *  Date Created: Oct. 2024
 *  Description: This file tests the DecisioNova BLE communication. You can use nRF
 *  android or iOS app (by Nordic) to find and connect with DecisioNova.
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
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

#define SERVICE_UUID "161f7607-0c42-496a-a801-cde039a41ab1"
#define CHARACTERISTIC_UUID0 "cc82a80d-28c7-4b16-9de5-8aec5823bf23"
#define CHARACTERISTIC_UUID1 "8d10caa8-ccee-4df5-a8db-a18ce84622f4"

/// use nRF connect app on your smart-phone to test the BLE
class MyCallbacks : public BLECharacteristicCallbacks
{
  void onWrite(BLECharacteristic *pCharacteristic)
  {
    std::string value = pCharacteristic->getValue();

    if (value.length() > 0)
    {
      Serial.print("Value Recieved: ");
      delay(10);
      for (int i = 0; i < value.length(); i++)
        Serial.print(value[i]);
      Serial.println();
    }
  }
};

uint8_t MAC[6];

void setup()
{
  Serial.begin(12000000);
  delay(3000);
  esp_read_mac(MAC, ESP_MAC_BT);
  Serial.printf("Bluetooth MAC: %02X:%02X:%02X:%02X:%02X:%02X\n", MAC[0], MAC[1], MAC[2], MAC[3], MAC[4], MAC[5]);
}

void loop()
{
  BLEDevice::init("DecisioNova");
  BLEServer *pServer = BLEDevice::createServer();

  BLEService *pService = pServer->createService(SERVICE_UUID);

  BLECharacteristic *pCharacteristic0 = pService->createCharacteristic(
      CHARACTERISTIC_UUID0,
      BLECharacteristic::PROPERTY_READ |
          BLECharacteristic::PROPERTY_WRITE);

  BLECharacteristic *pCharacteristic1 = pService->createCharacteristic(
      CHARACTERISTIC_UUID1,
      BLECharacteristic::PROPERTY_READ |
          BLECharacteristic::PROPERTY_WRITE);

  pCharacteristic0->setCallbacks(new MyCallbacks());
  pCharacteristic1->setCallbacks(new MyCallbacks());

  pCharacteristic0->setValue("Minutes");
  pCharacteristic1->setValue("Seconds");
  pService->start();

  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();

  while (1)
  {
    uint32_t minutes = millis() / 60000;
    uint16_t seconds = millis() / 1000;
    pCharacteristic0->setValue(minutes);
    pCharacteristic1->setValue(seconds);

    delay(200);
  }
}