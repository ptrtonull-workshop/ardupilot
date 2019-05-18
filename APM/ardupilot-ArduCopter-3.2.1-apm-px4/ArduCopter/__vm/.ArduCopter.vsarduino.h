/* 
	Editor: https://www.visualmicro.com/
			This file is for intellisense purpose only.
			Visual micro (and the arduino ide) ignore this code during compilation. This code is automatically maintained by visualmicro, manual changes to this file will be overwritten
			The contents of the _vm sub folder can be deleted prior to publishing a project
			All non-arduino files created by visual micro and all visual studio project or solution files can be freely deleted and are not required to compile a sketch (do not delete your own code!).
			Note: debugger breakpoints are stored in '.sln' or '.asln' files, knowledge of last uploaded breakpoints is stored in the upload.vmps.xml file. Both files are required to continue a previous debug session without needing to compile and upload again
	
	Hardware: Arduino Mega (ATmega1280), Platform=avr, Package=arduino
*/

#if defined(_VMICRO_INTELLISENSE)

#ifndef _VSARDUINO_H_
#define _VSARDUINO_H_
#define __AVR_atmega1280__
#define __AVR_ATmega1280__
#define ARDUINO 10900
#define ARDUINO_MAIN
#define __AVR__
#define __avr__
#define F_CPU 16000000L
#define __cplusplus 201103L
#define ARDUINO_ARCH_AVR
#define ARDUINO_AVR_MEGA
#define __inline__
#define __asm__(x)
#define __extension__
#define __ATTR_PURE__
#define __ATTR_CONST__
#define __inline__
#define __volatile__
#define __AVR__
typedef void *__builtin_va_list;
#define __builtin_va_start
#define __builtin_va_end
//#define __DOXYGEN__
#define __attribute__(x)
#define NOINLINE __attribute__((noinline))
#define prog_void
#define PGM_VOID_P int

#define NEW_H
            
typedef unsigned char byte;
extern "C" void __cxa_pure_virtual() {;}
#include "arduino.h"
#include <pins_arduino.h> 
#undef cli
#define cli()
#include "ArduCopter.pde"
#include "AP_State.pde"
#include "Attitude.pde"
#include "GCS_Mavlink.pde"
#include "Log.pde"
#include "Parameters.pde"
#include "UserCode.pde"
#include "commands.pde"
#include "commands_logic.pde"
#include "compassmot.pde"
#include "compat.pde"
#include "control_acro.pde"
#include "control_althold.pde"
#include "control_auto.pde"
#include "control_autotune.pde"
#include "control_circle.pde"
#include "control_drift.pde"
#include "control_flip.pde"
#include "control_guided.pde"
#include "control_land.pde"
#include "control_loiter.pde"
#include "control_ofloiter.pde"
#include "control_poshold.pde"
#include "control_rtl.pde"
#include "control_sport.pde"
#include "control_stabilize.pde"
#include "crash_check.pde"
#include "ekf_check.pde"
#include "events.pde"
#include "failsafe.pde"
#include "fence.pde"
#include "flight_mode.pde"
#include "heli.pde"
#include "heli_control_acro.pde"
#include "heli_control_stabilize.pde"
#include "inertia.pde"
#include "land_detector.pde"
#include "leds.pde"
#include "motor_test.pde"
#include "motors.pde"
#include "navigation.pde"
#include "perf_info.pde"
#include "position_vector.pde"
#include "radio.pde"
#include "sensors.pde"
#include "setup.pde"
#include "switches.pde"
#include "system.pde"
#include "test.pde"
#endif
#endif
