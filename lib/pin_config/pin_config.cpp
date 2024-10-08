/******************************************************************************
 *  Project: DecisioNova
 *  File: pin_config.cpp
 *  Author: Arman Arezoomand
 *  Date Created: Oct. 2024
 *  Description: DecisioNova hardware configurations, AD5940 interface functions,
 *  SPI interface with multiplexers, LCD interface, and etc.
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

#include "pin_config.h"

// uninitalised pointers to SPI objects
SPIClass *vspi = new SPIClass(VSPI);
SPIClass *hspi = new SPIClass(HSPI);

void AD5940_ReadWriteNBytes(unsigned char *pSendBuffer, unsigned char *pRecvBuff, unsigned long length)
{
  for (int i = 0; i < length; i++)
  {
    pRecvBuff[i] = vspi->transfer(pSendBuffer[i]);
  }
}

void AD5940_CsClr(void)
{
  digitalWrite(vspi->pinSS(), LOW);
}

void AD5940_CsSet(void)
{
  digitalWrite(vspi->pinSS(), HIGH);
}

void AD5940_RstSet(void)
{
  digitalWrite(AD_RST_PIN, HIGH);
}

void AD5940_RstClr(void)
{
  digitalWrite(AD_RST_PIN, LOW);
}

void AD5940_Delay10us(uint32_t time)
{
  delayMicroseconds(10);
}

uint32_t AD5940_GetMCUIntFlag(void)
{
  return ucInterrupted;
}

uint32_t AD5940_ClrMCUIntFlag(void)
{
  ucInterrupted = 0;
  // digitalWrite(25, LOW);

  return 1;
}

/* MCU related external line interrupt service routine */
void Ext_Int0_Handler()
{
  ucInterrupted = 1;
  // digitalWrite(25, HIGH);
  /* This example just set the flag and deal with interrupt in AD5940Main function. It's your choice to choose how to process interrupt. */
}

uint32_t AD5940_MCUResourceInit(void *pCfg)
{
  vspi = new SPIClass(VSPI);
  vspi->begin();
  vspi->setFrequency(SPI_CLOCK);

  pinMode(18, OUTPUT);
  pinMode(vspi->pinSS(), OUTPUT);
  pinMode(AD_RST_PIN, OUTPUT);
  pinMode(AD_INT_PIN, INPUT);

  pinMode(AD_TRG_PIN, OUTPUT);
  digitalWrite(AD_TRG_PIN, HIGH);

  attachInterrupt(AD_INT_PIN, Ext_Int0_Handler, FALLING);
  AD5940_CsSet();
  AD5940_RstSet();
  return 0;
}

uint32_t MUX_MCUResourceInit(void)
{
  hspi = new SPIClass(HSPI);
  hspi->begin(HSPI_SCLK, HSPI_MISO, HSPI_MOSI, HSPI_SS);
  hspi->setFrequency(SPI_CLOCK);

  pinMode(hspi->pinSS(), OUTPUT);
  return 0;
}

void HSPI_ReadWriteNBytes(unsigned char *pSendBuffer, unsigned char *pRecvBuff, unsigned long length)
{
  for (int i = 0; i < length; i++)
  {
    pRecvBuff[i] = hspi->transfer(pSendBuffer[i]);
  }
}

void set_mux(uint8_t IP, uint8_t IN, uint8_t MP, uint8_t MN)
{
  /*notes: the MUX 2 is not routed in order; thus we should first translate the user desired
  electrodes to what is routed on the board.
  in PCB MUX 2 receive the MOSI. MUX 1 works with the carry out pin
  MUX 2 Channel A is Injection P
  MUX 2 Channel B is Measuremnet N
  MUX 1 Channel A is Measurement P
  MUX 1 Channel B is Injection N
  MOSI => MUX 2 => MUX 1 => MISO*/

  // translation to real world routing
  while (MP > 15)
    MP -= 16;
  while (MN > 15)
    MN -= 16;
  while (IP > 15)
    IP -= 16;
  while (IN > 15)
    IN -= 16;
  MP = MP;
  IN = IN;
  if (IP < 8)
    IP = 7 - IP;
  else
    IP = 23 - IP;
  if (MN < 8)
    MN = 7 - MN;
  else
    MN = 23 - MN;

  uint32_t firstlong = 0;
  uint32_t secondlong = 0;

  secondlong = (1 << (MN + 16)) | (1 << (IP));
  firstlong = (1 << (IN + 16)) | (1 << (MP));

  // write them to spi
  digitalWrite(hspi->pinSS(), LOW);
  hspi->write32(firstlong);
  hspi->write32(secondlong);
  digitalWrite(hspi->pinSS(), HIGH);
}

void LCDbacklight(int brightness)
{
  uint32_t command;

  digitalWrite(hspi->pinSS(), HIGH);

  command = 0x25;
  command <<= 5;
  // OUTTMG = 1, EXTGCK = 0, TMGRST = 1, DSPRPT = 1, BLANK = 0 -> 0x16
  command |= 0x16;
  command <<= 7;
  command |= brightness;
  command <<= 7;
  command |= brightness;
  command <<= 7;
  command |= brightness;

  noInterrupts();

  for (uint8_t n = 0; n < 1; n++)
  {
    hspi->transfer(command >> 24);
    hspi->transfer(command >> 16);
    hspi->transfer(command >> 8);
    hspi->transfer(command);

    // 12 channels per TLC59711
    for (int8_t c = 11; c >= 0; c--)
    {
      // 16 bits per channel, send MSB first
      hspi->transfer(0x7f);
      hspi->transfer(0x00);
    }
  }
  interrupts();

  digitalWrite(hspi->pinSS(), LOW);
}
