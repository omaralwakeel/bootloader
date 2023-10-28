# bootloader
This project contains two codes:

A code to upload the bin file to the EEPROM
A code to flash the custom bootloader to the microcontroller
Upload bin file to EEPROM
This code uses the I2C protocol to upload the bin file to the EEPROM. The bin file can be any type of file, such as a firmware image or a configuration file.

To use this code:

Connect the EEPROM to the microcontroller using the I2C interface.
Compile and upload the code to the microcontroller.
Open a terminal emulator and connect to the microcontroller's serial port.
Type the following command to upload the bin file to the EEPROM:
upload <bin_file_path>
For example:

upload firmware.bin
Flash custom bootloader
This code uses the I2C protocol to flash the custom bootloader to the microcontroller. The custom bootloader must be in a bin file format.

To use this code:

Connect the microcontroller to the computer using the I2C interface.
Compile and upload the code to the computer.
Open a terminal emulator and navigate to the directory where the custom bootloader bin file is located.
Type the following command to flash the custom bootloader to the microcontroller:
flash <bootloader_bin_file_path>
drive.

