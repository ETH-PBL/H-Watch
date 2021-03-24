/*
 * max3010x.c
 *
 *  Created on: 1 ott 2020
 *      Author: Tommaso Polonelli
 */

/***************************************************
  This is a library written for the Maxim MAX30105 Optical Smoke Detector
  It should also work with the MAX30102. However, the MAX30102 does not have a Green LED.
  These sensors use I2C to communicate, as well as a single (optional)
  interrupt line that is not currently supported in this driver.
  Written by Peter Jansen and Nathan Seidle (SparkFun)
  BSD license, all text above must be included in any redistribution.
 *****************************************************/

#include "main.h"
#include "max3010x.h"

#include <string.h>

#define millis() HAL_GetTick()
#define delay(x) HAL_Delay(x)

// Status Registers
static const uint8_t MAX30105_INTSTAT1 =		0x00;
static const uint8_t MAX30105_INTSTAT2 =		0x01;
static const uint8_t MAX30105_INTENABLE1 =		0x02;
static const uint8_t MAX30105_INTENABLE2 =		0x03;

// FIFO Registers
static const uint8_t MAX30105_FIFOWRITEPTR = 	0x04;
static const uint8_t MAX30105_FIFOOVERFLOW = 	0x05;
static const uint8_t MAX30105_FIFOREADPTR = 	0x06;
static const uint8_t MAX30105_FIFODATA =		0x07;

// Configuration Registers
static const uint8_t MAX30105_FIFOCONFIG = 		0x08;
static const uint8_t MAX30105_MODECONFIG = 		0x09;
static const uint8_t MAX30105_PARTICLECONFIG = 	0x0A;    // Note, sometimes listed as "SPO2" config in datasheet (pg. 11)
static const uint8_t MAX30105_LED1_PULSEAMP = 	0x0C;
static const uint8_t MAX30105_LED2_PULSEAMP = 	0x0D;
static const uint8_t MAX30105_LED3_PULSEAMP = 	0x0E;
static const uint8_t MAX30105_LED_PROX_AMP = 	0x10;
static const uint8_t MAX30105_MULTILEDCONFIG1 = 0x11;
static const uint8_t MAX30105_MULTILEDCONFIG2 = 0x12;

// Die Temperature Registers
static const uint8_t MAX30105_DIETEMPINT = 		0x1F;
static const uint8_t MAX30105_DIETEMPFRAC = 	0x20;
static const uint8_t MAX30105_DIETEMPCONFIG = 	0x21;

// Proximity Function Registers
static const uint8_t MAX30105_PROXINTTHRESH = 	0x30;

// Part ID Registers
static const uint8_t MAX30105_REVISIONID = 		0xFE;
static const uint8_t MAX30105_PARTID = 			0xFF;    // Should always be 0x15. Identical to MAX30102.

// MAX30105 Commands
// Interrupt configuration (pg 13, 14)
static const uint8_t MAX30105_INT_A_FULL_MASK =		(uint8_t)~0b10000000;
static const uint8_t MAX30105_INT_A_FULL_ENABLE = 	0x80;
static const uint8_t MAX30105_INT_A_FULL_DISABLE = 	0x00;

static const uint8_t MAX30105_INT_DATA_RDY_MASK = (uint8_t)~0b01000000;
static const uint8_t MAX30105_INT_DATA_RDY_ENABLE =	0x40;
static const uint8_t MAX30105_INT_DATA_RDY_DISABLE = 0x00;

static const uint8_t MAX30105_INT_ALC_OVF_MASK = (uint8_t)~0b00100000;
static const uint8_t MAX30105_INT_ALC_OVF_ENABLE = 	0x20;
static const uint8_t MAX30105_INT_ALC_OVF_DISABLE = 0x00;

static const uint8_t MAX30105_INT_PROX_INT_MASK = (uint8_t)~0b00010000;
static const uint8_t MAX30105_INT_PROX_INT_ENABLE = 0x10;
static const uint8_t MAX30105_INT_PROX_INT_DISABLE = 0x00;

static const uint8_t MAX30105_INT_DIE_TEMP_RDY_MASK = (uint8_t)~0b00000010;
static const uint8_t MAX30105_INT_DIE_TEMP_RDY_ENABLE = 0x02;
static const uint8_t MAX30105_INT_DIE_TEMP_RDY_DISABLE = 0x00;

static const uint8_t MAX30105_SAMPLEAVG_MASK =	(uint8_t)~0b11100000;
static const uint8_t MAX30105_SAMPLEAVG_1 = 	0x00;
static const uint8_t MAX30105_SAMPLEAVG_2 = 	0x20;
static const uint8_t MAX30105_SAMPLEAVG_4 = 	0x40;
static const uint8_t MAX30105_SAMPLEAVG_8 = 	0x60;
static const uint8_t MAX30105_SAMPLEAVG_16 = 	0x80;
static const uint8_t MAX30105_SAMPLEAVG_32 = 	0xA0;

static const uint8_t MAX30105_ROLLOVER_MASK = 	0xEF;
static const uint8_t MAX30105_ROLLOVER_ENABLE = 0x10;
static const uint8_t MAX30105_ROLLOVER_DISABLE = 0x00;

static const uint8_t MAX30105_A_FULL_MASK = 	0xF0;

// Mode configuration commands (page 19)
static const uint8_t MAX30105_SHUTDOWN_MASK = 	0x7F;
static const uint8_t MAX30105_SHUTDOWN = 		0x80;
static const uint8_t MAX30105_WAKEUP = 			0x00;

static const uint8_t MAX30105_RESET_MASK = 		0xBF;
static const uint8_t MAX30105_RESET = 			0x40;

static const uint8_t MAX30105_MODE_MASK = 		0xF8;
static const uint8_t MAX30105_MODE_REDONLY = 	0x02;
static const uint8_t MAX30105_MODE_REDIRONLY = 	0x03;
static const uint8_t MAX30105_MODE_MULTILED = 	0x07;

// Particle sensing configuration commands (pgs 19-20)
static const uint8_t MAX30105_ADCRANGE_MASK = 	0x9F;
static const uint8_t MAX30105_ADCRANGE_2048 = 	0x00;
static const uint8_t MAX30105_ADCRANGE_4096 = 	0x20;
static const uint8_t MAX30105_ADCRANGE_8192 = 	0x40;
static const uint8_t MAX30105_ADCRANGE_16384 = 	0x60;

static const uint8_t MAX30105_SAMPLERATE_MASK = 0xE3;
static const uint8_t MAX30105_SAMPLERATE_50 = 	0x00;
static const uint8_t MAX30105_SAMPLERATE_100 = 	0x04;
static const uint8_t MAX30105_SAMPLERATE_200 = 	0x08;
static const uint8_t MAX30105_SAMPLERATE_400 = 	0x0C;
static const uint8_t MAX30105_SAMPLERATE_800 = 	0x10;
static const uint8_t MAX30105_SAMPLERATE_1000 = 0x14;
static const uint8_t MAX30105_SAMPLERATE_1600 = 0x18;
static const uint8_t MAX30105_SAMPLERATE_3200 = 0x1C;

static const uint8_t MAX30105_PULSEWIDTH_MASK = 0xFC;
static const uint8_t MAX30105_PULSEWIDTH_69 = 	0x00;
static const uint8_t MAX30105_PULSEWIDTH_118 = 	0x01;
static const uint8_t MAX30105_PULSEWIDTH_215 = 	0x02;
static const uint8_t MAX30105_PULSEWIDTH_411 = 	0x03;

//Multi-LED Mode configuration (pg 22)
static const uint8_t MAX30105_SLOT1_MASK = 		0xF8;
static const uint8_t MAX30105_SLOT2_MASK = 		0x8F;
static const uint8_t MAX30105_SLOT3_MASK = 		0xF8;
static const uint8_t MAX30105_SLOT4_MASK = 		0x8F;

const uint8_t SLOT_NONE = 				0x00;
const uint8_t SLOT_RED_LED = 			0x01;
const uint8_t SLOT_IR_LED = 			0x02;
const uint8_t SLOT_GREEN_LED = 			0x03;
const uint8_t SLOT_NONE_PILOT = 		0x04;
const uint8_t SLOT_RED_PILOT =			0x05;
const uint8_t SLOT_IR_PILOT = 			0x06;
const uint8_t SLOT_GREEN_PILOT = 		0x07;

static const uint8_t MAX_30105_EXPECTEDPARTID = 0x15;

//activeLEDs is the number of channels turned on, and can be 1 to 3. 2 is common for Red+IR.
uint8_t activeLEDs; //Gets set during setup. Allows check() to calculate how many bytes to read from FIFO

uint8_t revisionID;

sense_struct sense;

/* functions ptrs */
static uint8_t (*readRegister8)(uint8_t reg);
static void (*writeRegister8)(uint8_t reg, uint8_t value);
static void (*readRegisterN)( uint8_t reg, uint16_t size, uint8_t *data);


//
// Low-level I2C Communication
//
__weak uint8_t MAX30105_readRegister8( uint8_t reg) {
	/*_i2cPort->beginTransmission(address);
	_i2cPort->write(reg);
	_i2cPort->endTransmission(false);

	_i2cPort->requestFrom((uint8_t)address, (uint8_t)1); // Request 1 byte
	if (_i2cPort->available())
	{
		return(_i2cPort->read());
	}
*/
	return (0); //Fail

}

__weak uint8_t MAX30105_readRegisterN( uint8_t reg, uint16_t size) {
	/*_i2cPort->beginTransmission(address);
	_i2cPort->write(reg);
	_i2cPort->endTransmission(false);

	_i2cPort->requestFrom((uint8_t)address, (uint8_t)1); // Request 1 byte
	if (_i2cPort->available())
	{
		return(_i2cPort->read());
	}
*/
	return (0); //Fail

}

__weak void MAX30105_writeRegister8( uint8_t reg, uint8_t value) {
	/*_i2cPort->beginTransmission(address);
	_i2cPort->write(reg);
	_i2cPort->write(value);
	_i2cPort->endTransmission();*/
}



uint8_t MAX30105_Init(uint8_t (*readbyte)(uint8_t reg), void (*writebyte)(uint8_t reg, uint8_t value), void (*readN)( uint8_t reg, uint16_t size, uint8_t *data)) {

	readRegister8 = readbyte;
	writeRegister8 = writebyte;
	readRegisterN = readN;

	// Step 1: Initial Communication and Verification
	// Check that a MAX30105 is connected
	if (MAX30105_readPartID() != MAX_30105_EXPECTEDPARTID) {
		// Error -- Part ID read from MAX30105 does not match expected part ID.
		// This may mean there is a physical connectivity problem (broken wire, unpowered, etc).
		return 1;
	}

	// Populate revision ID
	MAX30105_readRevisionID();

	return 0;

}

//
// Configuration
//

//Begin Interrupt configuration
uint8_t MAX30105_getINT1(void) {
	return (readRegister8(MAX30105_INTSTAT1));
}
uint8_t MAX30105_getINT2(void) {
	return (readRegister8(MAX30105_INTSTAT2));
}

void MAX30105_enableAFULL(void) {
	MAX30105_bitMask(MAX30105_INTENABLE1, MAX30105_INT_A_FULL_MASK, MAX30105_INT_A_FULL_ENABLE);
}
void MAX30105_disableAFULL(void) {
	MAX30105_bitMask(MAX30105_INTENABLE1, MAX30105_INT_A_FULL_MASK, MAX30105_INT_A_FULL_DISABLE);
}

void MAX30105_enableDATARDY(void) {
	MAX30105_bitMask(MAX30105_INTENABLE1, MAX30105_INT_DATA_RDY_MASK, MAX30105_INT_DATA_RDY_ENABLE);
}
void MAX30105_disableDATARDY(void) {
	MAX30105_bitMask(MAX30105_INTENABLE1, MAX30105_INT_DATA_RDY_MASK, MAX30105_INT_DATA_RDY_DISABLE);
}

void MAX30105_enableALCOVF(void) {
	MAX30105_bitMask(MAX30105_INTENABLE1, MAX30105_INT_ALC_OVF_MASK, MAX30105_INT_ALC_OVF_ENABLE);
}
void MAX30105_disableALCOVF(void) {
	MAX30105_bitMask(MAX30105_INTENABLE1, MAX30105_INT_ALC_OVF_MASK, MAX30105_INT_ALC_OVF_DISABLE);
}

void MAX30105_enablePROXINT(void) {
	MAX30105_bitMask(MAX30105_INTENABLE1, MAX30105_INT_PROX_INT_MASK, MAX30105_INT_PROX_INT_ENABLE);
}
void MAX30105_disablePROXINT(void) {
	MAX30105_bitMask(MAX30105_INTENABLE1, MAX30105_INT_PROX_INT_MASK, MAX30105_INT_PROX_INT_DISABLE);
}

void MAX30105_enableDIETEMPRDY(void) {
	MAX30105_bitMask(MAX30105_INTENABLE2, MAX30105_INT_DIE_TEMP_RDY_MASK, MAX30105_INT_DIE_TEMP_RDY_ENABLE);
}
void MAX30105_disableDIETEMPRDY(void) {
	MAX30105_bitMask(MAX30105_INTENABLE2, MAX30105_INT_DIE_TEMP_RDY_MASK, MAX30105_INT_DIE_TEMP_RDY_DISABLE);
}

//End Interrupt configuration

void MAX30105_softReset(void) {
	MAX30105_bitMask(MAX30105_MODECONFIG, MAX30105_RESET_MASK, MAX30105_RESET);

	// Poll for bit to clear, reset is then complete
	// Timeout after 100ms
	unsigned long startTime = millis();
	while (millis() - startTime < 100)
	{
		uint8_t response = readRegister8(MAX30105_MODECONFIG);
		if ((response & MAX30105_RESET) == 0) break; //We're done!
		delay(1); //Let's not over burden the I2C bus
	}
}

void MAX30105_shutDown(void) {
	// Put IC into low power mode (datasheet pg. 19)
	// During shutdown the IC will continue to respond to I2C commands but will
	// not update with or take new readings (such as temperature)
	MAX30105_bitMask(MAX30105_MODECONFIG, MAX30105_SHUTDOWN_MASK, MAX30105_SHUTDOWN);
}

void MAX30105_wakeUp(void) {
	// Pull IC out of low power mode (datasheet pg. 19)
	MAX30105_bitMask(MAX30105_MODECONFIG, MAX30105_SHUTDOWN_MASK, MAX30105_WAKEUP);
}

void MAX30105_setLEDMode(uint8_t mode) {
	// Set which LEDs are used for sampling -- Red only, RED+IR only, or custom.
	// See datasheet, page 19
	MAX30105_bitMask(MAX30105_MODECONFIG, MAX30105_MODE_MASK, mode);
}

void MAX30105_setADCRange(uint8_t adcRange) {
	// adcRange: one of MAX30105_ADCRANGE_2048, _4096, _8192, _16384
	MAX30105_bitMask(MAX30105_PARTICLECONFIG, MAX30105_ADCRANGE_MASK, adcRange);
}

void MAX30105_setSampleRate(uint8_t sampleRate) {
	// sampleRate: one of MAX30105_SAMPLERATE_50, _100, _200, _400, _800, _1000, _1600, _3200
	MAX30105_bitMask(MAX30105_PARTICLECONFIG, MAX30105_SAMPLERATE_MASK, sampleRate);
}

void MAX30105_setPulseWidth(uint8_t pulseWidth) {
	// pulseWidth: one of MAX30105_PULSEWIDTH_69, _188, _215, _411
	MAX30105_bitMask(MAX30105_PARTICLECONFIG, MAX30105_PULSEWIDTH_MASK, pulseWidth);
}

// NOTE: Amplitude values: 0x00 = 0mA, 0x7F = 25.4mA, 0xFF = 50mA (typical)
// See datasheet, page 21
void MAX30105_setPulseAmplitudeRed(uint8_t amplitude) {
	writeRegister8(MAX30105_LED1_PULSEAMP, amplitude);
}

void MAX30105_setPulseAmplitudeIR(uint8_t amplitude) {
	writeRegister8(MAX30105_LED2_PULSEAMP, amplitude);
}

void MAX30105_setPulseAmplitudeGreen(uint8_t amplitude) {
	writeRegister8(MAX30105_LED3_PULSEAMP, amplitude);
}

void MAX30105_setPulseAmplitudeProximity(uint8_t amplitude) {
	writeRegister8(MAX30105_LED_PROX_AMP, amplitude);
}

void MAX30105_setProximityThreshold(uint8_t threshMSB) {
	// Set the IR ADC count that will trigger the beginning of particle-sensing mode.
	// The threshMSB signifies only the 8 most significant-bits of the ADC count.
	// See datasheet, page 24.
	writeRegister8(MAX30105_PROXINTTHRESH, threshMSB);
}

//Given a slot number assign a thing to it
//Devices are SLOT_RED_LED or SLOT_RED_PILOT (proximity)
//Assigning a SLOT_RED_LED will pulse LED
//Assigning a SLOT_RED_PILOT will ??
void MAX30105_enableSlot(uint8_t slotNumber, uint8_t device) {

	//uint8_t originalContents;

	switch (slotNumber) {
	case (1):
		MAX30105_bitMask(MAX30105_MULTILEDCONFIG1, MAX30105_SLOT1_MASK, device);
	break;
	case (2):
		MAX30105_bitMask(MAX30105_MULTILEDCONFIG1, MAX30105_SLOT2_MASK, device << 4);
	break;
	case (3):
		MAX30105_bitMask(MAX30105_MULTILEDCONFIG2, MAX30105_SLOT3_MASK, device);
	break;
	case (4):
		MAX30105_bitMask(MAX30105_MULTILEDCONFIG2, MAX30105_SLOT4_MASK, device << 4);
	break;
	default:
		//Shouldn't be here!
		break;
	}
}

//Clears all slot assignments
void MAX30105_disableSlots(void) {
	writeRegister8(MAX30105_MULTILEDCONFIG1, 0);
	writeRegister8(MAX30105_MULTILEDCONFIG2, 0);
}

//
// FIFO Configuration
//

//Set sample average (Table 3, Page 18)
void MAX30105_setFIFOAverage(uint8_t numberOfSamples) {
	MAX30105_bitMask(MAX30105_FIFOCONFIG, MAX30105_SAMPLEAVG_MASK, numberOfSamples);
}

//Resets all points to start in a known state
//Page 15 recommends clearing FIFO before beginning a read
void MAX30105_clearFIFO(void) {
	writeRegister8(MAX30105_FIFOWRITEPTR, 0);
	writeRegister8(MAX30105_FIFOOVERFLOW, 0);
	writeRegister8(MAX30105_FIFOREADPTR, 0);
}

//Enable roll over if FIFO over flows
void MAX30105_enableFIFORollover(void) {
	MAX30105_bitMask(MAX30105_FIFOCONFIG, MAX30105_ROLLOVER_MASK, MAX30105_ROLLOVER_ENABLE);
}

//Disable roll over if FIFO over flows
void MAX30105_disableFIFORollover(void) {
	MAX30105_bitMask(MAX30105_FIFOCONFIG, MAX30105_ROLLOVER_MASK, MAX30105_ROLLOVER_DISABLE);
}

//Set number of samples to trigger the almost full interrupt (Page 18)
//Power on default is 32 samples
//Note it is reverse: 0x00 is 32 samples, 0x0F is 17 samples
void MAX30105_setFIFOAlmostFull(uint8_t numberOfSamples) {
	MAX30105_bitMask(MAX30105_FIFOCONFIG, MAX30105_A_FULL_MASK, numberOfSamples);
}

//Read the FIFO Write Pointer
uint8_t MAX30105_getWritePointer(void) {
	return (readRegister8(MAX30105_FIFOWRITEPTR));
}

//Read the FIFO Read Pointer
uint8_t MAX30105_getReadPointer(void) {
	return (readRegister8(MAX30105_FIFOREADPTR));
}


// Die Temperature
// Returns temp in C
float MAX30105_readTemperature() {

	//DIE_TEMP_RDY interrupt must be enabled
	//See issue 19: https://github.com/sparkfun/SparkFun_MAX3010x_Sensor_Library/issues/19

	// Step 1: Config die temperature register to take 1 temperature sample
	writeRegister8(MAX30105_DIETEMPCONFIG, 0x01);

	// Poll for bit to clear, reading is then complete
	// Timeout after 100ms
	unsigned long startTime = millis();
	while (millis() - startTime < 100)
	{
		//uint8_t response = readRegister8(_i2caddr, MAX30105_DIETEMPCONFIG); //Original way
		//if ((response & 0x01) == 0) break; //We're done!

		//Check to see if DIE_TEMP_RDY interrupt is set
		uint8_t response = readRegister8(MAX30105_INTSTAT2);
		if ((response & MAX30105_INT_DIE_TEMP_RDY_ENABLE) > 0) break; //We're done!
		delay(1); //Let's not over burden the I2C bus
	}
	//TODO How do we want to fail? With what type of error?
	//? if(millis() - startTime >= 100) return(-999.0);

	// Step 2: Read die temperature register (integer)
	int8_t tempInt = readRegister8(MAX30105_DIETEMPINT);
	uint8_t tempFrac = readRegister8(MAX30105_DIETEMPFRAC); //Causes the clearing of the DIE_TEMP_RDY interrupt

	// Step 3: Calculate temperature (datasheet pg. 23)
	return (float)tempInt + ((float)tempFrac * 0.0625);
}

// Returns die temp in F
float MAX30105_readTemperatureF() {
	float temp = MAX30105_readTemperature();

	if (temp != -999.0) temp = temp * 1.8 + 32.0;

	return (temp);
}

// Set the PROX_INT_THRESHold
void MAX30105_setPROXINTTHRESH(uint8_t val) {
	writeRegister8(MAX30105_PROXINTTHRESH, val);
}


//
// Device ID and Revision
//
uint8_t MAX30105_readPartID() {
	return readRegister8(MAX30105_PARTID);
}

void MAX30105_readRevisionID() {
	revisionID = readRegister8(MAX30105_REVISIONID);
}

uint8_t MAX30105_getRevisionID() {
	return revisionID;
}


//Setup the sensor
//The MAX30105 has many settings. By default we select:
// Sample Average = 4
// Mode = MultiLED
// ADC Range = 16384 (62.5pA per LSB)
// Sample rate = 50
//Use the default setup if you are just getting started with the MAX30105 sensor
void MAX30105_setup(uint8_t powerLevel, uint8_t sampleAverage, uint8_t ledMode, int sampleRate, int pulseWidth, int adcRange) {
	MAX30105_softReset(); //Reset all configuration, threshold, and data registers to POR values

	//FIFO Configuration
	//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	//The chip will average multiple samples of same type together if you wish
	if (sampleAverage == 1) MAX30105_setFIFOAverage(MAX30105_SAMPLEAVG_1); //No averaging per FIFO record
	else if (sampleAverage == 2) MAX30105_setFIFOAverage(MAX30105_SAMPLEAVG_2);
	else if (sampleAverage == 4) MAX30105_setFIFOAverage(MAX30105_SAMPLEAVG_4);
	else if (sampleAverage == 8) MAX30105_setFIFOAverage(MAX30105_SAMPLEAVG_8);
	else if (sampleAverage == 16) MAX30105_setFIFOAverage(MAX30105_SAMPLEAVG_16);
	else if (sampleAverage == 32) MAX30105_setFIFOAverage(MAX30105_SAMPLEAVG_32);
	else MAX30105_setFIFOAverage(MAX30105_SAMPLEAVG_4);

	//setFIFOAlmostFull(2); //Set to 30 samples to trigger an 'Almost Full' interrupt
	MAX30105_enableFIFORollover(); //Allow FIFO to wrap/roll over
	//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

	//Mode Configuration
	//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	if (ledMode == 3) MAX30105_setLEDMode(MAX30105_MODE_MULTILED); //Watch all three LED channels
	else if (ledMode == 2) MAX30105_setLEDMode(MAX30105_MODE_REDIRONLY); //Red and IR
	else MAX30105_setLEDMode(MAX30105_MODE_REDONLY); //Red only
	activeLEDs = ledMode; //Used to control how many bytes to read from FIFO buffer
	//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

	//Particle Sensing Configuration
	//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	if(adcRange < 4096) MAX30105_setADCRange(MAX30105_ADCRANGE_2048); //7.81pA per LSB
	else if(adcRange < 8192) MAX30105_setADCRange(MAX30105_ADCRANGE_4096); //15.63pA per LSB
	else if(adcRange < 16384) MAX30105_setADCRange(MAX30105_ADCRANGE_8192); //31.25pA per LSB
	else if(adcRange == 16384) MAX30105_setADCRange(MAX30105_ADCRANGE_16384); //62.5pA per LSB
	else MAX30105_setADCRange(MAX30105_ADCRANGE_2048);

	if (sampleRate < 100) MAX30105_setSampleRate(MAX30105_SAMPLERATE_50); //Take 50 samples per second
	else if (sampleRate < 200) MAX30105_setSampleRate(MAX30105_SAMPLERATE_100);
	else if (sampleRate < 400) MAX30105_setSampleRate(MAX30105_SAMPLERATE_200);
	else if (sampleRate < 800) MAX30105_setSampleRate(MAX30105_SAMPLERATE_400);
	else if (sampleRate < 1000) MAX30105_setSampleRate(MAX30105_SAMPLERATE_800);
	else if (sampleRate < 1600) MAX30105_setSampleRate(MAX30105_SAMPLERATE_1000);
	else if (sampleRate < 3200) MAX30105_setSampleRate(MAX30105_SAMPLERATE_1600);
	else if (sampleRate == 3200) MAX30105_setSampleRate(MAX30105_SAMPLERATE_3200);
	else MAX30105_setSampleRate(MAX30105_SAMPLERATE_50);

	//The longer the pulse width the longer range of detection you'll have
	//At 69us and 0.4mA it's about 2 inches
	//At 411us and 0.4mA it's about 6 inches
	if (pulseWidth < 118) MAX30105_setPulseWidth(MAX30105_PULSEWIDTH_69); //Page 26, Gets us 15 bit resolution
	else if (pulseWidth < 215) MAX30105_setPulseWidth(MAX30105_PULSEWIDTH_118); //16 bit resolution
	else if (pulseWidth < 411) MAX30105_setPulseWidth(MAX30105_PULSEWIDTH_215); //17 bit resolution
	else if (pulseWidth == 411) MAX30105_setPulseWidth(MAX30105_PULSEWIDTH_411); //18 bit resolution
	else MAX30105_setPulseWidth(MAX30105_PULSEWIDTH_69);
	//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

	//LED Pulse Amplitude Configuration
	//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	//Default is 0x1F which gets us 6.4mA
	//powerLevel = 0x02, 0.4mA - Presence detection of ~4 inch
	//powerLevel = 0x1F, 6.4mA - Presence detection of ~8 inch
	//powerLevel = 0x7F, 25.4mA - Presence detection of ~8 inch
	//powerLevel = 0xFF, 50.0mA - Presence detection of ~12 inch

	MAX30105_setPulseAmplitudeRed(powerLevel);
	MAX30105_setPulseAmplitudeIR(powerLevel);
	MAX30105_setPulseAmplitudeGreen(powerLevel);
	MAX30105_setPulseAmplitudeProximity(powerLevel);
	//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

	//Multi-LED Mode Configuration, Enable the reading of the three LEDs
	//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	MAX30105_enableSlot(1, SLOT_RED_LED);
	if (ledMode > 1) MAX30105_enableSlot(2, SLOT_IR_LED);
	if (ledMode > 2) MAX30105_enableSlot(3, SLOT_GREEN_LED);
	//enableSlot(1, SLOT_RED_PILOT);
	//enableSlot(2, SLOT_IR_PILOT);
	//enableSlot(3, SLOT_GREEN_PILOT);
	//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

	MAX30105_clearFIFO(); //Reset the FIFO before we begin checking the sensor
}

//
// Data Collection
//

//Tell caller how many samples are available
uint8_t MAX30105_available(void)
{
	int8_t numberOfSamples = sense.head - sense.tail;
	if (numberOfSamples < 0) numberOfSamples += STORAGE_SIZE;

	return (numberOfSamples);
}

//Report the most recent red value
uint32_t MAX30105_getRed(void)
{
	//Check the sensor for new data for 250ms
	if(MAX30105_safeCheck(250))
		return (sense.red[sense.head]);
	else
		return(0); //Sensor failed to find new data
}

//Report the most recent IR value
uint32_t MAX30105_getIR(void)
{
	//Check the sensor for new data for 250ms
	if(MAX30105_safeCheck(250))
		return (sense.IR[sense.head]);
	else
		return(0); //Sensor failed to find new data
}

//Report the most recent Green value
uint32_t MAX30105_getGreen(void)
{
	//Check the sensor for new data for 250ms
	if(MAX30105_safeCheck(250))
		return (sense.green[sense.head]);
	else
		return(0); //Sensor failed to find new data
}

//Report the next Red value in the FIFO
uint32_t MAX30105_getFIFORed(void)
{
	return (sense.red[sense.tail]);
}

//Report the next IR value in the FIFO
uint32_t MAX30105_getFIFOIR(void)
{
	return (sense.IR[sense.tail]);
}

//Report the next Green value in the FIFO
uint32_t MAX30105_getFIFOGreen(void)
{
	return (sense.green[sense.tail]);
}

//Advance the tail
void MAX30105_nextSample(void)
{
	if(MAX30105_available()) //Only advance the tail if new data is available
	{
		sense.tail++;
		sense.tail %= STORAGE_SIZE; //Wrap condition
	}
}

//Polls the sensor for new data
//Call regularly
//If new data is available, it updates the head and tail in the main struct
//Returns number of new samples obtained
uint16_t MAX30105_check(void)
{
	uint8_t data[300], dt_p=0;
	//Read register FIDO_DATA in (3-byte * number of active LED) chunks
	//Until FIFO_RD_PTR = FIFO_WR_PTR

	uint8_t readPointer = MAX30105_getReadPointer();
	uint8_t writePointer = MAX30105_getWritePointer();

	int numberOfSamples = 0;

	//Do we have new data?
	if (readPointer != writePointer)
	{
		//Calculate the number of readings we need to get from sensor
		numberOfSamples = writePointer - readPointer;
		if (numberOfSamples < 0) numberOfSamples += 32; //Wrap condition

		//We now have the number of readings, now calc bytes to read
		//For this example we are just doing Red and IR (3 bytes each)
		int bytesLeftToRead = numberOfSamples * activeLEDs * 3;

		//Get ready to read a burst of data from the FIFO register
		/*_i2cPort->beginTransmission(MAX30105_ADDRESS);
		_i2cPort->write(MAX30105_FIFODATA);
		_i2cPort->endTransmission();*/
		readRegisterN( MAX30105_FIFODATA, bytesLeftToRead, data);

		//We may need to read as many as 288 bytes so we read in blocks no larger than I2C_BUFFER_LENGTH
		//I2C_BUFFER_LENGTH changes based on the platform. 64 bytes for SAMD21, 32 bytes for Uno.
		//Wire.requestFrom() is limited to BUFFER_LENGTH which is 32 on the Uno
		while (bytesLeftToRead > 0)
		{
			int toGet = bytesLeftToRead;
			if (toGet > I2C_BUFFER_LENGTH)
			{
				//If toGet is 32 this is bad because we read 6 bytes (Red+IR * 3 = 6) at a time
				//32 % 6 = 2 left over. We don't want to request 32 bytes, we want to request 30.
				//32 % 9 (Red+IR+GREEN) = 5 left over. We want to request 27.

				toGet = I2C_BUFFER_LENGTH - (I2C_BUFFER_LENGTH % (activeLEDs * 3)); //Trim toGet to be a multiple of the samples we need to read
			}

			bytesLeftToRead -= toGet;

			//Request toGet number of bytes from sensor
			//_i2cPort->requestFrom(MAX30105_ADDRESS, toGet);

			while (toGet > 0)
			{
				sense.head++; //Advance the head of the storage struct
				sense.head %= STORAGE_SIZE; //Wrap condition

				uint8_t temp[sizeof(uint32_t)]; //Array of 4 bytes that we will convert into long
				uint32_t tempLong;

				//Burst read three bytes - RED
				temp[3] = 0;
				temp[2] = data[dt_p++];//_i2cPort->read();
				temp[1] = data[dt_p++];//_i2cPort->read();
				temp[0] = data[dt_p++];//_i2cPort->read();

				//Convert array to long
				memcpy(&tempLong, temp, sizeof(tempLong));

				tempLong &= 0x3FFFF; //Zero out all but 18 bits

				sense.red[sense.head] = tempLong; //Store this reading into the sense array

				if (activeLEDs > 1)
				{
					//Burst read three more bytes - IR
					temp[3] = 0;
					temp[2] = data[dt_p++];//_i2cPort->read();
					temp[1] = data[dt_p++];//_i2cPort->read();
					temp[0] = data[dt_p++];//_i2cPort->read();

					//Convert array to long
					memcpy(&tempLong, temp, sizeof(tempLong));

					tempLong &= 0x3FFFF; //Zero out all but 18 bits

					sense.IR[sense.head] = tempLong;
				}

				if (activeLEDs > 2)
				{
					//Burst read three more bytes - Green
					temp[3] = 0;
					temp[2] = data[dt_p++];//_i2cPort->read();
					temp[1] = data[dt_p++];//_i2cPort->read();
					temp[0] = data[dt_p++];//_i2cPort->read();

					//Convert array to long
					memcpy(&tempLong, temp, sizeof(tempLong));

					tempLong &= 0x3FFFF; //Zero out all but 18 bits

					sense.green[sense.head] = tempLong;
				}

				toGet -= activeLEDs * 3;
			}

		} //End while (bytesLeftToRead > 0)

	} //End readPtr != writePtr

	return (numberOfSamples); //Let the world know how much new data we found
}

//Check for new data but give up after a certain amount of time
//Returns true if new data was found
//Returns false if new data was not found
uint8_t MAX30105_safeCheck(uint8_t maxTimeToCheck)
{
	uint32_t markTime = millis();

	while(1)
	{
		if(millis() - markTime > maxTimeToCheck) return(0);

		if(MAX30105_check() == 1) //We found new data!
			return(1);

		delay(1);
	}
}

//Given a register, read it, mask it, and then set the thing
void MAX30105_bitMask(uint8_t reg, uint8_t mask, uint8_t thing)
{
	// Grab current register context
	uint8_t originalContents = readRegister8(reg);

	// Zero-out the portions of the register we're interested in
	originalContents = originalContents & mask;

	// Change contents
	writeRegister8(reg, originalContents | thing);
}
