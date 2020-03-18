#include <Arduino.h>
#include <TinyWireS.h>
#include "cs01_register_space.h"
#include "cs01_i2c_slave.h"

uint16_t r,g,b,t;
uint16_t rgb_vals [3];

unsigned int arr [128];
int led_delay;

unsigned char RequestedRegister;
bool sense_enable = 0;


// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(uint8_t howMany)
{
  if(howMany == 1)  // Read register
  {
    RequestedRegister =  receive_i2c_data8();
  }
  else
    if (howMany == 3)  // Write register
    {
      RequestedRegister =  receive_i2c_data8();
      unsigned int value = receive_i2c_data16();
      switch(RequestedRegister)
      {
        case CS01_COMMAND: {
          // command regsiter
          command_handler (value);
          break;
        }
        default: {
          arr[RequestedRegister] = value;    
          break;  
        }  
      }
    }
    else // empty the receive buffer
      for (int i=0; i<howMany; i++)
        TinyWireS.receive();
}

void requestEvent()
{
  uint16_t data;
  switch(RequestedRegister)
  {
    case 0: {    
      data = 0xffff;      
      break;
    }
    case CS01_CLASSIFIED_COLOR: {
      color_classifier();
      data = color;
      break;
    }
    default: {
      data = (uint16_t) arr[RequestedRegister];
      break;
    }
  }
  send_i2c_data16(data);
}

void command_handler (int command) {  
  switch (command) {
    case CS01_DISABLE: {
      sense_enable = 0;  
      break;    
    }  
    case CS01_ENABLE: {
      sense_enable = 1;
      break;
    }  
    case CS01_R_LED_ON: {
      digitalWrite(RED_PIN,HIGH);
      break;
    }
    case CS01_G_LED_ON: {
      digitalWrite(GREEN_PIN,HIGH);
      break;
    }
    case CS01_B_LED_ON: {
      digitalWrite(BLUE_PIN,HIGH);
      break;
    }     
    case CS01_R_LED_OFF: {
      digitalWrite(RED_PIN,LOW);
      break;
    }
    case CS01_G_LED_OFF: {
      digitalWrite(GREEN_PIN,LOW);
      break;
    }
    case CS01_B_LED_OFF: {
      digitalWrite(BLUE_PIN,LOW);
      break;
    }
    case CS01_READ_SENSOR: {
      arr[100] = analogRead(COLOR_IN_PIN);
      break;
    }
    case CS01_ALL_LED_OFF: {
      digitalWrite(RED_PIN,LOW);
      digitalWrite(GREEN_PIN,LOW);
      digitalWrite(BLUE_PIN,LOW);
      break;
    } 
    case CS01_CLIBRATION: {
      cs_cal();
      break;
    }  
    default: {
      break;
    }
  }  
}

void setup()
{               
  for (int i=0;i<NUM_LED_PINS;i++){
      pinMode(RGB_PINS[i],OUTPUT);
      digitalWrite(RGB_PINS[i],LOW);
  }
  pinMode(COLOR_IN_PIN, INPUT);
  pinMode(5, INPUT);
  r = 250;
  g = 500;
  b = 750;
  t = 1023;
  arr[red_min] = 20000;

  TinyWireS.begin(I2C_ADDRESS);
  TinyWireS.onReceive(receiveEvent); //data written from master to slave
  TinyWireS.onRequest(requestEvent); //data requested by the master
  led_delay = 10;
  arr[CS01_BLACK_MAX] = 300;
  arr[CS01_RED_MAX] = 500;
  
}

void loop()
{
  TinyWireS_stop_check();
  read_sensor();
}

// per implementation functions (different between uno and tiny)
void send_i2c_data16(uint16_t data){
  TinyWireS.send(data & 0xff); // low byte
  TinyWireS.send(data >> 8);   // high byte
}
byte receive_i2c_data8() {
  byte result;
  result = TinyWireS.receive();
  return result;
}
uint16_t receive_i2c_data16() {
  uint16_t result;
  result = TinyWireS.receive() << 8 | TinyWireS.receive();
  return result;
}

void read_sensor(){
  //if(sense_enable == 1){
    digitalWrite(GREEN_PIN, LOW);
    digitalWrite(RED_PIN, LOW);
    digitalWrite(BLUE_PIN, LOW);
    int green = 0;
    int red = 0;
    int blue = 0;
    int total = 0;

    digitalWrite(GREEN_PIN, HIGH);
    delay(10);
    for(int i = 0; i < 10; i++){
      green += analogRead(COLOR_IN_PIN);
    }
    digitalWrite(GREEN_PIN, LOW);

    digitalWrite(RED_PIN, HIGH);
    delay(10);
    for(int i = 0; i < 10; i++){
      red += analogRead(COLOR_IN_PIN);
    }
    digitalWrite(RED_PIN, LOW);
    
    digitalWrite(BLUE_PIN, HIGH);
    delay(10);
    for(int i = 0; i < 10; i++){
      blue += analogRead(COLOR_IN_PIN);
    }
    digitalWrite(BLUE_PIN, LOW);
    
    total = red + green + blue / 30;
    arr [CS01_GREEN_SENSOR_VALUE] = green / 10;
    arr [CS01_RED_SENSOR_VALUE] = red / 10;
    arr [CS01_BLUE_SENSOR_VALUE] = blue / 10;
    arr [CS01_TOTAL_SENSOR_VALUE] = total;
  //}
}

void cs_cal(){
  for(int i = 0; i<10; i++){
    read_sensor();
    if(arr[3] > arr[red_max]){
      arr[red_max] = arr[3];
    }
    if(arr[3] < arr[red_min]){
      arr[red_min] = arr[3];
    }
  }
  arr[CS01_RED_MAX] = (arr[red_max] + 50);
  arr[CS01_BLACK_MAX] = (arr[red_min] - 50);
}

void color_classifier (){
  color = UNKNON;
  bool flag = 0;
  read_sensor();
  if((arr[3] > arr[CS01_BLACK_MAX]) and (arr[3] < arr[CS01_RED_MAX])){
    color = RED;
    flag = 1;
  }

  if(arr[3] > arr[CS01_RED_MAX]){// and (arr[3] < arr[white_min])){
    if(flag == 0){
      color = WHITE;
    }
    else{
      color = UNKNON;
    }
  }

  if(arr[3] < arr[CS01_BLACK_MAX]){// and (arr[3] < arr[black_min])){
    if(flag == 0){
      color = BLACK;
    }
    else{
      color = UNKNON;
    }
  }
  arr[127] = color;
}