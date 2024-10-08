/******************************************************************************
 *  Project: DecisioNova
 *  File: pin_config.h
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

#pragma once

#include <Arduino.h>
#include <SPI.h>
#include <ad5940.h>

#define NEOPIXEL_PIN 38
#define I2C_SDA 43
#define I2C_SCL 44

#define HSPI_MISO 18
#define HSPI_MOSI 17
#define HSPI_SCLK 21
#define HSPI_SS 16

#define VSPI_MISO 13
#define VSPI_MOSI 11
#define VSPI_SCLK 12
#define VSPI_SS 10

#define VSPI FSPI

#define SPI_CLOCK 16000000

#define AD_RST_PIN 1
#define AD_INT_PIN 2
#define AD_TRG_PIN 3

#define PUSH_BUTTON 4

/*LCD settings*/

#define ST7789_DRIVER
#define INIT_SEQUENCE_3

#define CGRAM_OFFSET
#define TFT_RGB_ORDER TFT_RGB

#define TFT_INVERSION_ON

#define TFT_PARALLEL_8_BIT

#define TFT_HEIGHT 320
#define TFT_WIDTH 170

#define TFT_D0 39
#define TFT_D1 40
#define TFT_D2 41
#define TFT_D3 42
#define TFT_D4 45
#define TFT_D5 46
#define TFT_D6 47
#define TFT_D7 48

#define TFT_CS 6
#define TFT_DC 7
#define TFT_RST 5
#define TFT_WR 8
#define TFT_RD 9
#define PIN_TOUCH_INT 14

#define LOAD_GLCD
#define LOAD_FONT2
#define LOAD_FONT4
#define LOAD_FONT6
#define LOAD_FONT7
#define LOAD_FONT8
#define LOAD_GFXFF
#define SMOOTH_FONT

volatile static uint32_t ucInterrupted = 0; /* Flag to indicate interrupt occurred */

// uninitalised pointers to SPI objects
extern SPIClass *vspi;
extern SPIClass *hspi;

uint32_t MUX_MCUResourceInit(void);
void LCDbacklight(int brightness);
void set_mux(uint8_t IP, uint8_t IN, uint8_t MP, uint8_t MN);
void HSPI_ReadWriteNBytes(unsigned char *pSendBuffer, unsigned char *pRecvBuff, unsigned long length);
