/*
 * boot.c
 *
 *  Created on: Oct 27, 2023
 *      Author: OMAR
 */

#include "boot.h"
static  void read_data_from_EEPROM(uint8_t *data, uint16_t data_length, uint16_t address);
static  uint16_t read_data_length_from_EEPROM(uint16_t address);
void bootloader_jump_to_user_app(void){
	/* Value of the main stack pointer of our main application */
	uint32_t MSP_Value = *((volatile uint32_t *)FLASH_SECTOR2_BASE_ADDRESS);

	/* Reset Handler definition function of our main application */
	uint32_t MainAppAddr = *((volatile uint32_t *)(FLASH_SECTOR2_BASE_ADDRESS + 4));

	/* Fetch the reset handler address of the user application */
	pMainApp ResetHandler_Address = (pMainApp)MainAppAddr;

	/* Set Main Stack Pointer */
	__set_MSP(MSP_Value);

	/* DeInitialize / Disable of modules */
	HAL_RCC_DeInit(); /* DeInitialize the RCC clock configuration to the default reset state. */
	                  /* Disable Maskable Interrupt */


	/* Jump to Application Reset Handler */
	ResetHandler_Address();
}

void flash_app_eeprom(void){
	 uint8_t data;
	 uint16_t data_size=0;
	 data_size=read_data_length_from_EEPROM(EEPROM_LEANTH_Address);
	 if(data_size!=0){
		 for(int i=0;i<=data_size;i++){
			 read_data_from_EEPROM(&data,data_size,EEPROM_Start_Address+i);
			 HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, FLASH_SECTOR2_BASE_ADDRESS, data);

		 }


	 }
 }

uint16_t read_data_length_from_EEPROM(uint16_t address)
{
  uint8_t cmd[1];
  uint16_t data_length;

  /* Send read command */
  cmd[0] = EEPROM_READ_CMD;
  HAL_I2C_Mem_Write(&hi2c1, EEPROM_ADDRESS, address, 1, cmd, 1, 1000);

  /* Read data length from EEPROM */
  HAL_I2C_Mem_Read(&hi2c1, EEPROM_ADDRESS, address, 1, (uint8_t *)&data_length, sizeof(data_length), 1000);

  return data_length;
}
void read_data_from_EEPROM(uint8_t *data, uint16_t data_length, uint16_t address)
{
  uint8_t cmd[1];

  /* Send read command */
  cmd[0] = EEPROM_READ_CMD;
  HAL_I2C_Mem_Write(&hi2c1, EEPROM_ADDRESS, address, 1, cmd, 1, 1000);

  /* Read data from EEPROM */
  HAL_I2C_Mem_Read(&hi2c1, EEPROM_ADDRESS, address, 1, data, data_length, 1000);
}
