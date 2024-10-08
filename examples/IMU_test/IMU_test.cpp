/******************************************************************************
 *  Project: DecisioNova
 *  File: IMU_test.cpp
 *  Author: Arman Arezoomand
 *  Date Created: Oct. 2024
 *  Description: Testing DecisioNova IMU (ICM(20948))
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
#include <Adafruit_ICM20948.h>

Adafruit_ICM20948 imu;
uint16_t measurement_delay_us = 65535;
TwoWire I2C = TwoWire(0);

void setup(void)
{
  Serial.begin(12000000);

  I2C.begin(I2C_SDA, I2C_SCL, 1000000);

  // Try to initialize!
  if (!imu.begin_I2C(0x68, &I2C))
  {
    while (1)
    {
      delay(10);
      Serial.println("Failed to find ICM20948 chip");
    }
  }

  Serial.println("ICM20948 Found!");
}

void loop()
{
  // creating sensor event
  sensors_event_t accel;
  sensors_event_t gyro;
  sensors_event_t mag;
  sensors_event_t temp;
  imu.getEvent(&accel, &gyro, &temp, &mag);

  Serial.printf("Temperature %.2f ^C\n", temp.temperature);
  Serial.printf("Accelerometer X: %.2f, Y: %.2f, Z: %.2f [m/s^2]\n", accel.acceleration.x, accel.acceleration.y, accel.acceleration.z);
  Serial.printf("Magnetometer  X: %.2f, Y: %.2f, Z: %.2f [uT]\n", mag.magnetic.x, mag.magnetic.y, mag.magnetic.z);
  Serial.printf("Gyroscope     X: %.2f, Y: %.2f, Z: %.2f [radian/s]\n", gyro.gyro.x, gyro.gyro.y, gyro.gyro.z);

  Serial.println();

  delay(100);
}