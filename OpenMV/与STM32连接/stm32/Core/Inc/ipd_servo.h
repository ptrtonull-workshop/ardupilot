/*
 * ipd_servo.h
 *
 *  Created on: 14.07.2018
 *      Author: johae
 */

#ifndef INC_IPD_SERVO_H_
#define INC_IPD_SERVO_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32746g_discovery.h"
#include "stm32746g_discovery_lcd.h"
#include "stm32746g_discovery_ts.h"
#include "stdint.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "tim.h"
#include "GUI.h" //STEmWin
#include "WM.h" //STEmWin


#define SERVO_PAN			(1)
#define SERVO_PAN_CH		(&htim12)
#define SERVO_TILT			(2)
#define SERVO_TILT_CH		(&htim3)
#define SERVO_ANGLE_SPEED	(1)
#define SERVO_ANGLE_GAIN	(0.03)

// duty cycle
#define SERVO_PAN_0DEG_DC 	(85)
#define SERVO_PAN_180DEG_DC	(250)
#define SERVO_PAN_CENTER_DC	(152)

#define SERVO_TILT_0DEG_DC 		(88)
#define SERVO_TILT_180DEG_DC	(250)
#define SERVO_TILT_CENTER_DC	(150)
#define SERVO_TILT_MAXDEG_DC	(190)

// degree
#define SERVO_MAX_DEG		(130)
#define SERVO_MIN_DEG		(40)
#define SERVO_TILT_MAX_DEG	(100)	// tilt mechanical max

// image positions
#define IMG_SIZE_X			(200)
#define IMG_SIZE_Y			(200)
#define IMG_CENTER_X		((IMG_SIZE_X)/2)
#define IMG_CENTER_Y		((IMG_SIZE_Y)/2)
#define IMG_CENTER_ERROR	(10)
#define IMG_FACT_VISU		(1)

void servo_init(uint8_t servo);
void servo_deinit(uint8_t servo);
void servo_writeMS (uint8_t servo, uint8_t value);
void servo_writeDEG (uint8_t servo, uint8_t value);

extern osMessageQId servoCmdQueueHandle;
extern uint32_t driveX, driveY;
extern uint8_t driveAngleTilt, driveAnglePan;

#endif /* INC_IPD_SERVO_H_ */
