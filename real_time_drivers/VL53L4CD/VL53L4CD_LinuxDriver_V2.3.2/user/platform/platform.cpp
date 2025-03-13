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

#include <fcntl.h> // open()
#include <unistd.h> // close()
#include <time.h> // clock_gettime()

#include <linux/i2c.h>
#include <linux/i2c-dev.h>

#include <thread> // adding cpp header files for real time interrupt
#include <gpiod.h>

#include <sys/ioctl.h>

#include "platform.h"
#include "types.h"
#include "../uld-driver/VL53L4CD_api.h"

#define VL53L4CD_ERROR_GPIO_SET_FAIL	1
#define VL53L4CD_COMMS_ERROR		2
#define VL53L4CD_ERROR_TIME_OUT		3

#define SUPPRESS_UNUSED_WARNING(x) \
	((void) (x))

#define VL53L4CD_COMMS_CHUNK_SIZE  1024

#define LOG 				printf

#ifndef STMVL53L4CD_KERNEL
static uint8_t i2c_buffer[VL53L4CD_COMMS_CHUNK_SIZE];
#else
struct comms_struct {
	uint16_t   len;
	uint16_t   reg_address;
	uint8_t    write_not_read;
	uint8_t    padding[3]; /* 64bits alignment */
	uint64_t   bufptr;
};
#endif

#define ST_TOF_IOCTL_TRANSFER           _IOWR('a',0x1, struct comms_struct)
#define ST_TOF_IOCTL_WAIT_FOR_INTERRUPT	_IO('a',0x2)

uint8_t VL53L4CD_comms_init(Dev_t dev)
{

#ifdef STMVL53L4CD_KERNEL
	LOG("Kernel com init\n");
	dev->fd = open("/dev/stmvl53l4cd", O_RDONLY);
	if (dev->fd == -1) {
		LOG("Failed to open /dev/stmvl53l4cd\n");
		return VL53L4CD_COMMS_ERROR;
	}
#else
	LOG("User space com init\n");

	/* Create sensor at default I2C address */
	dev->address = 0x52;
	dev->fd = open("/dev/i2c-1", O_RDONLY);
	if (dev->fd == -1) {
		LOG("Failed to open /dev/i2c-1\n");
		return VL53L4CD_COMMS_ERROR;
	}

	if (ioctl(dev->fd, I2C_SLAVE, dev->address) <0) {
		LOG("Could not speak to the device on the i2c bus\n");
		return VL53L4CD_COMMS_ERROR;
	}
#endif

	LOG("Opened ST TOF Dev = %d\n", dev->fd);

	return 0;
}

uint8_t VL53L4CD_comms_close(Dev_t dev)
{
	close(dev->fd);
	return 0;
}

uint8_t write_read_multi(
		Dev_t dev,
		uint16_t reg_address,
		uint8_t *pdata,
		uint32_t count,
		int write_not_read)
{
#ifdef STMVL53L4CD_KERNEL
	struct comms_struct cs;

	cs.len = count;
	cs.reg_address = reg_address;
	cs.bufptr = (uint64_t)(uintptr_t)pdata;
	cs.write_not_read = write_not_read;

	if (ioctl(dev->fd, ST_TOF_IOCTL_TRANSFER, &cs) < 0)
		return VL53L4CD_COMMS_ERROR;
#else

	struct i2c_rdwr_ioctl_data packets;
	struct i2c_msg messages[2];

	uint32_t data_size = 0;
	uint32_t position = 0;

	if (write_not_read) {
		do {
			data_size = (count - position) > (VL53L4CD_COMMS_CHUNK_SIZE-2) ? (VL53L4CD_COMMS_CHUNK_SIZE-2) : (count - position);

			memcpy(&i2c_buffer[2], &pdata[position], data_size);

			i2c_buffer[0] = (reg_address + position) >> 8;
			i2c_buffer[1] = (reg_address + position) & 0xFF;

			messages[0].addr = dev->address >> 1;
			messages[0].flags = 0; //I2C_M_WR;
			messages[0].len = data_size + 2;
			messages[0].buf = i2c_buffer;

			packets.msgs = messages;
			packets.nmsgs = 1;

			if (ioctl(dev->fd, I2C_RDWR, &packets) < 0)
				return VL53L4CD_COMMS_ERROR;
			position +=  data_size;

		} while (position < count);
	}

	else {
		do {
			data_size = (count - position) > VL53L4CD_COMMS_CHUNK_SIZE ? VL53L4CD_COMMS_CHUNK_SIZE : (count - position);

			i2c_buffer[0] = (reg_address + position) >> 8;
			i2c_buffer[1] = (reg_address + position) & 0xFF;

			messages[0].addr = dev->address >> 1;
			messages[0].flags = 0; //I2C_M_WR;
			messages[0].len = 2;
			messages[0].buf = i2c_buffer;

			messages[1].addr = dev->address >> 1;
			messages[1].flags = I2C_M_RD;
			messages[1].len = data_size;
			messages[1].buf = pdata + position;

			packets.msgs = messages;
			packets.nmsgs = 2;

			if (ioctl(dev->fd, I2C_RDWR, &packets) < 0)
				return VL53L4CD_COMMS_ERROR;

			position += data_size;

		} while (position < count);
	}

#endif

	return 0;
}

uint8_t write_multi(
		Dev_t dev,
		uint16_t reg_address,
		uint8_t *pdata,
		uint32_t count)
{
	return(write_read_multi(dev, reg_address, pdata, count, 1));
}

uint8_t read_multi(
		Dev_t dev,
		uint16_t reg_address,
		uint8_t *pdata,
		uint32_t count)
{
	return(write_read_multi(dev, reg_address, pdata, count, 0));
}


uint8_t VL53L4CD_RdDWord(Dev_t dev, uint16_t RegisterAdress, uint32_t *value)
{
	uint8_t status = 0;
	uint8_t data_read[4];

	status = read_multi(dev, RegisterAdress, (uint8_t*)data_read, 4);
	*value =  ((data_read[0] << 24) | (data_read[1]<<16) |
			(data_read[2]<<8)| (data_read[3]));

	return status;
}

uint8_t VL53L4CD_RdWord(Dev_t dev, uint16_t RegisterAdress, uint16_t *value)
{
	uint8_t status = 0;
	uint8_t data_read[2];

	status = read_multi(dev, RegisterAdress, (uint8_t*)data_read, 2);
	*value = (data_read[0] << 8) | (data_read[1]);
	return status;
}

uint8_t VL53L4CD_RdByte(Dev_t dev, uint16_t RegisterAdress, uint8_t *value)
{
	uint8_t status = 0;
	uint8_t data_read[1];

	status = read_multi(dev, RegisterAdress, (uint8_t*)data_read, 1);
	*value = data_read[0];
	return status;
}

uint8_t VL53L4CD_WrByte(Dev_t dev, uint16_t RegisterAdress, uint8_t value)
{
	uint8_t data_write[1];

	data_write[0] = value & 0xFF;
	return(write_multi(dev, RegisterAdress, (uint8_t*)data_write, 1));
}

uint8_t VL53L4CD_WrWord(Dev_t dev, uint16_t RegisterAdress, uint16_t value)
{
	uint8_t data_write[2];

	data_write[0] = (value >> 8) & 0xFF;
	data_write[1] = value & 0xFF;
	return(write_multi(dev, RegisterAdress, (uint8_t*)data_write, 2));
}

uint8_t VL53L4CD_WrDWord(Dev_t dev, uint16_t RegisterAdress, uint32_t value)
{	
	uint8_t data_write[4];

	data_write[0] = (value >> 24) & 0xFF;
	data_write[1] = (value >> 16) & 0xFF;
	data_write[2] = (value >> 8) & 0xFF;
	data_write[3] = value & 0xFF;
	return(write_multi(dev, RegisterAdress, (uint8_t*)data_write, 4));
}

uint8_t VL53L4CD_ReadMulti(
        Dev_t dev,
        uint16_t RegisterAdress,
        uint8_t *p_values,
        uint32_t size)
{
	return(read_multi(dev, RegisterAdress, (uint8_t*)p_values, size));
}

uint8_t	 VL53L4CD_WaitMs(Dev_t dev, uint32_t time_ms)
{
	usleep(time_ms*1000);
	return 0;
}

uint8_t VL53L4CD_IsDataReady(Dev_t dev)
{
#ifdef STMVL53L4CD_KERNEL
	if (ioctl(dev->fd, ST_TOF_IOCTL_WAIT_FOR_INTERRUPT) < 0)
		return 0;
#else
	uint8_t isReady = 0;
	do {
		VL53L4CD_WaitMs(dev, 5);
		VL53L4CD_CheckForDataReady(dev, &isReady);		
	} while (isReady == 0);
#endif
	return 1;
}

uint8_t VL53L4CD_IsDataReady_Ready_Interrupt_Based(Dev_t dev)
{	
	//int running = 1;

	//pin
	//while(running){
		//const struct timespec ts = (1,0);
		//gpiod_line_event_wait(, &ts);
	
	//}
		
	return 1;
}
