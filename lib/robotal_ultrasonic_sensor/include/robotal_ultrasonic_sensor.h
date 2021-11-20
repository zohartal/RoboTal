#include <Arduino.h>
#include <TinyWireS.h>

#define ATTINY85

void requestEvent();
void receiveEvent(uint8_t howMany);
void send_i2c_data16(uint16_t data);
byte receive_i2c_data8();
uint16_t receive_i2c_data16();

void read_sensor();
void command_handler (int command);

const int USS01_I2C_ADDRESS = 5;

const int TRIG_PIN = PB4; 
const int ECHO_PIN = PB3; 
