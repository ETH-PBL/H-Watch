/*
 * NB_BC95_task.c
 *
 *  Created on: 28 gen 2020
 *      Author: flavi
 */
/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "stdio.h"
#include "string.h"
#include "time_op_F.h"
#include "BC95_Def.h"
/*END Includes ---------------------------------------------------------------*/
/*Defines --------------------------------------------------------------------*/
#pragma GCC diagnostic ignored "-Wpointer-sign"
#pragma GCC diagnostic ignored "-Wdiscarded-qualifiers"
#define tx_ul_timeout 300
#define FIRST_TIME 0  //set this define to 1 ONLY if its the first time that you run the code on a virgin module, if it's not the first run the code blocks
#define NOSIM     1
/*END Defines ----------------------------------------------------------------*/
/*Variables ------------------------------------------------------------------*/
TIM_HandleTypeDef htim3;
HAL_StatusTypeDef s;
UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_rx;
uint8_t button = 0;
uint8_t tx[2750], baud_set=0, rx[2], rx1[buff_size], rx2[buff_size], check=1, err=1, check_pkt=1, wr_data=0, socket, counter=0, errors[20], sequence=1;
uint32_t RX_data[2],tick=0;
uint8_t volatile ul_conf=1, RX_pkt_data[100], RX_pkt_len=0;
uint16_t len , i=0, ri_cnt;
int rssi,ber, check_scan=12;
UART_AT var;
Cell_Info cell;
States state;
char *dummy;
uint8_t TX_pkt_len[10], TX_pkt_data[2600], time_str[50];
DL_States dl;
extern uint8_t pkt_sent, rd_wr;
extern uint64_t acq_tstamp;
RTC_TimeTypeDef tim;
RTC_DateTypeDef date;
/*END Variables --------------------------------------------------------------*/

/*Macro ----------------------------------------------------------------------*/
static StaticEventGroup_t NB_IoT_EventGroupBuffer;
EventGroupHandle_t NB_IoT_Event;
extern EventGroupHandle_t ADC_Event, SD_Event;
/*END Macro ------------------------------------------------------------------*/

/*Functions-------------------------------------------------------------------*/
//void BC95_time_to_RTC(uint8_t* time_string);
/*END Functions---------------------------------------------------------------*/

void BC95_op(void const * argument)
{
	/* USER CODE BEGIN BC95_NB_op */
	Mngm_DeepSleep_en(0);
	NB_IoT_Event = xEventGroupCreateStatic( &NB_IoT_EventGroupBuffer );
	EventBits_t uxBits;

	uint16_t c_ok=0, c_err=0;

	/* Infinite loop */
	for(;;)
	{
		/* USER CODE BEGIN NB_BC95_op */
		memset(rx1, 0, sizeof(rx1));
		memset(rx2, 0, sizeof(rx2));
		memset(rx,  0,sizeof(rx));
		//Uart structure Init.
		var.str_end = 0;
		var.str_len = 0;
#if FIRST_TIME
		HAL_GPIO_WritePin(BC95_Reset_GPIO_Port, BC95_Reset_Pin, GPIO_PIN_RESET);    //set reset pin to 0
		osDelay(110);
		HAL_UART_Receive_DMA(uart,rx,2);
		HAL_GPIO_WritePin(BC95_Reset_GPIO_Port, BC95_Reset_Pin, GPIO_PIN_SET);  //set reset pin to 1
		osDelay(200);
		HAL_UART_Transmit(uart,(char*)ate0,strlen(ate0),100);
		state = ATE0;
#else
		state = RST1;
#endif
		dl = WaitDL;
		var.rx_p = 0;
		var.str_point = rx2;
#define IS_EVT(x) (uxBits & x)

		while (1)
		{
			uxBits = xEventGroupWaitBits(
					/* The event group being tested. */
					NB_IoT_Event,
					/* The bits within the event group to wait for. */
					EVT_UART_RX | EVT_TX_UL | EVT_T_PSM | EVT_BUTTON,
					/* BIT_0 & BIT_4 should be cleared before returning. */
					pdTRUE,
					/* Don't wait for both bits, either bit will do. */
					pdFALSE,
					/* Wait a maximum of 100ms for either bit to be set. */
					150 );

			switch(state)
			{
			case ATE0:
			{
				if(IS_EVT(EVT_UART_RX)){
					if((memcmp(var.str_point,"OK\r\n",var.str_len)) == 0){
						HAL_UART_Transmit(uart,(char*)cnfg,strlen(cnfg),300);
						state = CONFIG;
					}
					else
					{
						state = ATE0;
					}
				}
				else
				{
					state = ATE0;
				}

				break;


			}

			case CONFIG:
			{
				if(IS_EVT(EVT_UART_RX)){
					if((memcmp(var.str_point,ok,var.str_len)) == 0){
						HAL_UART_Transmit(uart,(char*)cfun1,strlen(cfun1),100);
						state = INIT;
					}
					else
					{
						state = CONFIG;
					}
				}
				else
				{
					state = CONFIG;
				}

				break;

			}

			case RST1:
			{
				HAL_GPIO_WritePin(BC95_Reset_GPIO_Port, BC95_Reset_Pin, GPIO_PIN_RESET);    //set reset pin to 0
				osDelay(110);
				HAL_GPIO_WritePin(BC95_Reset_GPIO_Port, BC95_Reset_Pin, GPIO_PIN_SET);  //set reset pin to 1

				HAL_UART_Receive_DMA(uart,rx,2);

				state = RST2;

				break;

			}

			  case RST2:
			  {
				  if(IS_EVT(EVT_UART_RX)){
					  if((memcmp(var.str_point,"OK\r\n",var.str_len)) == 0){
#if NOSIM
						  HAL_UART_Transmit(uart,(char*)cfun0,strlen(cfun0),100);
						  state = NO_SIM;
#else
						  HAL_UART_Transmit(uart,(char*)cfun0,strlen(cfun0),100);
						  state = INIT;
#endif
					  }
					  else
					  {
						  state = RST2;
					  }
				  }
				  else
				  {
					state = RST2;
				  }

				  break;

			  }

			  case NO_SIM:
			  {
				  if(IS_EVT(EVT_UART_RX)){
					  if((memcmp(var.str_point,ok,var.str_len)) == 0){
						  s = HAL_UART_Transmit(uart, (char*)baud96, strlen(baud96), 300);
						  state = PSM_ENTER;
					  }
					  else
					  {
						  state = NO_SIM;
					  }
				  }
				  else
				  {
					state = NO_SIM;
				  }

				  break;

			  }

			case INIT:
			{
				if(IS_EVT(EVT_UART_RX)){
					if((memcmp(var.str_point,ok,var.str_len)) == 0){
						HAL_UART_Transmit(uart,(char*)cereg,strlen(cereg),100);
						state = RF_ON;
					}
					else
					{
						if((memcmp(var.str_point,nok,var.str_len))==0){
							state = WAIT;
						}
						else{
							state = INIT;
						}
					}
				}
				else
				{
					state = INIT;
				}

				break;
			}

			case WAIT:
			{
				osDelay(3000);
				HAL_UART_AbortReceive(uart); /*Disable UART and reset the module*/
				state = RST1;

				break;
			}

			case RF_ON:
			{
				if(IS_EVT(EVT_UART_RX)){
					check = memcmp(var.str_point,ok,var.str_len);
					err = memcmp(var.str_point,nok,var.str_len);
					if(check == 0){
						HAL_UART_Transmit(uart,(char*)npsmr,strlen(npsmr),100);

						state = NPSMR1;
					}
					else
					{
						if(err==0){
							state = WAIT;
						}
						else{
							state = RF_ON;
						}
					}
				}
				else
				{
					state = RF_ON;
				}

				break;
			}

			case NPSMR1:
			{
				if(IS_EVT(EVT_UART_RX)){
					check = memcmp(var.str_point,ok,var.str_len);
					err = memcmp(var.str_point,nok,var.str_len);
					if(check == 0){
						HAL_UART_Transmit(uart,(char*)cmee,strlen(cmee),100);
						state = CMEE;
					}
					else
					{
						if(err==0){
							state = WAIT;
						}
						else{
							state = NPSMR1;
						}
					}
				}
				else
				{
					state = NPSMR1;
				}

				break;
			}

			case CMEE:
			{
				if(IS_EVT(EVT_UART_RX)){
					check = memcmp(var.str_point,ok,var.str_len);
					err = memcmp(var.str_point,nok,var.str_len);
					if(check == 0){
						HAL_UART_Transmit(uart,(char*)nsonmi,strlen(nsonmi),100);
						state = NSONMI3;
					}
					else
					{
						if(err==0){
							state = WAIT;
						}
						else{
							state = CMEE;
						}
					}
				}
				else
				{
					state = CMEE;
				}

				break;
			}

			case NSONMI3:
			{
				if(IS_EVT(EVT_UART_RX)){
					check = memcmp(var.str_point,ok,var.str_len);
					err = memcmp(var.str_point,nok,var.str_len);
					if(check == 0){
						//						  HAL_UART_Transmit(uart,(char*)apn,strlen(apn),100);        //you can remove comment from this command and comment the command below if you dont' want to set psm settings
						//						  state = APN;

						HAL_UART_Transmit(uart,(char*)cpsms1,strlen(cpsms1),100);  //with "cpsms1" it sets t3312 to 0. with "cpsms0" it sets PSM OFF
						state = PSM_OFF;

					}
					else
					{
						if(err==0){
							state = WAIT;
						}
						else{
							state = NSONMI3;
						}
					}
				}
				else
				{
					state = NSONMI3;
				}

				break;
			}

			case PSM_OFF:
			{
				if(IS_EVT(EVT_UART_RX)){
					check = memcmp(var.str_point,ok,var.str_len);
					err = memcmp(var.str_point,nok,var.str_len);
					if(check == 0){
						HAL_UART_Transmit(uart,(char*)apn,strlen(apn),100);
						state = APN;
					}
					else
					{
						if(err==0){
							state = WAIT;
						}
						else{
							state = PSM_OFF;
						}
					}
				}
				else
				{
					state = PSM_OFF;
				}

				break;
			}

			case APN:
			{
				if(IS_EVT(EVT_UART_RX)){
					check = memcmp(var.str_point,ok,var.str_len);
					err = memcmp(var.str_point,nok,var.str_len);
					if(check == 0){
						HAL_UART_Transmit(uart,(char*)cgatt,strlen(cgatt),100);
						state = CONNECT;
					}
					else
					{
						if(err==0){
							state = WAIT;
						}
						else{
							state = APN;
						}
					}
				}
				else
				{
					state = APN;
				}

				break;
			}

			case CONNECT:
			{
				if(IS_EVT(EVT_UART_RX)){
					if(memcmp(var.str_point,ok,var.str_len) == 0){ //Check if I receive ok after at+cgatt=1
						state = CONNECT;
					}
					else
					{
						if(memcmp(var.str_point,nok1,13) == 0){
							state = WAIT;
						}
						else{
							if(memcmp(var.str_point,connect_ok,10) == 0){ //check if I receive +CEREG:1,....
								state = CONNECT;
							}
							else{
								if(memcmp(var.str_point,psmon,var.str_len) == 0){
									//									  HAL_UART_Transmit(uart,(char*)cclk,strlen(cclk),300);
									//									  state = TIME;
									s = HAL_UART_Transmit(uart,(char*)cell_info,strlen(cell_info),300);
									state = GET_RSSI;
								}
								else{
									state = CONNECT;
								}
							}
						}
					}
				}
				else
				{
					state = CONNECT;
				}

				break;
			}

			case UDP_PORT:
			{
				if(IS_EVT(EVT_UART_RX)){
					if(memcmp(var.str_point,ok,var.str_len) == 0){
						if(var.str_point == rx1){
							check_scan = sscanf(rx2,"\r\n%u\r\n",&socket);
						}
						else{
							check_scan = sscanf(rx1,"\r\n%u\r\n",&socket);
						}
						s = HAL_UART_Transmit(uart, (char*)baud96, strlen(baud96), 300);
						state = PSM_ENTER;
					}
					else
					{
						if(memcmp(var.str_point,nok,var.str_len) == 0){
							state = WAIT;
						}
					}
				}
				else
				{
					state = UDP_PORT;
				}

				break;
			}

			case GET_RSSI:
			{
				if(IS_EVT(EVT_UART_RX)){
					if(memcmp(var.str_point,"\r\nNUESTATS:CELL,",16) == 0){
						//here you get all cell informations, they are in struct "cell"
						sscanf(var.str_point,"\r\nNUESTATS:CELL,%d,%d,%d,%d,%d,%d,%d\r\n",&cell.earfcn,&cell.cellid,&cell.primarycell,&cell.rsrp,&cell.rsrq,&cell.rssi,&cell.snr);
						state = GET_RSSI;
					}
					else{
						if(memcmp(var.str_point,ok,var.str_len) == 0){
							sprintf(tx,"%s%s\n",udpcr,udpport);  //I create the command to create the udp port
							s = HAL_UART_Transmit(uart,(char*)tx,strlen(tx),300);
							state = UDP_PORT;
						}
						else{
							if(memcmp(var.str_point,nok1,var.str_len) == 0){
								state = WAIT;
							}
						}
					}
				}
				else{
					state = GET_RSSI;
				}

				break;
			}

			case PSM_ENTER:
			{
				//to enter in PSM you must set the BC95 baud rate to 9600
				if(IS_EVT(EVT_UART_RX)){
					if(baud_set == 0){
						if(memcmp(var.str_point,ok,var.str_len) == 0){
							USART.Init.BaudRate = 9600;
							HAL_UART_Init(uart);
							osDelay(100);
							baud_set = 1;
							s = HAL_UART_Transmit(uart, (char*)cereg, strlen(cereg), tx_ul_timeout);
							state = PSM_ENTER;
						}
						else{
							state = PSM_ENTER;
						}
					}
					else{
						if(memcmp(var.str_point,ok,var.str_len) == 0){
							baud_set = 0;
							state = PSM;
						}
						else{
							state = PSM_ENTER;
						}
					}
				}
				else{
					state = PSM_ENTER;
				}
				break;
			}

			case PSM:
			{
				if(IS_EVT(EVT_BUTTON)){
					//In My Board if I press a button it goes here

					/*Sample TX CODE*/
					sprintf(TX_pkt_data,"%04x",65535); //payload in this case is "ffff"
					sprintf(TX_pkt_len,"%d",(strlen(TX_pkt_data)>>1)); //payload lenght in bytes
					counter++;
					GLOB_SET_EVT(NB_IoT_Event, EVT_TX_UL);
					/*Sample TX CODE*/
				}
				else{
					if(IS_EVT(EVT_TX_UL)){
						if(USART.Init.BaudRate == 9600){
							//before sending a message we restore the BC95 Baud rate to 115200
							s = HAL_UART_Transmit(uart, (char*)baud1152, strlen(baud1152), tx_ul_timeout);
							state = HIGH_BAUD;
						}
						else{
							//TX_pkt_data is the payload, TX_pkt_len are the bytes in the payload
							sprintf(tx,"AT+NSOST=1,%s,%s,%s,%s,1\n\0",ip,udpport,TX_pkt_len,TX_pkt_data);
							s = HAL_UART_Transmit(uart,(char*)tx,strlen(tx),300);
							state = TX1;
						}
					}
				}
				break;
			}

			case HIGH_BAUD:
			{
				if(IS_EVT(EVT_UART_RX)){
					if(baud_set == 0){
						if(memcmp(var.str_point,ok,var.str_len) == 0){
							USART.Init.BaudRate = 115200;
							HAL_UART_Init(uart);
							osDelay(100);
							baud_set = 1;
							s = HAL_UART_Transmit(uart, (char*)cereg, strlen(cereg), tx_ul_timeout);
							state = HIGH_BAUD;
						}
						else{
							state = HIGH_BAUD;
						}
					}
					else{
						if(memcmp(var.str_point,ok,var.str_len) == 0){
							baud_set = 0;
							sprintf(tx,"AT+NSOST=1,%s,%s,%s,%s,1\n\0",ip,udpport,TX_pkt_len,TX_pkt_data);
							s = HAL_UART_Transmit(uart,(char*)tx,strlen(tx),300);
							state = TX1;
						}
						else{
							state = HIGH_BAUD;
						}
					}
				}
				else{
					state = HIGH_BAUD;
				}
				break;
			}

			case TX1:
			{
				if(IS_EVT(EVT_UART_RX)){
					if(memcmp(var.str_point,ok,strlen(ok)) == 0){
						c_ok++;
						state = TX2;
					}
					else{
						if(memcmp(var.str_point,nok1,13) == 0){
							c_err++;
							osDelay(200);
							state = PSM;
							GLOB_SET_EVT(NB_IoT_Event, EVT_TX_UL); // if you get error it resends the same packet
						}
						else{
							state = TX1;
						}
					}
				}
				else
				{
					state = TX1;
				}

				break;
			}

			case TX2:
			{
				if(IS_EVT(EVT_UART_RX)){

					if(memcmp(var.str_point,pkt_conf,10) == 0){
						dl = PKT_Sent;
					}
					else{
						if(memcmp(var.str_point,pkt_rx,12) == 0){
							dl = DL_Received;
						}
						else{
							if(memcmp(var.str_point,psmon,strlen(psmon)) == 0){
								dl = Enter_PSM;
							}
							else{
								dl = WaitDL;
							}
						}
					}

					switch(dl)
					{
					case PKT_Sent:
					{
						sscanf(var.str_point,"\r\n+NSOSTR:%u,%u,%u\r\n",&RX_pkt_len,&RX_pkt_len,&ul_conf);
						if(ul_conf == 1){
							//PKT sent confirmed
						}
						else{
							state = PSM;
							GLOB_SET_EVT(NB_IoT_Event, EVT_TX_UL);
						}
						break;
					}

					case DL_Received:
					{
						sscanf(var.str_point,"\r\n+NSONMI:1,%d,%s\r\n",&RX_pkt_len,RX_pkt_data);
						dummy = strstr(RX_pkt_data, "2222"); //invece di usare lo strstr si potrebbe usare la lunghezza del pacchetto ed il memcmp
						if(dummy != NULL){
							//Here you can manage the downlink
						}
						else{

							state = TX2;
						}
						break;
					}

					case Enter_PSM:
					{
						s = HAL_UART_Transmit(uart, (char*)baud96, strlen(baud96), 300);
						state = PSM_ENTER;
						break;
					}

					case WaitDL:
					{
						state = TX2;
						break;
					}

					}

				}
				else{
					state = TX2;
				}

				break;
			}

			case TIME:
			{
				if(IS_EVT(EVT_UART_RX)){
					if(memcmp(var.str_point,ok,var.str_len) == 0){
						if(var.str_point == rx1){
							sprintf(time_str,"%s",rx2);
						}
						else{
							sprintf(time_str,"%s",rx1);
						}
						BC95_time_to_RTC(time_str);
						s = HAL_UART_Transmit(uart,(char*)cell_info,strlen(cell_info),300);
						state = GET_RSSI;
					}
					else{
						state = TIME;
					}
				}
				else{
					state = TIME;
				}
			}

			}
		}

	}

}
#pragma GCC push_options
#pragma GCC optimize ("03")

void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart)
{
	if(var.str_point == rx1){
		rx2[var.rx_p]=rx[0];
		if(var.rx_p > 1){
			if( (rx2[var.rx_p-1] == '\r') && (rx2[var.rx_p] == '\n') ){
				var.str_end = 1;
				var.str_len = var.rx_p;
				var.str_point = rx2;
				rx2[var.rx_p+1] = 0;
				var.rx_p = 0;
				GLOB_SET_EVT( NB_IoT_Event, EVT_UART_RX );
			}
			else{
				var.rx_p++;
			}
		}
		else{
			var.rx_p++;
		}
	}
	else {
		rx1[var.rx_p]=rx[0];
		if(var.rx_p > 1){
			if( (rx1[var.rx_p-1] == '\r') && (rx1[var.rx_p] == '\n') ){
				var.str_end = 1;
				var.str_len = var.rx_p;
				var.str_point = rx1;
				rx1[var.rx_p+1] = 0;
				var.rx_p = 0;
				GLOB_SET_EVT( NB_IoT_Event, EVT_UART_RX );
			}
			else{
				var.rx_p++;
			}
		}
		else{
			var.rx_p++;
		}
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(var.str_point == rx1){
		rx2[var.rx_p]=rx[1];
		if(var.rx_p > 1){
			if( (rx2[var.rx_p-1] == '\r') && (rx2[var.rx_p] == '\n') ){
				var.str_end = 1;
				var.str_len = var.rx_p;
				var.str_point = rx2;
				rx2[var.rx_p+1] = 0;
				var.rx_p = 0;
				GLOB_SET_EVT( NB_IoT_Event, EVT_UART_RX );
			}
			else{
				var.rx_p++;
			}
		}
		else{
			var.rx_p++;
		}
	}
	else {
		rx1[var.rx_p]=rx[1];
		if(var.rx_p > 1){
			if( (rx1[var.rx_p-1] == '\r') && (rx1[var.rx_p] == '\n') ){
				var.str_end = 1;
				var.str_len = var.rx_p;
				var.str_point = rx1;
				rx1[var.rx_p+1] = 0;
				var.rx_p = 0;
				GLOB_SET_EVT( NB_IoT_Event, EVT_UART_RX );
			}
			else{
				var.rx_p++;
			}
		}
		else{
			var.rx_p++;
		}
	}
}
#pragma GCC pop_options
