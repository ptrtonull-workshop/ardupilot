/*
 * ipd_comm.c
 *
 *  Created on: 14.07.2018
 *      Author: johae
 */

#include "ipd_comm.h"
#include "fifofast.h"

#define HOST_RX_ISR_FLAG (0x00005)
#define HOST_RX_BUFFER 1

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART6)
    {
         osSemaphoreRelease(txSemaHandle);
    }
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

    if(huart->Instance == USART6)
    {
    	osSignalSet(RxTaskHandle, HOST_RX_ISR_FLAG );
    }
}


void StartRxTask(void const * argument)
{
    // Buffer to hold message
    uint8_t tmp_char = 0;
    uint8_t rx_tmp_buffer = {0};

    // Enable IRQs for UART
    HAL_NVIC_EnableIRQ(USART6_IRQn);
    __HAL_UART_ENABLE_IT(&huart6, UART_IT_RXNE);
    //__HAL_UART_ENABLE_IT(&huart6, UART_IT_TXE);

    // Trigger RX IT
    HAL_UART_Receive_IT( &huart6, &rx_tmp_buffer, 1 );

    for(;;)
    {
		// wait for char RX
		osSignalWait( HOST_RX_ISR_FLAG, osWaitForever);
		// read single byte from UART
		tmp_char = *(huart6.pRxBuffPtr - huart6.RxXferSize);
		// write to Queue
		osMessagePut(rxQueueHandle, (uint32_t) tmp_char, osWaitForever);

		// Be sure to set the interrupt for the next char
		if (HAL_UART_Receive_IT(&huart6, (huart6.pRxBuffPtr - huart6.RxXferSize), 1) != HAL_OK)
		{
			Error_Handler();
		}
	}
}

void StartTxTask(void const * argument)
{
	uint8_t tx_byte = 0;
	osEvent msg;
	for (;;)
	{
		osSemaphoreWait(txSemaHandle, osWaitForever);
		msg = osMessageGet(txQueueHandle, osWaitForever);
		if (msg.status == osEventMessage)
		{
			tx_byte = (uint8_t) msg.value.v;

			if (HAL_UART_Transmit_IT(&huart6, &tx_byte, 1) != HAL_OK)
			{
				Error_Handler();
			}
		}
		osDelay(1);
	}
}


