#include <Arduino.h>
//#include <TinyWireS.h>


void requestEvent();
//void ReceiveEvent(int howMany);
#ifdef ATTINY85
void receiveEvent(uint8_t howMany);
#else
void receiveEvent(int howMany);
#endif

void command_handler (int command);
void send_i2c_data16(uint16_t data);
byte receive_i2c_data8();
uint16_t receive_i2c_data16();
void writeToSD(char* filename, char* str);

const int I2C_ADDRESS = 11;
const int SD_CS = 10;
const int LED = 13;

int filename_idx = 0;
char filename[11] = "log000.csv";

char csv_header[100] = "time[ms],line_sensor_value";
char csv_row[100] = "";
