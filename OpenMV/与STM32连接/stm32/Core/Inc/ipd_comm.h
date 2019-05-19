/*
 * ipd_comm.h
 *
 *  Created on: 15.07.2018
 *      Author: johae
 */

#ifndef INC_IPD_COMM_H_
#define INC_IPD_COMM_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32746g_discovery.h"
#include "stm32746g_discovery_lcd.h"
#include "stm32746g_discovery_ts.h"
#include "FreeRTOS.h"
#include "task.h"
#include "usart.h"
#include "cmsis_os.h"
#include "ipd_visu.h"
#include "GUI.h" //STEmWin
#include "WM.h" //STEmWin
#include "DIALOG.h"

extern osMessageQId funcCmdQueueHandle;
extern osMessageQId txQueueHandle;
extern osMessageQId rxQueueHandle;
extern osMutexId xTxMutexHandle;
extern osSemaphoreId txSemaHandle;
extern osThreadId CommTaskHandle;
extern osThreadId TxTaskHandle;
extern osThreadId RxTaskHandle;

#endif /* INC_IPD_COMM_H_ */
