#include <Arduino.h>
#include <TinyWireS.h>
#include "robotal_ultrasonic_sensor_register_space.h"
#include "robotal_ultrasonic_sensor.h"

unsigned int arr [128];
unsigned int delay_loop_cnt;

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
        case USS01_COMMAND: {
          // command regsiter
          command_handler (value);
          break;
        }
        case USS01_LAST_VALUE: {          
          // RO register, cannot write it
          break;
        }        
        default: {
          arr[RequestedRegister] = value;    
          break;  
        }  
      }
    }
    else // this is an ERROR case - empty the receive buffer
      for (int i=0; i<howMany; i++)
        TinyWireS.receive();
}

void requestEvent()
{
  uint16_t data;
  switch(RequestedRegister)
  {
    case 0:    
     {    
      data = 0xffff;      
      break;
    }
    default: {
      data = (uint16_t) arr[RequestedRegister];
      break;
    }
  }
  send_i2c_data16(data);
}

void setup()
{               
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);  

  TinyWireS.begin(USS01_I2C_ADDRESS);
  TinyWireS.onReceive(receiveEvent); //data written from master to slave
  TinyWireS.onRequest(requestEvent); //data requested by the master 

  delay_loop_cnt = 0;
  arr[USS01_TRIGGER_INTERVAL] = 0;
}

void loop()
{
  TinyWireS_stop_check();
  // if ((sense_enable) && (delay_loop_cnt>= arr[USS01_TRIGGER_INTERVAL])) {
    read_sensor();
  //   delay_loop_cnt = 0;
  // }
  
  // add delay machanism
  delay (1);
  delay_loop_cnt++;
  arr[USS01_COUNTER_VALUE] = delay_loop_cnt;
}

void read_sensor(){
    // defines variables
    unsigned long duration; // variable for the duration of sound wave travel
    double distance; // variable for the distance measurement

    // add code for reading the sensor
    // Clears the trigPin condition
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(ECHO_PIN, HIGH);
    // Calculating the distance
    distance = (((double) duration) * 0.034) / 2; // Speed of sound wave divided by 2 (go and back)
    arr[USS01_LAST_VALUE] = (unsigned int) distance;
}

void command_handler (int command) {  
  switch (command) {
    case USS01_DISABLE: {
      sense_enable = 0;  
      break;    
    }  
    case USS01_ENABLE: {
      sense_enable = 1;
      break;
    }  
    case USS01_MANUAL_TRIGGER: {
      read_sensor();
      break;    
    }  
    default: {
      break;
    }
  }  
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