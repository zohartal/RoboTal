#include <Arduino.h>
#include <TinyWireS.h>
#include "cs01_register_space.h"
#include "cs01_i2c_slave.h"
//#include <USI_TWI_Master.h>


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
    case CS01_RED_SENSOR_VALUE: {
      data = r;
      break;
    }
    case CS01_GREEN_SENSOR_VALUE: {
      data = g;
      break;
    }    
    case CS01_BLUE_SENSOR_VALUE: {
      data = b;
      break;
    }    
    case CS01_TOTAL_SENSOR_VALUE: {
      data = t;
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
    }  
    case CS01_ENABLE: {
      sense_enable = 1;      
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
    case CS01_ALL_LED_OFF: {
      digitalWrite(RED_PIN,LOW);
      digitalWrite(GREEN_PIN,LOW);
      digitalWrite(BLUE_PIN,LOW);
      break;
    }            
    default: {
      break;
    }
  }  
}

void setup()
{               
//  Serial.begin(9600);
  for (int i=0;i<NUM_LED_PINS;i++){
      pinMode(RGB_PINS[i],OUTPUT);
      digitalWrite(RGB_PINS[i],LOW);
  }
  r = 250;
  g = 500;
  b = 750;
  t = 1023;

  TinyWireS.begin(I2C_ADDRESS);
  TinyWireS.onReceive(receiveEvent); //data written from master to slave
  TinyWireS.onRequest(requestEvent); //data requested by the master
//  Serial.println("I2C slave is up");
  led_delay = 10;
}

void loop()
{
    TinyWireS_stop_check();

//    if (sense_enable == 1){
//       for (int i=0;i<3;i++){
//         digitalWrite(RGB_PINS[i],HIGH);
//         delay(led_delay);
//        rgb_vals[i] = 0;
//       
//        for (int j=0;j<5;j++){
//          arr[i+1] += analogRead(COLOR_IN_PIN);      
//        }
//         digitalWrite(RGB_PINS[i],LOW);
//       }
//    }
//    else{
//    }
    
   
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
