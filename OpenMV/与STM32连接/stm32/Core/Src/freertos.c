/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2018 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

/* USER CODE BEGIN Includes */     
#include "ipd_visu.h"

/* USER CODE END Includes */

/* Variables -----------------------------------------------------------------*/
osThreadId defaultTaskHandle;
osThreadId STemWinTaskHandle;
osThreadId touchTaskHandle;
osThreadId CommTaskHandle;
osThreadId TxTaskHandle;
osThreadId RxTaskHandle;
osThreadId StatemaschineTaHandle;
osThreadId ServoTaskHandle;
osMessageQId funcCmdQueueHandle;
osMessageQId txQueueHandle;
osMessageQId rxQueueHandle;
osMessageQId servoCmdQueueHandle;
osMutexId xTxMutexHandle;
osSemaphoreId txSemaHandle;

/* USER CODE BEGIN Variables */


/* USER CODE END Variables */

/* Function prototypes -------------------------------------------------------*/
void StartDefaultTask(void const * argument);
void StartSTemWinTask(void const * argument);
void StartTouchTask(void const * argument);
void StartCommTask(void const * argument);
void StartTxTask(void const * argument);
void StartRxTask(void const * argument);
void StartStatemaschineTask(void const * argument);
void StartServoTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

/* Hook prototypes */

/* Init FreeRTOS */

void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */

  /* Create the mutex(es) */
  /* definition and creation of xTxMutex */
  osMutexDef(xTxMutex);
  xTxMutexHandle = osMutexCreate(osMutex(xTxMutex));

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* definition and creation of txSema */
  osSemaphoreDef(txSema);
  txSemaHandle = osSemaphoreCreate(osSemaphore(txSema), 1);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of STemWinTask */
  osThreadDef(STemWinTask, StartSTemWinTask, osPriorityBelowNormal, 0, 512);
  STemWinTaskHandle = osThreadCreate(osThread(STemWinTask), NULL);

  /* definition and creation of touchTask */
  osThreadDef(touchTask, StartTouchTask, osPriorityNormal, 0, 128);
  touchTaskHandle = osThreadCreate(osThread(touchTask), NULL);

  /* definition and creation of CommTask */
  osThreadDef(CommTask, StartCommTask, osPriorityNormal, 0, 256);
  CommTaskHandle = osThreadCreate(osThread(CommTask), NULL);

  /* definition and creation of TxTask */
  osThreadDef(TxTask, StartTxTask, osPriorityNormal, 0, 128);
  TxTaskHandle = osThreadCreate(osThread(TxTask), NULL);

  /* definition and creation of RxTask */
  osThreadDef(RxTask, StartRxTask, osPriorityNormal, 0, 128);
  RxTaskHandle = osThreadCreate(osThread(RxTask), NULL);

  /* definition and creation of StatemaschineTa */
  osThreadDef(StatemaschineTa, StartStatemaschineTask, osPriorityAboveNormal, 0, 256);
  StatemaschineTaHandle = osThreadCreate(osThread(StatemaschineTa), NULL);

  /* definition and creation of ServoTask */
  osThreadDef(ServoTask, StartServoTask, osPriorityNormal, 0, 128);
  ServoTaskHandle = osThreadCreate(osThread(ServoTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Create the queue(s) */
  /* definition and creation of funcCmdQueue */
/* what about the sizeof here??? cd native code */
  osMessageQDef(funcCmdQueue, 10, uint8_t);
  funcCmdQueueHandle = osMessageCreate(osMessageQ(funcCmdQueue), NULL);

  /* definition and creation of txQueue */
/* what about the sizeof here??? cd native code */
  osMessageQDef(txQueue, 1000, uint8_t);
  txQueueHandle = osMessageCreate(osMessageQ(txQueue), NULL);

  /* definition and creation of rxQueue */
/* what about the sizeof here??? cd native code */
  osMessageQDef(rxQueue, 1000, uint8_t);
  rxQueueHandle = osMessageCreate(osMessageQ(rxQueue), NULL);

  /* definition and creation of servoCmdQueue */
/* what about the sizeof here??? cd native code */
  osMessageQDef(servoCmdQueue, 10, uint8_t);
  servoCmdQueueHandle = osMessageCreate(osMessageQ(servoCmdQueue), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */
}

/* StartDefaultTask function */
void StartDefaultTask(void const * argument)
{

  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* StartSTemWinTask function */
__weak void StartSTemWinTask(void const * argument)
{
  /* USER CODE BEGIN StartSTemWinTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartSTemWinTask */
}

/* StartTouchTask function */
__weak void StartTouchTask(void const * argument)
{
  /* USER CODE BEGIN StartTouchTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartTouchTask */
}

/* StartCommTask function */
__weak void StartCommTask(void const * argument)
{
  /* USER CODE BEGIN StartCommTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartCommTask */
}

/* StartTxTask function */
__weak void StartTxTask(void const * argument)
{
  /* USER CODE BEGIN StartTxTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartTxTask */
}

/* StartRxTask function */
__weak void StartRxTask(void const * argument)
{
  /* USER CODE BEGIN StartRxTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartRxTask */
}

/* StartStatemaschineTask function */
__weak void StartStatemaschineTask(void const * argument)
{
  /* USER CODE BEGIN StartStatemaschineTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartStatemaschineTask */
}

/* StartServoTask function */
__weak void StartServoTask(void const * argument)
{
  /* USER CODE BEGIN StartServoTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartServoTask */
}

/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
