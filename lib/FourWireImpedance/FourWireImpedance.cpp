/******************************************************************************
 *  Project: DecisioNova
 *  File: FourWireImpedance.cpp
 *  Author: Arman Arezoomand
 *  Date Created: Oct. 2024
 *  Description: AD5940 4-wire impedance measurement configuration
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
#include "FourWireImpedance.h"

float RtiaCalValue[2]; // Calibrated Rtia value for 50kHz

// Initiate AD5940
int32_t AD5940_AFEResourceInit(void)
{
  AD5940_HWReset();
  AD5940_Initialize();

  // init clock
  CLKCfg_Type clk_cfg;
  clk_cfg.ADCClkDiv = ADCCLKDIV_1;
  clk_cfg.ADCCLkSrc = ADCCLKSRC_HFOSC;
  clk_cfg.SysClkDiv = SYSCLKDIV_1;
  clk_cfg.SysClkSrc = SYSCLKSRC_HFOSC;
  clk_cfg.HfOSC32MHzMode = bFALSE;
  clk_cfg.HFOSCEn = bTRUE;
  clk_cfg.HFXTALEn = bFALSE;
  clk_cfg.LFOSCEn = bTRUE;
  AD5940_CLKCfg(&clk_cfg);

  // init FIFO
  FIFOCfg_Type fifo_cfg;
  fifo_cfg.FIFOEn = bFALSE;
  fifo_cfg.FIFOMode = FIFOMODE_FIFO;
  fifo_cfg.FIFOSize = FIFOSIZE_4KB;
  fifo_cfg.FIFOSrc = FIFOSRC_DFT;
  fifo_cfg.FIFOThresh = 4;
  AD5940_FIFOCfg(&fifo_cfg);

  // start FIFO
  fifo_cfg.FIFOEn = bTRUE;
  AD5940_FIFOCfg(&fifo_cfg);

  // init interupt
  AD5940_INTCCfg(AFEINTC_1, AFEINTSRC_ALLINT, bTRUE);
  AD5940_INTCCfg(AFEINTC_0, AFEINTSRC_DATAFIFOTHRESH, bTRUE);
  AD5940_INTCClrFlag(AFEINTSRC_ALLINT);

  // init GPIO
  AGPIOCfg_Type gpio_cfg;
  gpio_cfg.FuncSet = GP6_SYNC | GP5_SYNC | GP4_SYNC | GP2_TRIG | GP1_SYNC | GP0_TRIG;
  gpio_cfg.InputEnSet = AGPIO_Pin2;
  gpio_cfg.OutputEnSet = AGPIO_Pin0 | AGPIO_Pin1 | AGPIO_Pin4 | AGPIO_Pin5 | AGPIO_Pin6;
  gpio_cfg.OutVal = 0;
  gpio_cfg.PullEnSet = 0;
  AD5940_AGPIOCfg(&gpio_cfg);

  AD5940_SleepKeyCtrlS(SLPKEY_LOCK);

  // init sequencer trig
  SeqGpioTrig_Cfg seqgpiotrig_cfg;
  seqgpiotrig_cfg.bEnable = bTRUE;
  seqgpiotrig_cfg.PinSel = AGPIO_Pin0;
  seqgpiotrig_cfg.SeqPinTrigMode = SEQPINTRIGMODE_FALLING;
  AD5940_SEQGpioTrigCfg(&seqgpiotrig_cfg);

  // init sequencer
  SEQCfg_Type seq_cfg;
  seq_cfg.SeqMemSize = SEQMEMSIZE_2KB;
  seq_cfg.SeqBreakEn = bFALSE;
  seq_cfg.SeqIgnoreEn = bFALSE;
  seq_cfg.SeqCntCRCClr = bTRUE;
  seq_cfg.SeqEnable = bFALSE;
  seq_cfg.SeqWrTimer = 0;
  AD5940_SEQCfg(&seq_cfg);

  // calibrate RTIA
  HSRTIACal_Type hsrtia_cal;
  hsrtia_cal.AdcClkFreq = 16000000.0;
  hsrtia_cal.ADCSinc2Osr = ADCSINC2OSR_22;
  hsrtia_cal.ADCSinc3Osr = ADCSINC3OSR_2;
  hsrtia_cal.bPolarResult = bTRUE;
  hsrtia_cal.DftCfg.DftNum = DFTNUM_1024;
  hsrtia_cal.DftCfg.DftSrc = DFTSRC_SINC3;
  hsrtia_cal.DftCfg.HanWinEn = bTRUE;
  hsrtia_cal.fRcal = 1000, 0; // 1k Ohm
  hsrtia_cal.HsTiaCfg.DiodeClose = bFALSE;
  hsrtia_cal.HsTiaCfg.HstiaBias = HSTIABIAS_1P1;
  hsrtia_cal.HsTiaCfg.HstiaCtia = 16;
  hsrtia_cal.HsTiaCfg.HstiaDeRload = HSTIADERLOAD_OPEN;
  hsrtia_cal.HsTiaCfg.HstiaDeRtia = HSTIADERTIA_TODE;
  hsrtia_cal.HsTiaCfg.HstiaRtiaSel = HSTIARTIA_1K;
  hsrtia_cal.SysClkFreq = 16000000.0;
  hsrtia_cal.fFreq = 50000.0; // excitation freq 50kHz
  AD5940_HSRtiaCal(&hsrtia_cal, RtiaCalValue);

  /* Reconfigure FIFO */
  AD5940_FIFOCtrlS(FIFOSRC_DFT, bFALSE); 
  fifo_cfg.FIFOEn = bTRUE;
  fifo_cfg.FIFOMode = FIFOMODE_FIFO;
  fifo_cfg.FIFOSize = FIFOSIZE_4KB; 
  fifo_cfg.FIFOSrc = FIFOSRC_DFT;
  fifo_cfg.FIFOThresh = 4; 
  AD5940_FIFOCfg(&fifo_cfg);

  // clear interupts
  AD5940_INTCClrFlag(AFEINTSRC_ALLINT);

  uint32_t buffer[512];

  // init sequence generator
  AD5940_SEQGenInit(buffer, 512);
  AD5940_SEQGenCtrl(bTRUE);

  // init AFE ref
  AFERefCfg_Type aferef_cfg;
  aferef_cfg.HpBandgapEn = bTRUE;
  aferef_cfg.Hp1V1BuffEn = bTRUE;
  aferef_cfg.Hp1V8BuffEn = bTRUE;
  aferef_cfg.Disc1V1Cap = bFALSE;
  aferef_cfg.Disc1V8Cap = bFALSE;
  aferef_cfg.Hp1V8ThemBuff = bFALSE;
  aferef_cfg.Hp1V8Ilimit = bFALSE;
  aferef_cfg.Lp1V1BuffEn = bFALSE;
  aferef_cfg.Lp1V8BuffEn = bFALSE;
  aferef_cfg.LpBandgapEn = bTRUE;
  aferef_cfg.LpRefBufEn = bTRUE;
  aferef_cfg.LpRefBoostEn = bFALSE;
  AD5940_REFCfgS(&aferef_cfg);

  // init high precision loop
  HSLoopCfg_Type hs_loop;
  hs_loop.HsDacCfg.ExcitBufGain = EXCITBUFGAIN_2;
  hs_loop.HsDacCfg.HsDacGain = HSDACGAIN_1;
  hs_loop.HsDacCfg.HsDacUpdateRate = 7;
  hs_loop.HsTiaCfg.DiodeClose = bFALSE;
  hs_loop.HsTiaCfg.HstiaBias = HSTIABIAS_1P1;
  hs_loop.HsTiaCfg.HstiaCtia = 16;
  hs_loop.HsTiaCfg.HstiaDeRload = HSTIADERLOAD_OPEN;
  hs_loop.HsTiaCfg.HstiaDeRtia = HSTIADERTIA_OPEN;
  hs_loop.HsTiaCfg.HstiaRtiaSel = HSTIARTIA_1K;
  hs_loop.WgCfg.WgType = WGTYPE_SIN;
  hs_loop.WgCfg.GainCalEn = bFALSE;
  hs_loop.WgCfg.OffsetCalEn = bFALSE;
  hs_loop.SWMatCfg.Dswitch = SWD_OPEN;
  hs_loop.SWMatCfg.Pswitch = SWP_PL | SWP_PL2;
  hs_loop.SWMatCfg.Nswitch = SWN_NL | SWN_NL2;
  hs_loop.SWMatCfg.Tswitch = SWT_TRTIA;
  hs_loop.WgCfg.SinCfg.SinFreqWord = AD5940_WGFreqWordCal(50000, 16000000);
  hs_loop.WgCfg.SinCfg.SinAmplitudeWord = (uint32_t)(2047.5f);
  hs_loop.WgCfg.SinCfg.SinOffsetWord = 0;
  hs_loop.WgCfg.SinCfg.SinPhaseWord = 0;
  AD5940_HSLoopCfgS(&hs_loop);

  // init low-power loop
  LPLoopCfg_Type lp_loop;
  lp_loop.LpDacCfg.LpdacSel = LPDAC0;
  lp_loop.LpDacCfg.LpDacSrc = LPDACSRC_MMR;
  lp_loop.LpDacCfg.LpDacSW = LPDACSW_VBIAS2LPPA | LPDACSW_VBIAS2PIN | LPDACSW_VZERO2LPTIA | LPDACSW_VZERO2PIN;
  lp_loop.LpDacCfg.LpDacVzeroMux = LPDACVZERO_6BIT;
  lp_loop.LpDacCfg.LpDacVbiasMux = LPDACVBIAS_12BIT;
  lp_loop.LpDacCfg.LpDacRef = LPDACREF_2P5;
  lp_loop.LpDacCfg.DataRst = bFALSE;
  lp_loop.LpDacCfg.PowerEn = bTRUE;
  lp_loop.LpDacCfg.DacData12Bit = (uint32_t)((1100 - 200) / 2200.0 * 4095);
  lp_loop.LpDacCfg.DacData6Bit = 31;
  lp_loop.LpAmpCfg.LpAmpSel = LPAMP0;
  lp_loop.LpAmpCfg.LpAmpPwrMod = LPAMPPWR_NORM;
  lp_loop.LpAmpCfg.LpPaPwrEn = bTRUE;
  lp_loop.LpAmpCfg.LpTiaPwrEn = bTRUE;
  lp_loop.LpAmpCfg.LpTiaRf = LPTIARF_20K;
  lp_loop.LpAmpCfg.LpTiaRload = LPTIARLOAD_SHORT;
  lp_loop.LpAmpCfg.LpTiaRtia = LPTIARTIA_OPEN;
  lp_loop.LpAmpCfg.LpTiaSW = LPTIASW(5) | LPTIASW(6) | LPTIASW(7) | LPTIASW(8) | LPTIASW(9) | LPTIASW(12) | LPTIASW(13); /** @todo Optimization needed for new silicon */
  AD5940_LPLoopCfgS(&lp_loop);

  // init DSP block
  DSPCfg_Type dsp_cfg;
  dsp_cfg.ADCBaseCfg.ADCMuxN = ADCMUXN_HSTIA_N;
  dsp_cfg.ADCBaseCfg.ADCMuxP = ADCMUXP_HSTIA_P;
  dsp_cfg.ADCBaseCfg.ADCPga = ADCPGA_1;
  memset(&dsp_cfg.ADCDigCompCfg, 0, sizeof(dsp_cfg.ADCDigCompCfg));
  dsp_cfg.ADCFilterCfg.ADCAvgNum = ADCAVGNUM_16;
  dsp_cfg.ADCFilterCfg.ADCRate = ADCRATE_800KHZ;
  dsp_cfg.ADCFilterCfg.ADCSinc2Osr = ADCSINC2OSR_22;
  dsp_cfg.ADCFilterCfg.ADCSinc3Osr = ADCSINC3OSR_2;
  dsp_cfg.ADCFilterCfg.BpSinc3 = bFALSE;
  dsp_cfg.ADCFilterCfg.BpNotch = bTRUE;
  dsp_cfg.ADCFilterCfg.Sinc2NotchEnable = bTRUE;
  dsp_cfg.DftCfg.DftNum = DFTNUM_1024;
  dsp_cfg.DftCfg.DftSrc = DFTSRC_SINC3;
  dsp_cfg.DftCfg.HanWinEn = bTRUE;
  memset(&dsp_cfg.StatCfg, 0, sizeof(dsp_cfg.StatCfg));
  AD5940_DSPCfgS(&dsp_cfg);

  // turn on AFE blocks
  AD5940_AFECtrlS(AFECTRL_HPREFPWR | AFECTRL_HSTIAPWR | AFECTRL_INAMPPWR | AFECTRL_EXTBUFPWR |
                      AFECTRL_WG | AFECTRL_DACREFPWR | AFECTRL_HSDACPWR | AFECTRL_SINC2NOTCH,
                  bTRUE);
  AD5940_SEQGpioCtrlS(0);

  // closing the sequence
  AD5940_SEQGenInsert(SEQ_STOP());

  AD5940Err error = AD5940ERR_OK;
  uint32_t const *pSeqCmd;
  uint32_t SeqLen;
  SEQInfo_Type Seq1Info;

  error = AD5940_SEQGenFetchSeq(&pSeqCmd, &SeqLen);
  AD5940_SEQGenCtrl(bFALSE);

  if (error == AD5940ERR_OK)
  {
    Seq1Info.SeqId = SEQID_1;
    Seq1Info.SeqRamAddr = 0;
    Seq1Info.pSeqCmd = pSeqCmd;
    Seq1Info.SeqLen = SeqLen;
    // writing the sequence
    AD5940_SEQCmdWrite(Seq1Info.SeqRamAddr, pSeqCmd, SeqLen);
  }
  else
  {
    while (1)
    {
      Serial.println("AFE error: SEQ_1 initiation failed!");
    }
  }

  // calculate clock cycles needed
  uint32_t WaitClks;
  ClksCalInfo_Type clks_cal;
  clks_cal.DataType = DATATYPE_DFT;
  clks_cal.DftSrc = DFTSRC_SINC3;
  clks_cal.DataCount = 1L << (DFTNUM_1024 + 2);
  clks_cal.ADCSinc2Osr = ADCSINC2OSR_22;
  clks_cal.ADCSinc3Osr = ADCSINC3OSR_2;
  clks_cal.ADCAvgNum = 0;
  clks_cal.RatioSys2AdcClk = 1.0;
  AD5940_ClksCalculate(&clks_cal, &WaitClks);

  // trigger sequence generator
  AD5940_SEQGenCtrl(bTRUE);

  // clear the interupt pin to MCU
  AD5940_SEQGpioCtrlS(AGPIO_Pin6 | AGPIO_Pin1);

  // configure the switch matrix
  SWMatrixCfg_Type sw_cfg;
  sw_cfg.Dswitch = SWD_CE0;
  sw_cfg.Pswitch = SWP_CE0;
  sw_cfg.Nswitch = SWP_AIN1;
  sw_cfg.Tswitch = SWT_AIN1 | SWT_TRTIA;
  AD5940_SWMatrixCfgS(&sw_cfg);

  // connect ADC to TIA to measure the current
  AD5940_ADCMuxCfgS(ADCMUXP_HSTIA_P, ADCMUXN_HSTIA_N);
  AD5940_AFECtrlS(AFECTRL_WG | AFECTRL_ADCPWR, bTRUE);
  AD5940_SEQGenInsert(SEQ_WAIT(50));
  AD5940_AFECtrlS(AFECTRL_ADCCNV | AFECTRL_DFT, bTRUE);
  AD5940_SEQGenInsert(SEQ_WAIT(WaitClks));
  AD5940_AFECtrlS(AFECTRL_ADCCNV | AFECTRL_DFT | AFECTRL_WG | AFECTRL_ADCPWR, bFALSE);

  // connect ADC to sense probes to measure voltage
  AD5940_ADCMuxCfgS(ADCMUXP_AIN3, ADCMUXN_AIN2);
  AD5940_AFECtrlS(AFECTRL_WG | AFECTRL_ADCPWR, bTRUE);
  AD5940_SEQGenInsert(SEQ_WAIT(50));
  AD5940_AFECtrlS(AFECTRL_ADCCNV | AFECTRL_DFT, bTRUE);
  AD5940_SEQGenInsert(SEQ_WAIT(WaitClks));
  AD5940_AFECtrlS(AFECTRL_ADCCNV | AFECTRL_DFT | AFECTRL_WG | AFECTRL_ADCPWR, bFALSE);

  AD5940_SEQGpioCtrlS(0);

  // ending sequence
  error = AD5940_SEQGenFetchSeq(&pSeqCmd, &SeqLen);
  AD5940_SEQGenCtrl(bFALSE);
  AD5940_SEQCycleTime();

  if (error != AD5940ERR_OK)
  {
    while (1)
    {
      Serial.println("AFE error: Seq 0 initiation failed!");
    }
  }

  SEQInfo_Type Seq0Info;

  Seq0Info.SeqId = SEQID_0;
  Seq0Info.SeqRamAddr = Seq1Info.SeqRamAddr + Seq1Info.SeqLen;
  Seq0Info.pSeqCmd = pSeqCmd;
  Seq0Info.SeqLen = SeqLen;
  // writing the sequence
  AD5940_SEQCmdWrite(Seq0Info.SeqRamAddr, pSeqCmd, SeqLen);

  // updating sequence 1 info
  Seq1Info.WriteSRAM = bFALSE;
  AD5940_SEQInfoCfg(&Seq1Info);
  seq_cfg.SeqEnable = bTRUE;
  AD5940_SEQCfg(&seq_cfg);
  AD5940_SEQMmrTrig(Seq1Info.SeqId);
  while (AD5940_INTCTestFlag(AFEINTC_1, AFEINTSRC_ENDSEQ) == bFALSE);

  // updating sequence 0 info
  Seq0Info.WriteSRAM = bFALSE;
  AD5940_SEQInfoCfg(&Seq0Info);
  seq_cfg.SeqEnable = bTRUE;
  AD5940_SEQCfg(&seq_cfg); // engaging sequencer trigger
  AD5940_ClrMCUIntFlag();  // clearing AFE interupts

  AD5940_AFEPwrBW(AFEPWR_HP, AFEBW_250KHZ);
  AD5940_WriteReg(REG_AFE_SWMUX, 1 << 3);

  return 0;
}

float q_rsqrt(float number)
{
  long i;
  float x2, y;
  const float threehalfs = 1.5F;

  x2 = number * 0.5F;
  y = number;
  i = *(long *)&y;           // evil floating point bit level hacking
  i = 0x5f3759df - (i >> 1); // what the fuck?
  y = *(float *)&i;
  y = y * (threehalfs - (x2 * y * y)); // 1st iteration
  // y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

  return y;
}

float DataProcess(int32_t *const data)
{
  iImpCar_Type *pSrcData = (iImpCar_Type *)data;

  // convert DFT output to int32_t
  // data is 18bit in two's complement
  for (int i = 0; i < 4; i++)
  {
    data[i] &= 0x3ffff;
    if (data[i] & (1 << 17)) // sign bit
    {
      data[i] |= 0xfffc0000;
    }
  }

  iImpCar_Type DftVolt, DftCurr;

  DftCurr = pSrcData[0];
  DftVolt = pSrcData[1];
  float VoltMag;
  float CurrMag;
  //float VoltPhase;
  //float CurrPhase;

  VoltMag = sqrt((float)DftVolt.Real * DftVolt.Real + (float)DftVolt.Image * DftVolt.Image);
  // VoltPhase = atan2(-pDftVolt->Image,pDftVolt->Real);

  CurrMag = q_rsqrt((float)DftCurr.Real * DftCurr.Real + (float)DftCurr.Image * DftCurr.Image);
  // CurrPhase = atan2(-pDftCurr->Image,pDftCurr->Real);

  // ImpMag = VoltMag * CurrMag * RtiaCalValue[0];
  // ImpPhase = VoltPhase - CurrPhase + RtiaCalValue[1];

  return VoltMag * CurrMag * RtiaCalValue[0];
}