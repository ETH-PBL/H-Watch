/*
 * time_op_F.c
 *
 *  Created on: 6 mag 2020
 *      Author: flavi
 */
#include "main.h"
#include "rtc.h"
#include "usart.h"
#include "gpio.h"
#include "time.h"
#include "stdio.h"
#include "string.h"
#include "BC95_Def.h"

void Tstamp_to_RTC(uint64_t timestamp, uint8_t setdate){
	extern RTC_TimeTypeDef tim;
	extern RTC_DateTypeDef date;
	uint32_t reg=0;

	reg = timestamp%1000;
	timestamp = timestamp/1000;

	struct tm time_tm;
	time_tm = *(localtime(&timestamp));

	tim.Hours = (uint8_t)time_tm.tm_hour;
	tim.Minutes = (uint8_t)time_tm.tm_min;
	tim.Seconds = ((uint8_t)time_tm.tm_sec + 1); //you must add one second because with the shift of subseconds the first time that secondas increments it doesn't do it
	if (HAL_RTC_SetTime(&hrtc, &tim, RTC_FORMAT_BIN) != HAL_OK)
	{
	 while(1);
	}

	reg = 999-reg;
	reg = (reg*((hrtc.Init.SynchPrediv) + 1))/1000;
	__HAL_RTC_WRITEPROTECTION_DISABLE(&hrtc);
	WRITE_REG(hrtc.Instance->SHIFTR,(uint32_t)reg);
	__HAL_RTC_WRITEPROTECTION_ENABLE(&hrtc);

	if(setdate == 1){
		if (time_tm.tm_wday == 0) {
			time_tm.tm_wday = 7; // the chip goes mon tue wed thu fri sat sun
		}
		date.WeekDay = (uint8_t)time_tm.tm_wday;
		date.Month = (uint8_t)time_tm.tm_mon+1; //momth 1- This is why date math is frustrating.
		date.Date = (uint8_t)time_tm.tm_mday;
		date.Year = (uint16_t)(time_tm.tm_year+1900-1970); // time.h is years since 1900, chip is years since 1970

		/*
		* update the RTC
		*/

		if (HAL_RTC_SetDate(&hrtc, &date, RTC_FORMAT_BIN) != HAL_OK)
		{
		 while(1);
		}
	}

	HAL_RTCEx_BKUPWrite(&hrtc,RTC_BKP_DR0,0x32F2);

}

uint64_t RTC_to_Tstamp(void){
	extern RTC_TimeTypeDef tim;
	extern RTC_DateTypeDef date;
	uint64_t tstamp=0,milli=0;

	HAL_RTC_GetTime(&hrtc, &tim, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);

	if(tim.SubSeconds > (hrtc.Init.SynchPrediv+1)){
		milli = ((((hrtc.Init.SynchPrediv+1)-((tim.SubSeconds)-(hrtc.Init.SynchPrediv)))*1000)/(hrtc.Init.SynchPrediv+1));
	}
	else{
		milli = ((((hrtc.Init.SynchPrediv+1)-(tim.SubSeconds))*1000)/(hrtc.Init.SynchPrediv+1));
	}
//	milli = 999-milli;

	uint8_t hh = tim.Hours;
	uint8_t mm = tim.Minutes;
	uint8_t ss = tim.Seconds;
	uint8_t d = date.Date;
	uint8_t m = date.Month;
	uint16_t y = date.Year;
	uint16_t yr = (uint16_t)(y+1970-1900);

	time_t currentTime = {0};
	struct tm t = {0};
	t.tm_year = yr;
	t.tm_mon = m - 1;
	t.tm_mday = d;
	t.tm_hour = hh;
	t.tm_min = mm;
	t.tm_sec = ss;
	currentTime = mktime(&t);
	tstamp = (((uint64_t)currentTime) *1000) + milli;

	return tstamp;
}

void BC95_time_to_RTC(uint8_t* time_string){
	extern RTC_TimeTypeDef tim;
	extern RTC_DateTypeDef date;
	uint8_t gmt=0;

	sscanf(time_string,"\r\n+CCLK:%u/%u/%u,%u:%u:%u+%u\r\n",&date.Year,&date.Month,&date.Date,&tim.Hours,&tim.Minutes,&tim.Seconds,&gmt);

	tim.Hours = tim.Hours + (gmt/4);
//	date.Year = ((2000+year)-1970);
	date.Year = 50; //devo inserirlo a mano perchè viene rimesso a zero dalla scanf

	if (HAL_RTC_SetTime(&hrtc, &tim, RTC_FORMAT_BIN) != HAL_OK)
	{
	 while(1);
	}
	if (HAL_RTC_SetDate(&hrtc, &date, RTC_FORMAT_BIN) != HAL_OK)
	{
	 while(1);
	}
}

void Set_Acquisition_Alarm(uint8_t h, uint8_t m){
	//sets an rtc alarm: it gets the time and adds h and m, if you put h to 0 it sets only m, if you put m to 0 it set only h ecc. ecc.
	extern RTC_TimeTypeDef tim;
	extern RTC_DateTypeDef date;
	RTC_AlarmTypeDef a;
	HAL_StatusTypeDef s;
	HAL_RTC_GetTime(&hrtc, &tim, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);

	s = HAL_RTC_DeactivateAlarm(&hrtc, RTC_ALARM_A);

	a.Alarm = RTC_ALARM_A;

	if((h!=0) && (m==0)){
		a.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY | RTC_ALARMMASK_SECONDS | RTC_ALARMMASK_MINUTES;
		a.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;

		if(((tim.Hours) + h)>23){
		  a.AlarmTime.Hours = ((tim.Hours) + h) - 24;
		}
		else{
		  a.AlarmTime.Hours = tim.Hours + h;
		}
	}

	if((h==0) && (m!=0)){
		a.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY | RTC_ALARMMASK_SECONDS | RTC_ALARMMASK_HOURS;
		a.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;

		if(((tim.Minutes) + m)>59){
		  a.AlarmTime.Minutes = ((tim.Minutes) + m) - 60;
		}
		else{
		  a.AlarmTime.Minutes = tim.Minutes + m;
		}
	}

	if((h!=0) && (m!=0)){
		a.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY | RTC_ALARMMASK_SECONDS;
		a.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;

		if(((tim.Hours) + h)>23){
		  a.AlarmTime.Hours = ((tim.Hours) + h) - 24;
		}
		else{
		  a.AlarmTime.Hours = tim.Hours + h;
		}

		if(((tim.Minutes) + m)>59){
		  a.AlarmTime.Minutes = ((tim.Minutes) + m) - 60;
		}
		else{
		  a.AlarmTime.Minutes = tim.Minutes + m;
		}
	}

	s = HAL_RTC_SetAlarm_IT(&hrtc, &a, RTC_FORMAT_BIN);
}
