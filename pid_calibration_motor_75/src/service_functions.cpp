#include "service_functions.h"

void eeprom_2_serial(){
    Serial.begin(9600);
    Serial.println("EEPROM contents:");
    uint16_t addr = 0;
    while(addr < (uint16_t)(((int)EEPROM.length() -1))){        
        Serial.println(EEPROMReadInt(addr));
        addr = addr + 2;
        delay(200);
    }
    
}


void EEPROMWriteInt(int address, int value)
{
  byte two = (value & 0xFF);
  byte one = ((value >> 8) & 0xFF);
  
  EEPROM.update(address, two);
  EEPROM.update(address + 1, one);
}

int EEPROMReadInt(int address)
{
  long two = EEPROM.read(address);
  long one = EEPROM.read(address + 1);
 
  return ((two << 0) & 0xFFFFFF) + ((one << 8) & 0xFFFFFFFF);
}