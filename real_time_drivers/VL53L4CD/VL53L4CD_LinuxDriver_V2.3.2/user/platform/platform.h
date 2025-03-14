/**
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#ifndef _PLATFORM_H_
#define _PLATFORM_H_
#pragma once

#include <stdint.h>
#include <string.h>


typedef struct {
	int      fd;    	/* File descriptor */
	uint16_t  address;    	/* I2C address of sensor */
} VL53L4CD_LinuxDev;
/**
* VL53L4CD device instance.
*/

typedef VL53L4CD_LinuxDev* Dev_t;

/**
 * @brief Error instance.
 */
typedef uint8_t VL53L4CD_Error;

/**
 * @brief If the macro below is defined, the device will be programmed to run
 * with I2C Fast Mode Plus (up to 1MHz). Otherwise, default max value is 400kHz.
 */

//#define VL53L4CD_I2C_FAST_MODE_PLUS


/**
 * @brief Init communication
 */

uint8_t VL53L4CD_comms_init(Dev_t dev);

/**
 * @brief Close communication
 */

uint8_t VL53L4CD_comms_close(Dev_t dev);

/**
 * @brief Read 32 bits through I2C.
 */

uint8_t VL53L4CD_RdDWord(Dev_t dev, uint16_t registerAddr, uint32_t *value);
/**
 * @brief Read 16 bits through I2C.
 */

uint8_t VL53L4CD_RdWord(Dev_t dev, uint16_t registerAddr, uint16_t *value);

/**
 * @brief Read 8 bits through I2C.
 */

uint8_t VL53L4CD_RdByte(Dev_t dev, uint16_t registerAddr, uint8_t *value);

/**
 * @brief Write 8 bits through I2C.
 */

uint8_t VL53L4CD_WrByte(Dev_t dev, uint16_t registerAddr, uint8_t value);

/**
 * @brief Write 16 bits through I2C.
 */

uint8_t VL53L4CD_WrWord(Dev_t dev, uint16_t RegisterAdress, uint16_t value);

/**
 * @brief Write 32 bits through I2C.
 */

uint8_t VL53L4CD_WrDWord(Dev_t dev, uint16_t RegisterAdress, uint32_t value);

/**
 * @brief Wait during N milliseconds.
 */

uint8_t VL53L4CD_WaitMs(Dev_t dev, uint32_t TimeMs);

/**
 * @brief This function is used to wait for a new measurement. It can 
 * support both interrupt mode with kernel module and polling mode
 * @param (Dev_t) dev : Pointer of VL53L4CD_LinuxDev structure.
 * @return (uint8_t) status : 1 if data is ready
 */

 /*
uint8_t VL53L4CD_IsDataReady(Dev_t dev);

uint8_t VL53L4CD_IsDataReady_Ready_Interrupt_Based(Dev_t dev);
*/
#endif	// _PLATFORM_H_