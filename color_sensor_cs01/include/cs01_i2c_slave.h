#include <Arduino.h>
#include <TinyWireS.h>

#define ATTINY85

void requestEvent();
//void ReceiveEvent(int howMany);
void receiveEvent(uint8_t howMany);
void command_handler (int command);
void send_i2c_data16(uint16_t data);
byte receive_i2c_data8();
uint16_t receive_i2c_data16();
void read_sensor();
void final_sensor_valeu();
void cs_cal();
void color_classifier ();

const int I2C_ADDRESS = 4;

const int RED_PIN = PB1; //=D11 on uno/nano
const int BLUE_PIN = PB3; //=D13 on uno/nano, reset pin on Tiny85
const int GREEN_PIN = PB5; //=D9 on uno/nano
const int COLOR_IN_PIN = 2; //=D12 on uno/nano

typedef enum color {RED,WHITE,BLACK,UNKNON} color_type;
color_type color;

//const int RGB_PINS [3] = {RED_PIN,GREEN_PIN,BLUE_PIN};
//const int RGB_PINS [3] = {RED_PIN,GREEN_PIN,BLUE_PIN};


// #ifdef ATTINY85
    // #define NUM_LED_PINS 2
    // const int RGB_PINS [NUM_LED_PINS] = {RED_PIN,GREEN_PIN};
// #else
    #define NUM_LED_PINS 3
    const int RGB_PINS [NUM_LED_PINS] = {RED_PIN,GREEN_PIN,BLUE_PIN};
// #endif