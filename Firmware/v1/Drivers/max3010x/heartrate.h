/*
 * heartrate.h
 *
 *  Created on: 7 ott 2020
 *      Author: tommy
 */

#ifndef MAX3010X_HEARTRATE_H_
#define MAX3010X_HEARTRATE_H_

uint8_t checkForBeat(int32_t sample);
int16_t averageDCEstimator(int32_t *p, uint16_t x);
int16_t lowPassFIRFilter(int16_t din);
int32_t mul16(int16_t x, int16_t y);

#endif /* MAX3010X_HEARTRATE_H_ */
