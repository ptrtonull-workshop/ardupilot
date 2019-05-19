/*
 * ipd_visu.c
 *
 *  Created on: 14.07.2018
 *      Author: johae
 */

#include "ipd_visu.h"
#include "ipd_servo.h"
#include "ipd_comm.h"
#include "fifofast.h"

WM_HWIN hWin;
WM_HWIN CreateMainWindow(void);
TS_StateTypeDef  TS_State;
state_t currState;
uint8_t currServoState;

uint8_t str[MAX_STR_SIZE];
uint8_t x_pos = 100, y_pos = 100, yPosHold = 888, xPosHold = 888;
uint8_t xVisuPos = 0, yVisuPos = 0;

/* Helper for display text from UART */
void dispText(const uint8_t * s)
{
	WM_HWIN hItem = WM_GetDialogItem(hWin, (GUI_ID_USER + 0x07));
	TEXT_SetText(hItem, (const char *) s);
}

/* StartStatemaschineTask function */
void StartStatemaschineTask(void const * argument)
{
  /* USER CODE BEGIN StartStatemaschineTask */
	currState = STATE_INITIAL;
	currServoState = SERVOSTATE_DISABLED;
  /* Infinite loop */
  for(;;)
  {
	  // message from queue
	  msg_t currMsg = MSG_NONE;
	  osEvent msg = osMessageGet(funcCmdQueueHandle,0);
	  if (msg.status == osEventMessage)
	  {
		  currMsg = msg.value.v;
	  }

	  // disable servo tracking
	  if ((currMsg == MSG_SERVO) && (currServoState != SERVOSTATE_DISABLED))
	  {
		  currServoState = SERVOSTATE_DISABLED;
	  }
	  // disable servo tracking
	  if ((currMsg == MSG_SERVO) && (currServoState == SERVOSTATE_DISABLED))
	  {
		  currServoState = SERVOSTATE_INIT;
	  }
	  // state machine
	  switch(currState){
	  case STATE_INITIAL:
			  currState = STATE_WAITING;
		  break;
	  case STATE_WAITING:
		  strcpy(&str,(const char *) "> Waiting ...");
		  if (currMsg == MSG_FACE)
		  {
			  currState = STATE_FACE;
		  }
		  if (currMsg == MSG_COLOR)
		  {
			  currState = STATE_COLOR;
		  }
		  if (currMsg == MSG_CODE)
		  {
			  currState = STATE_CODE;
		  }
		  break;
		case STATE_COLOR:
			if (currMsg == MSG_START)
			{
				osMessagePut(txQueueHandle, (uint32_t) 0xFF, osWaitForever);
				osMessagePut(txQueueHandle, (uint32_t) 0xFF, osWaitForever);
				osMessagePut(txQueueHandle, (uint32_t) 0xAA, osWaitForever);
				osMessagePut(txQueueHandle, (uint32_t) 0xAA, osWaitForever);

			}
			if (currMsg == MSG_STOP)
			{
				osMessagePut(txQueueHandle, (uint32_t) 0xAB, osWaitForever);
				osMessagePut(txQueueHandle, (uint32_t) 0xAB, osWaitForever);
				currState = STATE_WAITING;
			}
			break;
		case STATE_FACE:
		  if (currMsg == MSG_START)
		  {
			  osMessagePut(txQueueHandle, (uint32_t) 0xFE, osWaitForever);
			  osMessagePut(txQueueHandle, (uint32_t) 0xFE, osWaitForever);
			  osMessagePut(txQueueHandle, (uint32_t) 0xAA, osWaitForever);
			  osMessagePut(txQueueHandle, (uint32_t) 0xAA, osWaitForever);

		  }
		  if (currMsg == MSG_STOP)
		  {
			  osMessagePut(txQueueHandle, (uint32_t) 0xAB, osWaitForever);
			  osMessagePut(txQueueHandle, (uint32_t) 0xAB, osWaitForever);
			  currState = STATE_WAITING;
		  }
		  break;
	  case STATE_CODE:
		  if (currMsg == MSG_START)
		  {
			  osMessagePut(txQueueHandle, (uint32_t) 0xFD, osWaitForever);
			  osMessagePut(txQueueHandle, (uint32_t) 0xFD, osWaitForever);
			  osMessagePut(txQueueHandle, (uint32_t) 0xAA, osWaitForever);
			  osMessagePut(txQueueHandle, (uint32_t) 0xAA, osWaitForever);
		  }
		  if (currMsg == MSG_STOP)
		  {
			  osMessagePut(txQueueHandle, (uint32_t) 0xAB, osWaitForever);
			  osMessagePut(txQueueHandle, (uint32_t) 0xAB, osWaitForever);
			  currState = STATE_WAITING;
		  }
		  break;
	  default:
		  Error_Handler();
		  break;

	  }
    osDelay(1);
  }
  /* USER CODE END StartStatemaschineTask */
}



void StartSTemWinTask(void const * argument)
{
  /* USER CODE BEGIN StartSTemWinTask */
	//buffer
	uint8_t tiltBuf[20], panBuf[20], xBuf[20], yBuf[20];
	// init
	BSP_LCD_Init();
	HAL_Delay(200);
	GUI_Init();
	GUI_SetBkColor(GUI_WHITE);
	GUI_Clear();

	hWin = CreateMainWindow();

	// get item IDs
	WM_HWIN hItemInfo = WM_GetDialogItem(hWin, (GUI_ID_USER + 0x07));
	WM_HWIN hItemXVal = WM_GetDialogItem(hWin, (GUI_ID_USER + 0x06));
	WM_HWIN hItemYVal = WM_GetDialogItem(hWin, (GUI_ID_USER + 0x05));
	WM_HWIN hItemPan = WM_GetDialogItem(hWin,(GUI_ID_USER + 0x09));
	WM_HWIN hItemTilt = WM_GetDialogItem(hWin,(GUI_ID_USER + 0x0A));

  /* Infinite loop */
	for (;;)
	{
		// display string
		TEXT_SetText(hItemInfo, (const char *)&str);

		// calc visu point
//		xVisuPos = x_pos + 25;
//		yVisuPos = y_pos + 5;

		// write PAN/TILT to LCD
		sprintf((char *) tiltBuf,"TILT: %u DEG", (const uint8_t) driveAngleTilt);
		sprintf((char *) panBuf,"PAN: %u DEG", (const uint8_t) driveAnglePan);
		TEXT_SetText(hItemPan,(const char *) &panBuf);
		TEXT_SetText(hItemTilt,(const char *) &tiltBuf);

		// write xpos/ypos
		sprintf((char *) xBuf,"X:%03u", x_pos);
		sprintf((char *) yBuf,"Y:%03u", y_pos);
		TEXT_SetText(hItemXVal,(const char *) &xBuf);
		TEXT_SetText(hItemYVal,(const char *) &yBuf);

		// force update graph
		WM_HWIN hItemGraph = WM_GetDialogItem(hWin, (GUI_ID_USER + 0x08));
		WM_Invalidate(hItemGraph);


		// Update GUI
		WM_Update(hWin);
		GUI_Delay(500);

		osDelay(1);
	}
  /* USER CODE END StartSTemWinTask */
}


/* StartTouchTask function */
void StartTouchTask(void const * argument)
{
	GUI_PID_STATE STemWin_TS;
	/* USER CODE BEGIN StartTouchTask */
	if (BSP_TS_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize()) != TS_OK) {
		Error_Handler();
	}
	/* Infinite loop */
	for (;;)
	{
		BSP_TS_GetState(&TS_State);
		if (TS_State.touchEventId[0] == TOUCH_EVENT_PRESS_DOWN)
		{
			GUI_SetFont(&GUI_FontComic24B_ASCII);
			STemWin_TS.Pressed = 1;
			STemWin_TS.Layer = 0;
			STemWin_TS.x = TS_State.touchX[0];
			STemWin_TS.y = TS_State.touchY[0];
			BSP_TS_ResetTouchData(&TS_State);
		}
		else
		{
			STemWin_TS.Pressed = 0;
		}
		GUI_TOUCH_StoreStateEx(&STemWin_TS);
		osDelay(1);
	}
	/* USER CODE END StartTouchTask */
}

/* StartCommTask function */
void StartCommTask(void const * argument)
{
  /* USER CODE BEGIN StartCommTask */
	uint8_t rx_byte = '0';
	uint8_t i = 0;
	uint8_t yPosStr[6] = "X:000", xPosStr[6] = "Y:000";
	str[0] = '\0';

  /* Infinite loop */
	for (;;)
	{
		// new char from UART
		osEvent msg = osMessageGet(rxQueueHandle, osWaitForever);
		if (msg.status == osEventMessage)
		{
			rx_byte = (uint8_t) msg.value.v;
			// write to char array
			str[i] = rx_byte;
			i++;
			// LF or CR
			if (rx_byte == CHAR_CR || rx_byte == CHAR_LF )
			{
				str[i-1] = '\0';
				i = 0;
			}
			// overflow
			if (i > MAX_STR_SIZE - 2)
			{
				str[0] = '\0';
				i = 0;
			}
		}



		// search position data
		for (uint8_t idx = 0; idx < MAX_STR_SIZE; idx++)
		{
			// end of data
			if (str[idx] == '\0')
			{
				break;
			}
			// pos data: X:000
			if (str[idx] == 'X')
			{
				for(uint8_t cpIdx = 0; cpIdx<= 4; cpIdx++)
				{
					xPosStr[cpIdx] = str[idx+cpIdx];
				}
				// numeric position
				xPosHold = ((xPosStr[2] - 0x30) * 100) + ((xPosStr[3] - 0x30) * 10) + (xPosStr[4] - 0x30);
				if (xPosHold < IMG_SIZE_X)
				{
					x_pos = xPosHold;
				}

			}
			// pos data: Y:000
			if (str[idx] == 'Y')
			{
				for (uint8_t cpIdx = 0; cpIdx <= 4; cpIdx++)
				{
					yPosStr[cpIdx] = str[idx + cpIdx];
				}
				// numeric position
				yPosHold = ((yPosStr[2] - 0x30) * 100) + ((yPosStr[3] - 0x30) * 10) + (yPosStr[4] - 0x30);
				if (yPosHold < IMG_SIZE_Y)
				{
					y_pos = yPosHold;
				}

			}
		}
		osDelay(1);
	}
  /* USER CODE END StartCommTask */
}
