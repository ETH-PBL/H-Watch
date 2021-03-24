/*
 * heartrate.c
 *
 * Optical Heart Rate Detection (PBA Algorithm)
 *
 *  Created on: 7 ott 2020
 *      Author: Tommaso Polonelli
 */

#include "main.h"
#include "heartRate.h"

static int16_t IR_AC_Max = 20;
static int16_t IR_AC_Min = -20;

static int16_t IR_AC_Signal_Current = 0;
static int16_t IR_AC_Signal_Previous;
static int16_t IR_AC_Signal_min = 0;
static int16_t IR_AC_Signal_max = 0;
static int16_t IR_Average_Estimated;

static int16_t positiveEdge = 0;
static int16_t negativeEdge = 0;
static int32_t ir_avg_reg = 0;

static int16_t cbuf[32];
static uint8_t offset = 0;

static const uint16_t FIRCoeffs[12] = {172, 321, 579, 927, 1360, 1858, 2390, 2916, 3391, 3768, 4012, 4096};

//  Heart Rate Monitor functions takes a sample value and the sample number
//  Returns true if a beat is detected
//  A running average of four samples is recommended for display on the screen.
uint8_t checkForBeat(int32_t sample)
{
	uint8_t beatDetected = 0;

  //  Save current state
  IR_AC_Signal_Previous = IR_AC_Signal_Current;

  //This is good to view for debugging
  //Serial.print("Signal_Current: ");
  //Serial.println(IR_AC_Signal_Current);

  //  Process next data sample
  IR_Average_Estimated = averageDCEstimator(&ir_avg_reg, sample);
  IR_AC_Signal_Current = lowPassFIRFilter(sample - IR_Average_Estimated);

  //  Detect positive zero crossing (rising edge)
  if ((IR_AC_Signal_Previous < 0) & (IR_AC_Signal_Current >= 0))
  {

    IR_AC_Max = IR_AC_Signal_max; //Adjust our AC max and min
    IR_AC_Min = IR_AC_Signal_min;

    positiveEdge = 1;
    negativeEdge = 0;
    IR_AC_Signal_max = 0;

    //if ((IR_AC_Max - IR_AC_Min) > 100 & (IR_AC_Max - IR_AC_Min) < 1000)
    if (((IR_AC_Max - IR_AC_Min) > 20) && ((IR_AC_Max - IR_AC_Min) < 1000))
    {
      //Heart beat!!!
      beatDetected = 1;
    }
  }

  //  Detect negative zero crossing (falling edge)
  if ((IR_AC_Signal_Previous > 0) & (IR_AC_Signal_Current <= 0))
  {
    positiveEdge = 0;
    negativeEdge = 1;
    IR_AC_Signal_min = 0;
  }

  //  Find Maximum value in positive cycle
  if (positiveEdge & (IR_AC_Signal_Current > IR_AC_Signal_Previous))
  {
    IR_AC_Signal_max = IR_AC_Signal_Current;
  }

  //  Find Minimum value in negative cycle
  if (negativeEdge & (IR_AC_Signal_Current < IR_AC_Signal_Previous))
  {
    IR_AC_Signal_min = IR_AC_Signal_Current;
  }

  return(beatDetected);
}

//  Average DC Estimator
int16_t averageDCEstimator(int32_t *p, uint16_t x)
{
  *p += ((((long) x << 15) - *p) >> 4);
  return (*p >> 15);
}

//  Low Pass FIR Filter
int16_t lowPassFIRFilter(int16_t din)
{
  cbuf[offset] = din;

  int32_t z = mul16(FIRCoeffs[11], cbuf[(offset - 11) & 0x1F]);

  for (uint8_t i = 0 ; i < 11 ; i++)
  {
    z += mul16(FIRCoeffs[i], cbuf[(offset - i) & 0x1F] + cbuf[(offset - 22 + i) & 0x1F]);
  }

  offset++;
  offset %= 32; //Wrap condition

  return(z >> 15);
}

//  Integer multiplier
int32_t mul16(int16_t x, int16_t y)
{
  return((long)x * (long)y);
}
