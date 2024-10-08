/******************************************************************************
 *  Project: DecisioNova
 *  File: FourWireImpedance.h
 *  Author: Arman Arezoomand
 *  Date Created: Oct. 2024
 *  Description: AD5940 4-wire impedance measurement configuration header file
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
#include <ad5940.h>

int32_t AD5940_AFEResourceInit();
float DataProcess(int32_t *const pData);
