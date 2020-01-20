#include <Arduino.h>
#include <TinyWireS.h>
#include "cs01_i2c_slave.h"
//#include <USI_TWI_Master.h>


uint8_t r,g,b;
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
    RequestedRegister = TinyWireS.receive();
  }
  else
    if (howMany == 3)  // Write register
    {
      RequestedRegister = TinyWireS.receive();
      unsigned int value = TinyWireS.receive() << 8 | TinyWireS.receive();  // Get two byte value
      switch(RequestedRegister)
      {
        case 0: {
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
  switch(RequestedRegister)
  {
    case 0: {    
      TinyWireS.send(0xFF);
      break;
    }
//  case 1:   
//  case 2:   
//  case 3:       
  default: {
      TinyWireS.send(arr[RequestedRegister] & 0xff); // low byte
      TinyWireS.send(arr[RequestedRegister] >> 8);   // high byte
      break;
    }
  }
}

void command_handler (int command) {  
  switch (command) {
    case 0: {
      sense_enable = sense_enable == 0 ? 1:0;
    }  
    case 1: {
      digitalWrite(RGB_PINS[0],HIGH);
      break;
    }
    case 2: {
      digitalWrite(RGB_PINS[0],LOW);
      break;
    }
    case 3: {
      digitalWrite(RGB_PINS[1],HIGH);
      break;
    }
    case 4: {
      digitalWrite(RGB_PINS[1],LOW);
      break;
    }    
#ifndef ATTINY85    
    case 5: {
      digitalWrite(RGB_PINS[2],HIGH);
      break;
    }
    case 6: {
      digitalWrite(RGB_PINS[2],LOW);
      break;
    }    
#endif    
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
  r=LOW;
  g=LOW;
  b=LOW;
    
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
