
#ifndef INT_SENSORS_H_
#define INT_SENSORS_H_

typedef struct {
	/* lsm303agr */
	float acceleration_mg[3];
	float magnetic_mG[3];
	float temperature_degC;
	/* lps22hb */
	float pressure_hPa;
	float temp_degC;
	/* lsm6dsm */
	float acc_mg[3];
	float angular_rate_mdps[3];
	/* MAX3010 */
	uint32_t PPG_red;
	uint32_t PPG_IR;
	uint32_t PPG_green;
	float HeartRate;
	float avgHeartRate;
	int32_t spo2_HeartRate;
	int32_t spo2;
}Sensors_data_t;

extern Sensors_data_t Sen_dt_b;

typedef union{
  int16_t i16bit[3];
  uint8_t u8bit[6];
} axis3bit16_t;

typedef union{
  int16_t i16bit;
  uint8_t u8bit[2];
} axis1bit16_t;

typedef union{
  int32_t i32bit[3];
  uint8_t u8bit[12];
} axis3bit32_t;

typedef union{
  int32_t i32bit;
  uint8_t u8bit[4];
} axis1bit32_t;

void IntSensors_Task (void const * argument);
void LSM303AGR_DRDY_Callback( void );
void LSM303AGR_INT1_Callback( void );
void LSM303AGR_INT2_Callback( void );
void LPS22HB_Callback( void );
void LSM6DSM_Callback( void );
void MAX30101_Callback( void );

#endif /* INT_SENSORS_H_ */
