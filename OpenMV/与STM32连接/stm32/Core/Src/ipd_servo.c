/*
 * ipd_servo.c
 *
 *  Created on: 14.07.2018
 *      Author: johae
 */

#include "ipd_servo.h"
#include "ipd_visu.h"
#include "utils.h"

uint32_t driveX, driveY;
uint8_t driveAngleTilt, driveAnglePan;

int32_t improved_map(int32_t value, int32_t minIn, int32_t maxIn, int32_t minOut, int32_t maxOut)
{
    const int32_t rangeIn = maxIn - minIn;
    const int32_t rangeOut = maxOut - minOut;
    const int32_t deltaIn = value - minIn;
    // fixed point math constants to improve accurancy of divide and rounding
    const int32_t fixedHalfDecimal = 1;
    const int32_t fixedDecimal = fixedHalfDecimal * 2;

    return ((deltaIn * rangeOut * fixedDecimal) / (rangeIn) + fixedHalfDecimal) / fixedDecimal + minOut;
}


void servo_init(uint8_t servo)
{
	// TODO: Do something?!
}

void servo_deinit(uint8_t servo)
{
	if (servo == SERVO_PAN)
	{
		HAL_TIM_PWM_Stop(SERVO_PAN_CH, TIM_CHANNEL_1);
	}
	else if (servo == SERVO_TILT)
	{
		HAL_TIM_PWM_Stop(SERVO_TILT_CH, TIM_CHANNEL_1);
	}
	else
	{
		_Error_Handler(__FILE__, __LINE__);
	}

}

void servo_writeMS (uint8_t servo, uint8_t value)
{
	if (value < SERVO_PAN_0DEG_DC || value > SERVO_PAN_180DEG_DC)
	{
		_Error_Handler(__FILE__, __LINE__);
	}
	/*
	if (servo == SERVO_TILT && value < SERVO_TILT_MAXDEG_DC)
	{
			_Error_Handler(__FILE__, __LINE__);
	}
	*/
	TIM_OC_InitTypeDef sConfigOC;
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = value;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	if (servo == SERVO_PAN)
	{
		if (HAL_TIM_PWM_ConfigChannel(SERVO_PAN_CH, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
		{
			_Error_Handler(__FILE__, __LINE__);
		}
		HAL_TIM_PWM_Start(SERVO_PAN_CH, TIM_CHANNEL_1);
	}
	else if (servo == SERVO_TILT)
	{
		if (HAL_TIM_PWM_ConfigChannel(SERVO_TILT_CH, &sConfigOC, TIM_CHANNEL_1)!= HAL_OK)
		{
			_Error_Handler(__FILE__, __LINE__);
		}
		HAL_TIM_PWM_Start(SERVO_TILT_CH, TIM_CHANNEL_1);
	}
	else
	{
		_Error_Handler(__FILE__, __LINE__);
	}
}

void servo_writeDEG (uint8_t servo, uint8_t deg)
{
	// limit mechanical (only tilt)
	if (servo == SERVO_TILT && deg >= SERVO_TILT_MAX_DEG)
	{
		deg = SERVO_MAX_DEG;
	}
	// limit for functional reasons
	if (deg >= SERVO_MAX_DEG)
	{
		deg = SERVO_MAX_DEG;
	}
	if (deg <= SERVO_MIN_DEG)
	{
		deg = SERVO_MIN_DEG;
	}
	uint8_t dutycycle = improved_map(deg,0,180,SERVO_PAN_0DEG_DC,SERVO_PAN_180DEG_DC);
	servo_writeMS(servo, dutycycle);
}

uint8_t servo_posFilter(const uint8_t n, const uint8_t in, const uint8_t disabled)
{
	static uint8_t buffer[101] = {0}, pos = 0;
	uint32_t sum = 0;

	if (disabled != 0)
	{
		return in;
	}

	if ((n >= 100) || (n < 1))
	{
		return 0;
	}

	buffer[pos] = in;
	pos++;

	if (pos > n)
	{
		pos = 0;
	}

	for (uint8_t idx = 0; idx <n; idx++)
	{
		sum += buffer[idx];
	}

	return (sum/pos);
}

/* StartServoTask function */
void StartServoTask(void const * argument)
{
  /* USER CODE BEGIN StartServoTask */

  /* Infinite loop */
  for(;;)
  {
	  switch(currServoState){
	  case SERVOSTATE_DISABLED:
		  servo_deinit(SERVO_TILT);
		  servo_deinit(SERVO_TILT);
		  break;

	  case SERVOSTATE_INIT:
		  driveAngleTilt = 90;
		  driveAnglePan = 90;
		  servo_writeDEG(SERVO_TILT,driveAngleTilt);
		  servo_writeDEG(SERVO_PAN,driveAnglePan);
		  currServoState = SERVOSTATE_READY;
		  break;

	  case SERVOSTATE_READY:
		  // here comes the tracking ...
		  // X --> PAN *******************************************
		  if (x_pos < (IMG_CENTER_X - IMG_CENTER_ERROR))
		  {
			  driveAnglePan += SERVO_ANGLE_SPEED;
			  if (driveAnglePan > SERVO_MAX_DEG)
			  {
				  driveAnglePan = SERVO_MAX_DEG;
			  }
			  servo_writeDEG(SERVO_PAN,driveAnglePan);
		  }
		  if (x_pos > (IMG_CENTER_X + IMG_CENTER_ERROR))
		  {
			  driveAnglePan -= SERVO_ANGLE_SPEED;
			  if (driveAnglePan < SERVO_MIN_DEG)
			  {
				  driveAnglePan = SERVO_MIN_DEG;
			  }
			  servo_writeDEG(SERVO_PAN,driveAnglePan);
		  }
		  // Y --> TILT *******************************************
		  if (y_pos > (IMG_CENTER_Y + IMG_CENTER_ERROR))
		  {
			  driveAngleTilt+= SERVO_ANGLE_SPEED;
			  if (driveAngleTilt > SERVO_TILT_MAX_DEG)
			  {
				  driveAngleTilt = SERVO_TILT_MAX_DEG;
			  }
			  servo_writeDEG(SERVO_TILT,driveAngleTilt);
		  }
		  if (y_pos < (IMG_CENTER_Y - IMG_CENTER_ERROR))
		  {
			  driveAngleTilt -= SERVO_ANGLE_SPEED;
			  if (driveAngleTilt < SERVO_MIN_DEG)
			  {
				  driveAngleTilt = SERVO_MIN_DEG;
			  }
			  servo_writeDEG(SERVO_TILT,driveAngleTilt);
		  }
		  break;

	  default:
		  Error_Handler();
		  break;

	  }
    osDelay(300);
  }
  /* USER CODE END StartServoTask */
}

