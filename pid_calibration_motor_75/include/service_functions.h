#include <Arduino.h>
#include <EEPROM.h>

void EEPROMWriteInt(int address, int value);
int EEPROMReadInt(int address);

void eeprom_2_serial();