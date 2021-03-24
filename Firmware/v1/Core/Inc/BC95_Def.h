/*
 * AcquiriaNB_Def.h
 *
 *  Created on: Jan 27, 2020
 *      Author: flavi
 */

#ifndef INC_BC95_DEF_H_
#define INC_BC95_DEF_H_

/*Application Defines*/
//#define RTOS_SLEEP_EN   0
//#define DEEP_SLEEP_EN   1

/*Uart Defines*/
#define buff_size       ((uint16_t)0x07d0U)  /*Uart RX Buffer dimension*/
#define uart            &huart1
#define USART			huart1

typedef struct
{
	uint8_t str_end;                  /*String end flag*/
	uint16_t str_len;                 /*String lenght*/
	uint8_t *str_point;               /*String buffer pointer*/
	uint16_t  rx_p;                   /*Real time pointer*/
}UART_AT;

typedef struct
{
	int earfcn;
	int cellid;
	int primarycell;
	int rsrp;
	int rsrq;
	int rssi;
	int snr;
}Cell_Info;


/*AT Commands*/
#define ate0 "ATE0\n\0"
#define cnfg "AT+NCONFIG=\"AUTOCONNECT\",FALSE\n\0"
#define baud96 "AT+NATSPEED=9600,30,0,2\n\0"
#define baud1152 "AT+NATSPEED=115200,30,0,2\n\0"
#define cereg "AT+CEREG=4\n\0"
#define npsmr "AT+NPSMR=1\n\0"
#define cfun1 "AT+CFUN=1\n\0"
#define cfun0 "AT+CFUN=0\n\0"
#define isconn "AT+CEREG?\n\0"
#define nsonmi "AT+NSONMI=3\n\0"
#define cgatt "AT+CGATT=1\n\0"
#define cpsms0 "AT+CPSMS=0\n\0"
#define cpsms1 "AT+CPSMS=1,,,10100111,00000000\n\0"
#define stat "AT+NUESTATS\n\0"
#define apn "AT+CGDCONT=1,\"IP\",\"nbiot.tids.tim.it\"\n\0"
#define sw_reset "AT+NRB\n\0"
#define udpcr "AT+NSOCR=DGRAM,17,"
#define newline "\n"
#define pkt1 "AT+NSOSTF=1,\""
#define pkt2 "\","
#define pkt3 ",0x100,2,AB30\n"
#define pkt_flag ",0x100,"
#define csq "AT+CSQ\n\0"
#define cmee "AT+CMEE=1\n\0"
#define cclk "AT+CCLK?\n\0"
#define cell_info "AT+NUESTATS=CELL\n\0"

/*AT commands notifications and terminations*/
#define ok "\r\nOK\r\n"
#define nok "\r\nERROR\r\n"
#define nok1 "\r\n+CME ERROR:"
#define disconn "\r\n+CEREG:4,0,0000,00000000,9,,,,\r\n"
#define inconnection "\r\n+CEREG:2,0000,00000000,9,,,,\r\n"
#define connect_ok "\r\n+CEREG:1"
#define psmon "\r\n+NPSMR:1\r\n"
#define pkt_rx "\r\n+NSONMI:1,"
#define pkt_conf "\r\n+NSOSTR:"

/*BC95 State Machine states*/
typedef enum{ATE0, CONFIG, INIT, NO_SIM, RF_ON, CMEE, CONNECT, PSM, PSM_ENTER, HIGH_BAUD, TX1, TX2, WAIT, UDP_PORT, RST1, RST2, NPSMR1, APN, NSONMI3, PSM_OFF, GET_RSSI, TIME}
States;

/*DL Cases*/
typedef enum{Enter_PSM, DL_Received, PKT_Sent, WaitDL}
DL_States;

/*UDP Port and IP define*/
#define udpport "51000"
#define ip "137.204.213.206"

/*NB_IoT evt*/
enum {
  EVT_UART_RX  						= (1 << 0),     /* 0b0000000000000001 */
  EVT_T_PSM               			= (1 << 1),     /* 0b0000000000000010 */
  EVT_TX_UL             			= (1 << 2),     /* 0b0000000000000100 */
  EVT_BUTTON             			= (1 << 3),     /* 0b0000000000001000 */
};

/*Task Function*/
void BC95_op(void const * argument);

#endif /* INC_BC95_DEF_H_ */
