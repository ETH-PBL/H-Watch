/*
 * time_op_F.h
 *
 *  Created on: 6 mag 2020
 *      Author: flavi
 */

#ifndef INC_TIME_OP_F_H_
#define INC_TIME_OP_F_H_

void Tstamp_to_RTC(uint64_t timestamp, uint8_t setdate);
uint64_t RTC_to_Tstamp(void);
void BC95_time_to_RTC(uint8_t* time_string);
void Set_Acquisition_Alarm(uint8_t h, uint8_t m);

#endif /* INC_TIME_OP_F_H_ */
