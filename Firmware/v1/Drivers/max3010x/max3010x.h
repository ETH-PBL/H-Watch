/*
 * max3010x.h
 *
 *  Created on: 1 ott 2020
 *      Author: tommy
 */

#ifndef MAX3010X_MAX3010X_H_
#define MAX3010X_MAX3010X_H_


/***************************************************
 This is a library written for the Maxim MAX30105 Optical Smoke Detector
 It should also work with the MAX30102. However, the MAX30102 does not have a Green LED.
 These sensors use I2C to communicate, as well as a single (optional)
 interrupt line that is not currently supported in this driver.

 Written by Peter Jansen and Nathan Seidle (SparkFun)
 BSD license, all text above must be included in any redistribution.
 *****************************************************/



#ifndef __weak
#define __weak   __attribute__((weak))
#endif /* __weak */
#ifndef __packed
#define __packed __attribute__((__packed__))
#endif /* __packed */



#define MAX30105_ADDRESS          0x57 //7-bit I2C Address
#define MAX30101_ADDRESS		  0xAE
//Note that MAX30102 has the same I2C address and Part ID

#define I2C_SPEED_STANDARD        100000
#define I2C_SPEED_FAST            400000



//The catch-all default is 32
#define I2C_BUFFER_LENGTH 32


#define STORAGE_SIZE 4 //Each long is 4 bytes so limit this to fit on your micro
typedef struct Record
{
	uint32_t red[STORAGE_SIZE];
	uint32_t IR[STORAGE_SIZE];
	uint32_t green[STORAGE_SIZE];
	uint8_t head;
	uint8_t tail;
} sense_struct; //This is our circular buffer of readings from the sensor


uint8_t MAX30105_Init(uint8_t (*readbyte)(uint8_t reg), void (*writebyte)(uint8_t reg, uint8_t value),
		void (*readN)( uint8_t reg, uint16_t size, uint8_t *data));


uint32_t MAX30105_getRed(void); //Returns immediate red value
uint32_t MAX30105_getIR(void); //Returns immediate IR value
uint32_t MAX30105_getGreen(void); //Returns immediate green value
uint8_t MAX30105_safeCheck(uint8_t maxTimeToCheck); //Given a max amount of time, check for new data

// Configuration
void MAX30105_softReset();
void MAX30105_shutDown();
void MAX30105_wakeUp();

void MAX30105_setLEDMode(uint8_t mode);

void MAX30105_setADCRange(uint8_t adcRange);
void MAX30105_setSampleRate(uint8_t sampleRate);
void MAX30105_setPulseWidth(uint8_t pulseWidth);

void MAX30105_setPulseAmplitudeRed(uint8_t value);
void MAX30105_setPulseAmplitudeIR(uint8_t value);
void MAX30105_setPulseAmplitudeGreen(uint8_t value);
void MAX30105_setPulseAmplitudeProximity(uint8_t value);

void MAX30105_setProximityThreshold(uint8_t threshMSB);

//Multi-led configuration mode (page 22)
void MAX30105_enableSlot(uint8_t slotNumber, uint8_t device); //Given slot number, assign a device to slot
void MAX30105_disableSlots(void);

// Data Collection

//Interrupts (page 13, 14)
uint8_t MAX30105_getINT1(void); //Returns the main interrupt group
uint8_t MAX30105_getINT2(void); //Returns the temp ready interrupt
void MAX30105_enableAFULL(void); //Enable/disable individual interrupts
void MAX30105_disableAFULL(void);
void MAX30105_enableDATARDY(void);
void MAX30105_disableDATARDY(void);
void MAX30105_enableALCOVF(void);
void MAX30105_disableALCOVF(void);
void MAX30105_enablePROXINT(void);
void MAX30105_disablePROXINT(void);
void MAX30105_enableDIETEMPRDY(void);
void MAX30105_disableDIETEMPRDY(void);

//FIFO Configuration (page 18)
void MAX30105_setFIFOAverage(uint8_t samples);
void MAX30105_enableFIFORollover();
void MAX30105_disableFIFORollover();
void MAX30105_setFIFOAlmostFull(uint8_t samples);

//FIFO Reading
uint16_t MAX30105_check(void); //Checks for new data and fills FIFO
uint8_t MAX30105_available(void); //Tells caller how many new samples are available (head - tail)
void MAX30105_nextSample(void); //Advances the tail of the sense array
uint32_t MAX30105_getFIFORed(void); //Returns the FIFO sample pointed to by tail
uint32_t MAX30105_getFIFOIR(void); //Returns the FIFO sample pointed to by tail
uint32_t MAX30105_getFIFOGreen(void); //Returns the FIFO sample pointed to by tail

uint8_t MAX30105_getWritePointer(void);
uint8_t MAX30105_getReadPointer(void);
void MAX30105_clearFIFO(void); //Sets the read/write pointers to zero

//Proximity Mode Interrupt Threshold
void MAX30105_setPROXINTTHRESH(uint8_t val);

// Die Temperature
float MAX30105_readTemperature();
float MAX30105_readTemperatureF();

// Detecting ID/Revision
uint8_t MAX30105_getRevisionID();
uint8_t MAX30105_readPartID();


void MAX30105_readRevisionID();

void MAX30105_bitMask(uint8_t reg, uint8_t mask, uint8_t thing);

void MAX30105_setup(uint8_t powerLevel, uint8_t sampleAverage, uint8_t ledMode, int sampleRate, int pulseWidth, int adcRange);






#endif /* MAX3010X_MAX3010X_H_ */
