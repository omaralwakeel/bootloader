/*
 * Bootloader.h
 *
 *  Created on: Oct 27, 2023
 *      Author: OMAR
 */

#ifndef BOOTLOADER_H_
#define BOOTLOADER_H_
/* ----------------- Includes -----------------*/
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "usart.h"
#include "crc.h"
#include "i2c.h"
/* ----------------- Macro Declarations -----------------*/
#define BL_UART                		 &huart1
#define WRITE_IN_MEM                 &hi2c2
#define CRC_ENGINE_OBJ               &hcrc
#define EEPROM_ADD					   0XA0
#define EEPROM_ERASE_CMD 			   0x03
#define DEBUG_INFO_DISABLE             0
#define DEBUG_INFO_ENABLE              1


#define BL_HOST_BUFFER_RX_LENGTH   	   200

#define CBL_GET_VER_CMD                0x10
#define CBL_GET_HELP_CMD               0x11
#define CBL_FLASH_ERASE_CMD            0x12
#define CBL_MEM_WRITE_CMD              0x13
#define CBL_MEM_READ_CMD               0x14


#define INVALID_SECTOR_NUMBER          0x00
#define VALID_SECTOR_NUMBER            0x01
#define UNSUCCESSFUL_ERASE             0x02
#define SUCCESSFUL_ERASE               0x03

#define FLASH_PAYLOAD_WRITE_FAILED     0x00
#define FLASH_PAYLOAD_WRITE_PASSED     0x01

#define CRC_TYPE_SIZE_BYTE            4
#define CRC_FAILED     				   0x00
#define CRC_SUCSSEC     			   0x01
#define CBL_SEND_NACK                  0xAB
#define CBL_SEND_ACK                   0xCD

/* EEPROM write command */
#define EEPROM_WRITE_CMD 0x02

/* Maximum number of bytes to write to EEPROM at once */
#define EEPROM_WRITE_MAX_BYTES 255

#define EEPROM_Start_Address   0x10
#define EEPROM_LEANTH_Address   0x0A
/* ----------------- Macro Functions Declarations -----------------*/


/* ----------------- Data Type Declarations -----------------*/

typedef enum{
	BL_NACK = 0,
	BL_OK
}BL_Status;

typedef void (*pMainApp)(void);
typedef void (*Jump_Ptr)(void);

/* ----------------- Software Interfaces Declarations -----------------*/
//void BL_Print_Message(char *format, ...);
BL_Status BL_UART_Fetch_Host_Command(void);

#endif /* BOOTLOADER_H_ */
