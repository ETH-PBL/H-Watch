#ifndef __MACRO_H__
#define __MACRO_H__


/*! CPP guard */
#ifdef __cplusplus
extern "C" {
#endif


/* check the HEAP usage */
#define CHECK_HEAP_RUNTIME	(0)
/* enable SLEEP */
#define RTOS_SLEEP_EN		(0) //Disable sleep temperaily for debugging - Steve
/* enable DEEP_SLEEP */
#define DEEP_SLEEP_EN		(0) //Disable Deep_sleep temperaily for debugging - Steve


#define IS_EVT(x)					(x & uxBits)
#if 0
#define EVENTSETBIT(evt,x)	do{            									\
							BaseType_t xHiPTWok;  							\
							xHiPTWok = pdFALSE;								\
							xEventGroupSetBitsFromISR(evt,(x),&xHiPTWok ); 	\
							}while(0)
#endif
#define EVENTSETBIT(evt,x)	GLOB_SET_EVT(evt,x)

#if CHECK_HEAP_RUNTIME
#define CHECK_STACK_OF()			do{if(uxTaskGetStackHighWaterMark(osThreadGetId ()) < 20){while(1);}}while(0)
#else
#define CHECK_STACK_OF()			((void)0)
#endif


#ifndef TRUE
#define TRUE	(1)
#endif
#ifndef FALSE
#define FALSE	(0)
#endif


/*  				   	POWER TRANSFER     		    					  */

#define ADCCH_UWB_POWERTRANSF	Sen_dt_b[Sen_dt_p].extadc

/**************************************************************************/
/**************************************************************************/
/**************************************************************************/
/**************************************************************************/
/*  						DW CONFIGURATIONS 							  */
/**************************************************************************/
/**************************************************************************/
/**************************************************************************/
/**************************************************************************/

/* if 1 enable the continuous TX */
#define SWS_TXSPECT_MODE	0
/* if 1 enable the anchor mode */
#define SWS_ANC_MODE 		1

/* +======+======+======+======+=========+===========+=====+==========+ */
/* | S1-5 | S1-6 | S1-7 | Mode | Channel | Data Rate | PRF | Preamble | */
/* +======+======+======+======+=========+===========+=====+==========+ */
/* | OFF  | OFF  | OFF  |    1 |       2 | 110KBPS   |  16 |     1024 | */
/* +------+------+------+------+---------+-----------+-----+----------+ */
/* | ON   | OFF  | OFF  |    2 |       2 | 6.8MBPS   |  16 |      128 | */
/* +------+------+------+------+---------+-----------+-----+----------+ */
/* | OFF  | ON   | OFF  |    3 |       2 | 110KBPS   |  64 |     1024 | */
/* +------+------+------+------+---------+-----------+-----+----------+ */
/* | ON   | ON   | OFF  |    4 |       2 | 6.8MBPS   |  64 |      128 | */
/* +------+------+------+------+---------+-----------+-----+----------+ */
/* | OFF  | OFF  | ON   |    5 |       5 | 110KBPS   |  16 |     1024 | */
/* +------+------+------+------+---------+-----------+-----+----------+ */
/* | ON   | OFF  | ON   |    6 |       5 | 6.8MBPS   |  16 |      128 | */
/* +------+------+------+------+---------+-----------+-----+----------+ */
/* | OFF  | ON   | ON   |    7 |       5 | 110KBPS   |  64 |     1024 | */
/* +------+------+------+------+---------+-----------+-----+----------+ */
/* | ON   | ON   | ON   |    8 |       5 | 6.8MBPS   |  64 |      128 | */
/* +------+------+------+------+---------+-----------+-----+----------+ */

//s1-5
#define SWS_SHF_MODE		0
//S1-6
#define SWS_64M_MODE		1
//S1-7
#define SWS_CH5_MODE		0

#ifdef __cplusplus
}
#endif /* End of CPP guard */

#endif /* __MACRO_H__ */
