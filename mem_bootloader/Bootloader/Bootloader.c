/*
 * Bootloader.c
 *
 *  Created on: Oct 27, 2023
 *      Author: OMAR
 */


/* ----------------- Includes -----------------*/
#include "bootloader.h"

/* ----------------- Static Functions Decleration -----------------*/
static void Bootloader_Get_Version(uint8_t *Host_Buffer);
static void Bootloader_Get_Help(uint8_t *Host_Buffer);

static void Bootloader_Erase_EEPROM(uint8_t *Host_Buffer);
static void Bootloader_EEPROM_Write(uint8_t *Host_Buffer);
//static void Bootloader_EEPROM_Read(uint8_t *Host_Buffer);


static uint8_t Bootloader_CRC_Verify(uint8_t *pData, uint32_t Data_Len, uint32_t Host_CRC);
static void Bootloader_Send_ACK(uint8_t Replay_Len);
static void Bootloader_Send_NACK(void);
//static uint8_t Perform_Flash_Erase(uint8_t Sector_Numebr, uint8_t Number_Of_Sectors);

/* ----------------- Global Variables Definitions -----------------*/
static uint8_t BL_Host_Buffer[BL_HOST_BUFFER_RX_LENGTH];

static uint8_t Bootloader_Supported_CMDs[6] = {
    CBL_GET_VER_CMD,
    CBL_GET_HELP_CMD,
    CBL_FLASH_ERASE_CMD,
    CBL_MEM_WRITE_CMD,
    CBL_MEM_READ_CMD,
};

/* -----------------  Software Interfaces Definitions -----------------*/

BL_Status BL_UART_Fetch_Host_Command(void){
	BL_Status Status = BL_NACK;
	HAL_StatusTypeDef HAL_Status = HAL_ERROR;
	uint8_t Data_Length = 0;

	memset(BL_Host_Buffer, 0, BL_HOST_BUFFER_RX_LENGTH);
	/* Read the length of the command packet received from the HOST */
	HAL_Status = HAL_UART_Receive(BL_UART, BL_Host_Buffer, 1, HAL_MAX_DELAY);
	if(HAL_Status != HAL_OK){
		Status = BL_NACK;
	}
	else{
		Data_Length = BL_Host_Buffer[0];
		/* Read the command packet received from the HOST */
		HAL_Status = HAL_UART_Receive(BL_UART, &BL_Host_Buffer[1], Data_Length, HAL_MAX_DELAY);
		if(HAL_Status != HAL_OK){
			Status = BL_NACK;
		}
		else{
			switch(BL_Host_Buffer[1]){
				case CBL_GET_VER_CMD:
					Bootloader_Get_Version(BL_Host_Buffer);
					Status = BL_OK;
					break;
				case CBL_GET_HELP_CMD:
					Bootloader_Get_Help(BL_Host_Buffer);
					Status = BL_OK;
					break;
				case CBL_FLASH_ERASE_CMD:
					Bootloader_Erase_EEPROM(BL_Host_Buffer);
					Status = BL_OK;
					break;
				case CBL_MEM_WRITE_CMD:
					Bootloader_EEPROM_Write(BL_Host_Buffer);
					Status = BL_OK;
					break;
				/*case CBL_MEM_READ_CMD:
					Bootloader_EEPROM_Read(BL_Host_Buffer);
					Status = BL_OK;
					break;*/
				default:
					break;
			}
		}
	}

	return Status;
}

/* ----------------- Static Functions Definitions -----------------*/

static uint8_t Bootloader_CRC_Verify(uint8_t *pData, uint32_t Data_Len, uint32_t Host_CRC){
	uint8_t CRC_STATUS =CRC_FAILED;
	uint8_t counter=0;
	uint32_t CRC_ACUMLATION=0;
	uint32_t data_buffer=0;
	for(counter=0;counter<Data_Len;counter++){
		data_buffer=(uint32_t)pData[counter];
		CRC_ACUMLATION=HAL_CRC_Accumulate(CRC_ENGINE_OBJ, &data_buffer, 1);
	}
	__HAL_CRC_DR_RESET(CRC_ENGINE_OBJ);
	if(CRC_ACUMLATION==Host_CRC){
		CRC_STATUS=CRC_SUCSSEC;
	}else{
		CRC_STATUS =CRC_FAILED;
	}
	return CRC_STATUS;
}
static void Bootloader_Send_ACK(uint8_t Replay_Len){
	uint8_t ACK_VALUE[2]={0};
	ACK_VALUE[0]= CBL_SEND_NACK;
	ACK_VALUE[1]=Replay_Len;
	HAL_UART_Transmit(BL_UART,(uint8_t *) ACK_VALUE, 2, HAL_MAX_DELAY);
}
static void Bootloader_Send_NACK(void){
	HAL_UART_Transmit(BL_UART,(uint8_t *)CBL_SEND_ACK, 1, HAL_MAX_DELAY);
}

static void Bootloader_Get_Version(uint8_t *Host_Buffer){
	uint8_t version[]={1,0,0};
	uint8_t Data_leanth=Host_Buffer[0]+1;
	uint32_t host_crc=0;
	host_crc=*(uint32_t *)((Host_Buffer[0]+Data_leanth)-CRC_TYPE_SIZE_BYTE);
	uint8_t crc_vrefication=CRC_FAILED;
	crc_vrefication=Bootloader_CRC_Verify((uint8_t *)&Host_Buffer[0], Data_leanth, host_crc);
	if(crc_vrefication==CRC_SUCSSEC){
		Bootloader_Send_ACK(Data_leanth);
		HAL_UART_Transmit(BL_UART,(uint8_t *) version,sizeof(version), HAL_MAX_DELAY);
	}else{Bootloader_Send_NACK();}
}
static void Bootloader_Get_Help(uint8_t *Host_Buffer){
	uint8_t Data_leanth=Host_Buffer[0]+1;
	uint32_t host_crc=0;
	host_crc=*((uint32_t *)((Host_Buffer[0]+Data_leanth)-CRC_TYPE_SIZE_BYTE));
	uint8_t crc_vrefication=CRC_FAILED;
	crc_vrefication=Bootloader_CRC_Verify((uint8_t *)&Host_Buffer[0], Data_leanth, host_crc);
	if(crc_vrefication==CRC_SUCSSEC){
		Bootloader_Send_ACK(Data_leanth);
		HAL_UART_Transmit(BL_UART,Bootloader_Supported_CMDs,sizeof(Bootloader_Supported_CMDs), HAL_MAX_DELAY);
	}else{Bootloader_Send_NACK();}
}

static void Bootloader_Erase_EEPROM(uint8_t *Host_Buffer){
	uint8_t Data_leanth=Host_Buffer[0]+1;
	uint32_t host_crc=0;
	char massage[]="EEPROM IS ERASED";
	host_crc=*((uint32_t *)((Host_Buffer[0]+Data_leanth)-CRC_TYPE_SIZE_BYTE));
	uint8_t crc_vrefication=CRC_FAILED;
	crc_vrefication=Bootloader_CRC_Verify((uint8_t *)&Host_Buffer[0], Data_leanth, host_crc);
	if(crc_vrefication==CRC_SUCSSEC){
		Bootloader_Send_ACK(Data_leanth);
		HAL_I2C_Mem_Write(WRITE_IN_MEM, EEPROM_ADD, 0X00, 1,(uint8_t *) EEPROM_ERASE_CMD, 1, HAL_MAX_DELAY);
		 HAL_Delay(1000);
		HAL_UART_Transmit(BL_UART,(uint8_t *)massage,sizeof(massage), HAL_MAX_DELAY);
	}else{Bootloader_Send_NACK();}
}

void write_data_to_EEPROM(uint8_t *data, uint16_t data_length)
{
  uint8_t cmd[1];

  /* Check if the data length is too long */
  if (data_length > EEPROM_WRITE_MAX_BYTES) {
    data_length = EEPROM_WRITE_MAX_BYTES;
  }

  /* Send write command */
  cmd[0] = EEPROM_WRITE_CMD;
  HAL_I2C_Mem_Write(WRITE_IN_MEM, EEPROM_ADD, (uint16_t)EEPROM_Start_Address, 1, cmd, 1, 1000);

  /* Write data to EEPROM */
  HAL_I2C_Mem_Write(WRITE_IN_MEM, EEPROM_ADD, (uint16_t)EEPROM_Start_Address, 1, (uint8_t *)&data_length, data_length, 1000);
  HAL_I2C_Mem_Write(WRITE_IN_MEM, EEPROM_ADD, (uint16_t)EEPROM_LEANTH_Address, 1, data, data_length, 1000);
}


static void Bootloader_EEPROM_Write(uint8_t *Host_Buffer){
	uint8_t Data_leanth=Host_Buffer[0]+1;
	uint32_t host_crc=0;
	char massage[]="BIN HAS BEEN WRITEN";
	host_crc=*((uint32_t *)((Host_Buffer[0]+Data_leanth)-CRC_TYPE_SIZE_BYTE));
	uint8_t crc_vrefication=CRC_FAILED;
	crc_vrefication=Bootloader_CRC_Verify((uint8_t *)&Host_Buffer[0], Data_leanth, host_crc);
	if(crc_vrefication==CRC_SUCSSEC){
		Bootloader_Send_ACK(Data_leanth);
		write_data_to_EEPROM((uint8_t *)&Host_Buffer[6],Data_leanth);
		HAL_UART_Transmit(BL_UART,(uint8_t *)massage,sizeof(massage), HAL_MAX_DELAY);
	}else{Bootloader_Send_NACK();}
}

//static void Bootloader_EEPROM_Read(uint8_t *Host_Buffer);
