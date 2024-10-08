/******************************************************************************
 *  Project: DecisioNova
 *  File: wifi_test.cpp
 *  Author: Arman Arezoomand
 *  Date Created: Oct. 2024
 *  Description: This example tests DecisioNova wifi communication by connecting to a
 *  wifi access point.
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
#include <WiFi.h>

const char *ssid = "your_ssid";
const char *password = "your_password";

void setup()
{
  Serial.begin(12000000);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("trying to connect!");
  }
  Serial.print("WiFi connected with IP:");
  Serial.println(WiFi.localIP());
}

void loop()
{
  WiFiClient client;

  // enter an IP address and port to connect
  while (!client.connect(IPAddress(192, 168, 1, 123), 10000))
  {
    Serial.println("Connection to host failed!");
    delay(1000);
  }

  Serial.println("client connected sending packet!");
  client.print("Hello from ESP32S3!");
  client.stop();
  delay(1000);
}