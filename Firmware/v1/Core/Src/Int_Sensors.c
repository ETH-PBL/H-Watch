/**
 ******************************************************************************
 * @file           : Int_Sensors.c
 * @brief          : Sensors Management
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */


/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "macro.h"
#include "cmsis_os.h"
#include "i2c.h"
#include "gpio.h"
#include "Int_Sensors.h"
#include "spi.h"

#include "string.h"

#include "lps22hb_reg.h"
#include "lsm303agr_reg.h"
#include "lsm6dsm_reg.h"
#include "max3010x.h"
#include "heartrate.h"
#include "spo2_algorithm.h"

/* Private includes ----------------------------------------------------------*/


/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/

enum {
	EVT_LSM6DSM  						= (1 << 0), /* 0b0000000000000001 */
	EVT_LSM303AGR               			= (1 << 1), /* 0b0000000000000010 */
	EVT_LPS22HB             			= (1 << 2), /* 0b0000000000000100 */
	EVT_PPG             			= (1 << 3), /* 0b0000000000001000 */
	EVT_XX                 		= (1 << 4), /* 0b0000000000010000 */
	EVT_LSM303AGR_INT1              = (1 << 5), /* 0b0000000000100000 */
	EVT_LSM303AGR_INT2             = (1 << 8), /* 0b0000000100000000 */
	kCGDisplayDisabledFlag            = (1 << 9), /* 0b0000001000000000 */
	kCGDisplayMirrorFlag              = (1 << 10),/* 0b0000010000000000 */
	kCGDisplayUnMirrorFlag            = (1 << 11),/* 0b0000100000000000 */
	kCGDisplayDesktopShapeChangedFlag = (1 << 12) /* 0b0001000000000000 */
};

#define LOW_TIMER_MS 				(1000)
#define PPG_TIMER_MS 				(10)


/* heart rate */
#define RATE_SIZE 4 //Increase this for more averaging. 4 is good.
#define MAX_BRIGHTNESS 255
#define SPO2_BUFF_SIZE	100
/* Private variables ---------------------------------------------------------*/

static StaticEventGroup_t IntSensors_EventGroupBuffer;
EventGroupHandle_t IntSensors_Event;

// An array to hold handles to the created timers.
static xTimerHandle Low_Timer;
static StaticTimer_t Low_Timer_Buffer;

static xTimerHandle PPG_Timer;
static StaticTimer_t PPG_Timer_Buffer;

//float temp, hum;
//Sensors_data_t Sen_dt;
Sensors_data_t Sen_dt_b;
uint16_t Sen_dt_p = 0;
uint16_t Sen_dt_stream = 0;


static stmdev_ctx_t dev_ctx_xl;
static 	stmdev_ctx_t dev_ctx_mg;

static stmdev_ctx_t dev_ctx;

static stmdev_ctx_t lsm_ctx;


/* SPO2 variables */
static uint32_t irBuffer[SPO2_BUFF_SIZE]; //infrared LED sensor data
static uint32_t redBuffer[SPO2_BUFF_SIZE];  //red LED sensor data


/* Private function prototypes -----------------------------------------------*/

static void Low_Timer_Callback( TimerHandle_t xTimer );
static void PPG_Timer_Callback( TimerHandle_t xTimer );
static int LSM303AGR_Init(void);
static void LSM303AGR_Read_All(void);
static int LPS22HB_Init (void);
static void LPS22HB_Read_All(void);
static int LSM6DSM_Init (void);
static void LSM6DSM_Read_All(void);
uint8_t MAX30105_readRegister8( uint8_t reg);
void MAX30105_writeRegister8( uint8_t reg, uint8_t value);
void MAX30105_readRegisterN( uint8_t reg, uint16_t size, uint8_t *data);
static void MAX30105_Read_All(void);
static void HeartRate_SPO2 (uint32_t IR, uint32_t RED);

/* Private user code ---------------------------------------------------------*/





void IntSensors_Task (void const * argument){

	EventBits_t uxBits;

	/* Disable deep sleep */
	Mngm_DeepSleep_en (0);

	osDelay(1);

	// Create the event group
	IntSensors_Event = xEventGroupCreateStatic( &IntSensors_EventGroupBuffer );

	// Create low frequency timer
	Low_Timer = xTimerCreateStatic("Low_Timer", // Just a text name, not used by the kernel.
			LOW_TIMER_MS ,     					// The timer period in ticks.
			pdTRUE,         					// The timers will auto-reload themselves when they expire.
			"Low_Timer",     					// Assign each timer a unique id equal to its array index.
			Low_Timer_Callback,     			// Each timer calls the same callback when it expires.
			&Low_Timer_Buffer
	);
	// Create low frequency timer
	PPG_Timer = xTimerCreateStatic("PPG_Timer", // Just a text name, not used by the kernel.
			PPG_TIMER_MS ,     					// The timer period in ticks.
			pdTRUE,         					// The timers will auto-reload themselves when they expire.
			"PPG_Timer",     					// Assign each timer a unique id equal to its array index.
			PPG_Timer_Callback,     			// Each timer calls the same callback when it expires.
			&PPG_Timer_Buffer
	);

	/* Start Init Sensors */
	if (LSM303AGR_Init()){
		/* delete task */
		vTaskDelete( osThreadGetId () );
		return;
	}
	osDelay(1);

	if(LPS22HB_Init()){
		/* delete task */
		vTaskDelete( osThreadGetId () );
		return;
	}
	osDelay(1);

//	if(LSM6DSM_Init ()){
//		/* delete task */
//		vTaskDelete( osThreadGetId () );
//		return;
//	}

	if(MAX30105_Init(MAX30105_readRegister8, MAX30105_writeRegister8, MAX30105_readRegisterN)){
		/* delete task */
		vTaskDelete( osThreadGetId () );
		return;
	}
	/* power level: 6mA, 4 samples, led mode 3, samplerate 400, ADC 4096 LSB */
	MAX30105_setup(0x3F, 4, 3, 400, 411, 4096);
	/* enable interrupt on FIFO */
	MAX30105_enableAFULL();
	MAX30105_setFIFOAlmostFull(3);

	/* start application */
	xTimerStart( Low_Timer, 1000 );
	xTimerStart( PPG_Timer, 1000 );
	while (1){

		/* Enable deep sleep */
		Mngm_DeepSleep_en (1);

		uxBits = xEventGroupWaitBits(
				/* The event group being tested. */
				IntSensors_Event,
				/* The bits within the event group to wait for. */
				EVT_LSM6DSM | EVT_LSM303AGR | EVT_LSM303AGR_INT1 | EVT_LSM303AGR_INT2 | EVT_LPS22HB | EVT_PPG,
				/* BIT_0 & BIT_4 should be cleared before returning. */
				pdTRUE,
				/* Don't wait for both bits, either bit will do. */
				pdFALSE,
				/* Wait a maximum of 100ms for either bit to be set. */
				portMAX_DELAY );

		/* Disable deep sleep */
		Mngm_DeepSleep_en (0);

		if(IS_EVT(EVT_LSM303AGR)){
			LSM303AGR_Read_All();
		}
		if(IS_EVT(EVT_LSM303AGR_INT1)){
			LSM303AGR_Read_All();
		}
		if(IS_EVT(EVT_LSM303AGR_INT2)){
			LSM303AGR_Read_All();
		}

		if(IS_EVT(EVT_LPS22HB)){
			LPS22HB_Read_All();
		}

		if(IS_EVT(EVT_LSM6DSM)){
//			LSM6DSM_Read_All();
		}

		if(IS_EVT(EVT_PPG)){
			MAX30105_Read_All();
		}
		CHECK_STACK_OF();

	}


}




/*
 *   MAX30101
 */


//
// Low-level I2C Communication
//
uint8_t MAX30105_readRegister8( uint8_t reg) {

	uint8_t bufp;

	HAL_I2C_Mem_Read(&hi2c1, MAX30101_ADDRESS, reg,
			I2C_MEMADD_SIZE_8BIT, &bufp, 1, 1000);

	return (bufp); //Fail

}

void MAX30105_readRegisterN( uint8_t reg, uint16_t size, uint8_t *data) {

	HAL_I2C_Mem_Read(&hi2c1, MAX30101_ADDRESS, reg,
			I2C_MEMADD_SIZE_8BIT, data, size, 1000);

}

void MAX30105_writeRegister8( uint8_t reg, uint8_t value) {

	HAL_I2C_Mem_Write(&hi2c1, MAX30101_ADDRESS, reg,
			I2C_MEMADD_SIZE_8BIT, &value, 1, 1000);

}

static void HeartRate (uint32_t IR){

	static long lastBeat = 0; //Time at which the last beat occurred
	static uint8_t rateSpot = 0;
	static float rates[RATE_SIZE] = {0}; //Array of heart rates


	float beatsPerMinute;
	float beatAvg;

	if (checkForBeat(IR) == 1)
	{
		//We sensed a beat!
		long delta = HAL_GetTick() - lastBeat;
		lastBeat = HAL_GetTick();

		beatsPerMinute = 60 / (delta / 1000.0);

		if (beatsPerMinute < 255 && beatsPerMinute > 20)
		{
			rates[rateSpot++] = beatsPerMinute; //Store this reading in the array
			rateSpot %= RATE_SIZE; //Wrap variable

			//Take average of readings
			beatAvg = 0;
			for (uint8_t x = 0 ; x < RATE_SIZE ; x++)
				beatAvg += rates[x];
			beatAvg /= RATE_SIZE;

			/* save data */
			Sen_dt_b.HeartRate = beatsPerMinute;
			Sen_dt_b.avgHeartRate = beatAvg;
		}
	}
}


static void HeartRate_SPO2 (uint32_t IR, uint32_t RED){

	const int32_t bufferLength = SPO2_BUFF_SIZE; //data length
	int32_t spo2; //SPO2 value
	int8_t validSPO2; //indicator to show if the SPO2 calculation is valid
	int32_t heartRate; //heart rate value
	int8_t validHeartRate; //indicator to show if the heart rate calculation is valid

	static int buffercount = ((SPO2_BUFF_SIZE/4)*3);

	/* update the buffer */
	redBuffer[buffercount] = RED;
	irBuffer[buffercount] = IR;
	buffercount++;

	if (buffercount >= SPO2_BUFF_SIZE){

		/* calculate SPO2 and Heart Rate */
		maxim_heart_rate_and_oxygen_saturation(irBuffer, (int32_t)bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);

		//TODO
		/* New data */
		if (validSPO2){
			Sen_dt_b.spo2 = spo2;
		}
		if (validHeartRate){
			Sen_dt_b.spo2_HeartRate = heartRate;
		}

		//dumping the first 25 sets of samples in the memory and shift the last 75 sets of samples to the top
		for ( buffercount = (SPO2_BUFF_SIZE/4); buffercount < SPO2_BUFF_SIZE; buffercount++)
		{
			redBuffer[buffercount - (SPO2_BUFF_SIZE/4)] = redBuffer[buffercount];
			irBuffer[buffercount - (SPO2_BUFF_SIZE/4)] = irBuffer[buffercount];
		}

		buffercount = ((SPO2_BUFF_SIZE/4)*3);

	}

}


static void MAX30105_Read_All(void){

	/* read data from the MAX30105 */
	if(!MAX30105_check()){
		return;
	}

	Sen_dt_b.PPG_red = MAX30105_getFIFORed();
	Sen_dt_b.PPG_IR = MAX30105_getFIFOIR();
	Sen_dt_b.PPG_green = MAX30105_getFIFOGreen();

	if (Sen_dt_b.PPG_IR < 50000){
		/* No finger? */
		HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET);
	}else{
		HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET);

		while(MAX30105_available() > 0){
			/* process data for HeartRate */
			HeartRate (MAX30105_getFIFOIR());

			/* Spo2 */
			HeartRate_SPO2 (MAX30105_getFIFOIR(), MAX30105_getFIFORed());

			/* get next sample from the FIFO */
			MAX30105_nextSample();
			/* iterates until the FIFO is empty */
		}

	}

}

/*
 *   END MAX30101
 */



/*
 *   LSM6DSM
 */


static int32_t LSM6DSM_platform_write(void *handle, uint8_t reg, uint8_t *bufp,
		uint16_t len)
{
	if (handle == &hi2c1)
	{
		HAL_I2C_Mem_Write(handle, LSM6DSM_I2C_ADD_H, reg,
				I2C_MEMADD_SIZE_8BIT, bufp, len, 1000);
	}
	else if (handle == &hspi1)
	{
		HAL_GPIO_WritePin(LSM6DSM_CS_GPIO_Port, LSM6DSM_CS_Pin, GPIO_PIN_RESET);
		HAL_SPI_Transmit(handle, &reg, 1, 1000);
		HAL_SPI_Transmit(handle, bufp, len, 1000);
		HAL_GPIO_WritePin(LSM6DSM_CS_GPIO_Port, LSM6DSM_CS_Pin, GPIO_PIN_SET);
	}
	return 0;
}


static int32_t LSM6DSM_platform_read(void *handle, uint8_t reg, uint8_t *bufp,
		uint16_t len)
{
	if (handle == &hi2c1)
	{
		HAL_I2C_Mem_Read(handle, LSM6DSM_I2C_ADD_H, reg,
				I2C_MEMADD_SIZE_8BIT, bufp, len, 1000);
	}
	else if (handle == &hspi1)
	{
		/* Read command */
		reg |= 0x80;
		HAL_GPIO_WritePin(LSM6DSM_CS_GPIO_Port, LSM6DSM_CS_Pin, GPIO_PIN_RESET);
		HAL_SPI_Transmit(handle, &reg, 1, 1000);
		HAL_SPI_Receive(handle, bufp, len, 1000);
		HAL_GPIO_WritePin(LSM6DSM_CS_GPIO_Port, LSM6DSM_CS_Pin, GPIO_PIN_SET);
	}
	return 0;
}

static int LSM6DSM_Init (void)
{
	uint8_t whoamI, rst;
	lsm6dsm_int1_route_t int_1_reg;

	lsm_ctx.write_reg = LSM6DSM_platform_write;
	lsm_ctx.read_reg = LSM6DSM_platform_read;
	lsm_ctx.handle = &hspi1;

	/* Wait sensor boot time */
	osDelay(15);

	/* Check device ID */
	lsm6dsm_device_id_get(&lsm_ctx, &whoamI);
	if (whoamI != LSM6DSM_ID){
		/* manage here device not found */
		return 1;
	}

	/* Restore default configuration */
	lsm6dsm_reset_set(&lsm_ctx, PROPERTY_ENABLE);
	do {
		lsm6dsm_reset_get(&lsm_ctx, &rst);
	} while (rst);

	/* Enable Block Data Update */
	lsm6dsm_block_data_update_set(&lsm_ctx, PROPERTY_ENABLE);

	/* Set full scale */
	lsm6dsm_xl_full_scale_set(&lsm_ctx, LSM6DSM_2g);
	lsm6dsm_gy_full_scale_set(&lsm_ctx, LSM6DSM_2000dps);

	/* Set Output Data Rate */
	lsm6dsm_xl_data_rate_set(&lsm_ctx, LSM6DSM_XL_ODR_12Hz5);
	lsm6dsm_gy_data_rate_set(&lsm_ctx, LSM6DSM_GY_ODR_12Hz5);

	/* Enable drdy 75 μs pulse: uncomment if interrupt must be pulsed */
	//lsm6dsm_data_ready_mode_set(&dev_ctx, LSM6DSM_DRDY_PULSED);

	/* Enable interrupt generation on DRDY INT1 pin */
	lsm6dsm_pin_int1_route_get(&lsm_ctx, &int_1_reg);
	int_1_reg.int1_drdy_g = PROPERTY_ENABLE;
	int_1_reg.int1_drdy_xl = PROPERTY_ENABLE;
	lsm6dsm_pin_int1_route_set(&lsm_ctx, int_1_reg);

	return 0;

}


static void LSM6DSM_Read_All(void){

	uint8_t reg;
	axis3bit16_t data_raw_acceleration;
	axis3bit16_t data_raw_angular_rate;

	/* Read status register */
	lsm6dsm_xl_flag_data_ready_get(&lsm_ctx, &reg);
	if (reg)
	{
		/* Read accelerometer field data */
		memset(data_raw_acceleration.u8bit, 0, 3 * sizeof(int16_t));
		lsm6dsm_acceleration_raw_get(&lsm_ctx, data_raw_acceleration.u8bit);
		Sen_dt_b.acc_mg[0] =
				lsm6dsm_from_fs2g_to_mg(data_raw_acceleration.i16bit[0]);
		Sen_dt_b.acc_mg[1] =
				lsm6dsm_from_fs2g_to_mg(data_raw_acceleration.i16bit[1]);
		Sen_dt_b.acc_mg[2] =
				lsm6dsm_from_fs2g_to_mg(data_raw_acceleration.i16bit[2]);
	}

	lsm6dsm_gy_flag_data_ready_get(&lsm_ctx, &reg);
	if (reg)
	{
		/* Read gyroscope field data */
		memset(data_raw_angular_rate.u8bit, 0, 3 * sizeof(int16_t));
		lsm6dsm_angular_rate_raw_get(&lsm_ctx, data_raw_angular_rate.u8bit);
		Sen_dt_b.angular_rate_mdps[0] =
				lsm6dsm_from_fs2000dps_to_mdps(data_raw_angular_rate.i16bit[0]);
		Sen_dt_b.angular_rate_mdps[1] =
				lsm6dsm_from_fs2000dps_to_mdps(data_raw_angular_rate.i16bit[1]);
		Sen_dt_b.angular_rate_mdps[2] =
				lsm6dsm_from_fs2000dps_to_mdps(data_raw_angular_rate.i16bit[2]);
	}

}

/*
 *   END LSM6DSM
 */



/*
 *   LPS22HB
 */


static int32_t LPS22HB_platform_write(void *handle, uint8_t reg, uint8_t *bufp,
		uint16_t len)
{
	if (handle == &hi2c3)
	{
		HAL_I2C_Mem_Write(handle, LPS22HB_I2C_ADD_H, reg,
				I2C_MEMADD_SIZE_8BIT, bufp, len, 1000);
	}
	else if (handle == &hspi2)
	{
		HAL_GPIO_WritePin(LPS22HB_CS_GPIO_Port, LPS22HB_CS_Pin, GPIO_PIN_RESET);
		HAL_SPI_Transmit(handle, &reg, 1, 1000);
		HAL_SPI_Transmit(handle, bufp, len, 1000);
		HAL_GPIO_WritePin(LPS22HB_CS_GPIO_Port, LPS22HB_CS_Pin, GPIO_PIN_SET);
	}

	return 0;
}


static int32_t LPS22HB_platform_read(void *handle, uint8_t reg, uint8_t *bufp,
		uint16_t len)
{
	if (handle == &hi2c3)
	{
		HAL_I2C_Mem_Read(handle, LPS22HB_I2C_ADD_H, reg,
				I2C_MEMADD_SIZE_8BIT, bufp, len, 1000);
	}
	else if (handle == &hspi2)
	{
		/* Read command */
		reg |= 0x80;
		HAL_GPIO_WritePin(LPS22HB_CS_GPIO_Port, LPS22HB_CS_Pin, GPIO_PIN_RESET);
		HAL_SPI_Transmit(handle, &reg, 1, 1000);
		HAL_SPI_Receive(handle, bufp, len, 1000);
		HAL_GPIO_WritePin(LPS22HB_CS_GPIO_Port, LPS22HB_CS_Pin, GPIO_PIN_SET);
	}

	return 0;
}



static int LPS22HB_Init (void)
{
	uint8_t whoamI,rst;

	/* Initialize mems driver interface */
	dev_ctx.write_reg = LPS22HB_platform_write;
	dev_ctx.read_reg = LPS22HB_platform_read;
	dev_ctx.handle = &hspi2;

	/* Check device ID */
	lps22hb_device_id_get(&dev_ctx, &whoamI);
	if (whoamI != LPS22HB_ID){
		return 1;
	}

	/* Restore default configuration */
	lps22hb_reset_set(&dev_ctx, PROPERTY_ENABLE);
	do {
		lps22hb_reset_get(&dev_ctx, &rst);
	} while (rst);

	/* Can be enabled low pass filter on output */
	lps22hb_low_pass_filter_mode_set(&dev_ctx, LPS22HB_LPF_ODR_DIV_2);

	/* Can be set Data-ready signal on INT_DRDY pin */
	lps22hb_drdy_on_int_set(&dev_ctx, PROPERTY_ENABLE);

	/* Set Output Data Rate */
	lps22hb_data_rate_set(&dev_ctx, LPS22HB_ODR_10_Hz);

	return 0;

}


static void LPS22HB_Read_All(void){

	axis1bit32_t data_raw_pressure;
	axis1bit16_t data_raw_temperature;
	uint8_t reg;

	/* Read output only if new value is available */
	lps22hb_press_data_ready_get(&dev_ctx, &reg);
	if (reg)
	{
		memset(data_raw_pressure.u8bit, 0x00, sizeof(int32_t));
		lps22hb_pressure_raw_get(&dev_ctx, data_raw_pressure.u8bit);

		Sen_dt_b.pressure_hPa = lps22hb_from_lsb_to_hpa(data_raw_pressure.i32bit);

		memset(data_raw_temperature.u8bit, 0x00, sizeof(int16_t));
		lps22hb_temperature_raw_get(&dev_ctx, data_raw_temperature.u8bit);

		Sen_dt_b.temp_degC = lps22hb_from_lsb_to_degc(data_raw_temperature.i16bit);
	}

}

/*
 *   END LPS22HB
 */


/*
 *   LSM303AGR
 */

static int32_t LSM303AGR_platform_write(void *handle, uint8_t Reg, uint8_t *Bufp,
		uint16_t len)
{
	uint32_t i2c_add = (uint32_t)handle;
	if (i2c_add == LSM303AGR_I2C_ADD_XL)
	{
		/* enable auto incremented in multiple read/write commands */
		Reg |= 0x80;
	}
	HAL_I2C_Mem_Write(&hi2c3, i2c_add, Reg,
			I2C_MEMADD_SIZE_8BIT, Bufp, len, 1000);
	return 0;
}

static int32_t LSM303AGR_platform_read(void *handle, uint8_t Reg, uint8_t *Bufp,
		uint16_t len)
{
	uint32_t i2c_add = (uint32_t)handle;
	if (i2c_add == LSM303AGR_I2C_ADD_XL)
	{
		/* enable auto incremented in multiple read/write commands */
		Reg |= 0x80;
	}
	HAL_I2C_Mem_Read(&hi2c3, (uint8_t) i2c_add, Reg,
			I2C_MEMADD_SIZE_8BIT, Bufp, len, 1000);
	return 0;
}

static int LSM303AGR_Init(void){

	uint8_t whoamI;

	/* Initialize mems driver interface */
	dev_ctx_xl.write_reg = LSM303AGR_platform_write;
	dev_ctx_xl.read_reg = LSM303AGR_platform_read;
	dev_ctx_xl.handle = (void*)LSM303AGR_I2C_ADD_XL;

	dev_ctx_mg.write_reg = LSM303AGR_platform_write;
	dev_ctx_mg.read_reg = LSM303AGR_platform_read;
	dev_ctx_mg.handle = (void*)LSM303AGR_I2C_ADD_MG;

	/* Check device ID */
	whoamI = 0;
	lsm303agr_xl_device_id_get(&dev_ctx_xl, &whoamI);
	if ( whoamI != LSM303AGR_ID_XL )
		return 1;

	whoamI = 0;
	lsm303agr_mag_device_id_get(&dev_ctx_mg, &whoamI);
	if ( whoamI != LSM303AGR_ID_MG )
		return 1;


	/* Enable Block Data Update */
	lsm303agr_xl_block_data_update_set(&dev_ctx_xl, PROPERTY_ENABLE);
	lsm303agr_mag_block_data_update_set(&dev_ctx_mg, PROPERTY_ENABLE);
	/* Set Output Data Rate */
	lsm303agr_xl_data_rate_set(&dev_ctx_xl, LSM303AGR_XL_ODR_1Hz);
	lsm303agr_mag_data_rate_set(&dev_ctx_mg, LSM303AGR_MG_ODR_10Hz);
	/* Set accelerometer full scale */
	lsm303agr_xl_full_scale_set(&dev_ctx_xl, LSM303AGR_2g);
	/* Set / Reset magnetic sensor mode */
	lsm303agr_mag_set_rst_mode_set(&dev_ctx_mg, LSM303AGR_SENS_OFF_CANC_EVERY_ODR);
	/* Enable temperature compensation on mag sensor */
	lsm303agr_mag_offset_temp_comp_set(&dev_ctx_mg, PROPERTY_ENABLE);
	/* Enable temperature sensor */
	lsm303agr_temperature_meas_set(&dev_ctx_xl, LSM303AGR_TEMP_ENABLE);
	/* Set device in continuos mode */
	lsm303agr_xl_operating_mode_set(&dev_ctx_xl, LSM303AGR_HR_12bit);
	/* Set magnetometer in continuos mode */
	lsm303agr_mag_operating_mode_set(&dev_ctx_mg, LSM303AGR_CONTINUOUS_MODE);

	//Data-ready signal on INT_DRDY pin.[set]
	lsm303agr_mag_drdy_on_pin_set(&dev_ctx_mg, PROPERTY_ENABLE);

	//Int1 enable
	lsm303agr_ctrl_reg3_a_t int1set;
	int1set.i1_aoi1 = PROPERTY_DISABLE;
	int1set.i1_drdy1 = PROPERTY_ENABLE;
	int1set.i1_click = PROPERTY_DISABLE;
	int1set.i1_drdy2 = PROPERTY_ENABLE;
	int1set.i1_overrun = PROPERTY_DISABLE;

	lsm303agr_xl_pin_int1_config_set(&dev_ctx_xl,&int1set);

	return 0;

}

static void LSM303AGR_Read_All(void){

	axis3bit16_t data_raw_acceleration;
	axis3bit16_t data_raw_magnetic;
	axis1bit16_t data_raw_temperature;

	/* Read output only if new value is available */
	lsm303agr_reg_t reg;
	lsm303agr_xl_status_get(&dev_ctx_xl, &reg.status_reg_a);

	if (reg.status_reg_a.zyxda)
	{
		/* clear interrupt */
		lsm303agr_ctrl_reg3_a_t var;
		lsm303agr_xl_pin_int1_config_get(&dev_ctx_xl,&var);

		/* Read accelerometer data */
		memset(data_raw_acceleration.u8bit, 0x00, 3*sizeof(int16_t));
		lsm303agr_acceleration_raw_get(&dev_ctx_xl, data_raw_acceleration.u8bit);
		Sen_dt_b.acceleration_mg[0] = lsm303agr_from_fs_2g_hr_to_mg( data_raw_acceleration.i16bit[0] );
		Sen_dt_b.acceleration_mg[1] = lsm303agr_from_fs_2g_hr_to_mg( data_raw_acceleration.i16bit[1] );
		Sen_dt_b.acceleration_mg[2] = lsm303agr_from_fs_2g_hr_to_mg( data_raw_acceleration.i16bit[2] );
	}

	lsm303agr_mag_status_get(&dev_ctx_mg, &reg.status_reg_m);
	if (reg.status_reg_m.zyxda)
	{
		/* Read magnetic field data */
		memset(data_raw_magnetic.u8bit, 0x00, 3*sizeof(int16_t));
		lsm303agr_magnetic_raw_get(&dev_ctx_mg, data_raw_magnetic.u8bit);
		Sen_dt_b.magnetic_mG[0] = lsm303agr_from_lsb_to_mgauss( data_raw_magnetic.i16bit[0]);
		Sen_dt_b.magnetic_mG[1] = lsm303agr_from_lsb_to_mgauss( data_raw_magnetic.i16bit[1]);
		Sen_dt_b.magnetic_mG[2] = lsm303agr_from_lsb_to_mgauss( data_raw_magnetic.i16bit[2]);
	}

	lsm303agr_temp_data_ready_get(&dev_ctx_xl, &reg.byte);
	if (reg.byte)
	{
		/* Read temperature data */
		memset(data_raw_temperature.u8bit, 0x00, sizeof(int16_t));
		lsm303agr_temperature_raw_get(&dev_ctx_xl, data_raw_temperature.u8bit);
		Sen_dt_b.temperature_degC = lsm303agr_from_lsb_hr_to_celsius( data_raw_temperature.i16bit );
	}

}


/*
 *   END LSM303AGR
 */




#pragma GCC push_options
#pragma GCC optimize ("O3")

static void Low_Timer_Callback( TimerHandle_t xTimer ){

	EVENTSETBIT(IntSensors_Event,EVT_LPS22HB | EVT_LSM303AGR | EVT_LSM6DSM );

}

static void PPG_Timer_Callback( TimerHandle_t xTimer ){

	EVENTSETBIT(IntSensors_Event, EVT_PPG);

}

void LPS22HB_Callback( void ){

	EVENTSETBIT(IntSensors_Event,EVT_LPS22HB);

}

void LSM303AGR_DRDY_Callback( void ){

	EVENTSETBIT(IntSensors_Event,EVT_LSM303AGR);

}

void LSM303AGR_INT1_Callback( void ){

	EVENTSETBIT(IntSensors_Event,EVT_LSM303AGR_INT1);

}

void LSM303AGR_INT2_Callback( void ){

	EVENTSETBIT(IntSensors_Event,EVT_LSM303AGR_INT2);

}

void LSM6DSM_Callback( void ){

	EVENTSETBIT(IntSensors_Event,EVT_LSM6DSM);

}

void MAX30101_Callback( void ){

	EVENTSETBIT(IntSensors_Event,EVT_PPG);

}



#pragma GCC pop_options
