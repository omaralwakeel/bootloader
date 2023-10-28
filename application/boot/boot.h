/*
 * boot.h
 *
 *  Created on: Oct 27, 2023
 *      Author: OMAR
 */

#ifndef BOOT_H_
#define BOOT_H_
#include "stm32f4xx_hal.h"
#include "i2c.h"
#include "gpio.h"
#include "boot.h"
#define FLASH_SECTOR2_BASE_ADDRESS  0x08000400
#define EEPROM_Start_Address   0x10
#define EEPROM_LEANTH_Address   0x0A

typedef void (*pMainApp)(void);

/* EEPROM device address */
#define EEPROM_ADDRESS 0xA0

/* Read command */
#define EEPROM_READ_CMD 0x01

/* Write command */
#define EEPROM_WRITE_CMD 0x02

/* Clear data size command */
#define EEPROM_CLEAR_DATA_SIZE_CMD 0x04

 void bootloader_jump_to_user_app(void);
 void flash_app_eeprom(void);
#endif /* BOOT_H_ */
