#include <Arduino.h>

#define SD_CARD

// SPI for SD card i/f
#include <SPI.h>
#include <SD.h>



//#include "logger_register_space.h"
#include "logger_i2c_slave.h"

// Wire implements I2C i/f, 
#ifdef ATTINY85 
  //#include <TinyWireS.h>  
#else
  #include <Wire.h>
#endif

uint16_t r,g,b,t;
uint16_t rgb_vals [3];

unsigned int arr [128];
int led_delay;

unsigned char RequestedRegister;
bool sense_enable = 0;
int err_code = 0;
int tmp = 0;
void setup()
{            
//----- LED init -----//  
  pinMode(LED, OUTPUT); 
  pinMode(SD_CS, OUTPUT);  
  
  Serial.begin(9600);
  #ifdef ATTINY85
    TinyWireS.begin(I2C_ADDRESS);
    TinyWireS.onReceive(receiveEvent); //data written from master to slave
    TinyWireS.onRequest(requestEvent); //data requested by the master
  #else
    Wire.begin(I2C_ADDRESS);               
    Wire.onReceive(receiveEvent);
    Wire.onRequest(requestEvent);
  #endif
//  Serial.println("I2C slave is up");
  led_delay = 10;

//----   SD card reader init   -----//
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:

#ifdef SD_CARD
  // see if the card is present and can be initialized:
  if (!SD.begin(SD_CS)) {
    err_code = 15;
  }


  // find a new filename to use  
  while (SD.exists(filename)) {
    filename_idx++;
    sprintf(filename, "log%03d.csv", filename_idx);
  }
  
  File dataFile = SD.open(filename, FILE_WRITE);
  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(csv_header);
    dataFile.close();  
  }
#endif    
   delay(500); 
   for (tmp=0; tmp<10; tmp++) {    
      digitalWrite(LED, HIGH);  
      delay(100); 
      digitalWrite(LED, LOW);  
      delay(100);   
   }

  if (err_code > 0) {
    while (1){
      for (tmp=0; tmp< err_code; tmp++) {  
        digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
        delay(250);           
        digitalWrite(LED, LOW);
        delay(250);           
      } 
      delay(5000);
    }
  }
  Serial.println("I2C slave is up");
} 



void loop()
{
    #ifdef ATTINY85
      TinyWireS_stop_check();  
    #endif
}



// function that executes whenever data is received from master
// this function is registered as an event, see setup()
#ifdef ATTINY85
void receiveEvent(uint8_t howMany)
#else
void receiveEvent(int howMany)
#endif
{
  unsigned long time;
  Serial.print("I2C slave: receiveEvent. howMany=");
  Serial.println(howMany);
  //writeToSD(filename, "receiveEvent");
  //  sprintf(csv_row, "%lu,%d,%d,%d,%lu,%ld,%d,%s", time,x,parachute_open_code, (int)(temp*100),pres,dPa, dPa_running_avg,StateNames[state]);      
  //  writeToSD(filename, csv_row);

  if(howMany == 1)   {
    int command =  receive_i2c_data8();
    command_handler (command);
  }
  else {
    char csv_row[100] = "";
    for (int i=0; i<(howMany/2); i++) {
      int st = receive_i2c_data16();
      sprintf(csv_row, "%s,%d", csv_row,st);
      Serial.println(csv_row);
    }
    time = millis();
    sprintf(csv_row, "%s,%lu",csv_row, time); 
    Serial.print("row:");
    Serial.println(csv_row);
    writeToSD(filename, csv_row);
  }      
}

void requestEvent()
{
  uint16_t data;
  Serial.println("I2C slave: requestEvent");
  //writeToSD(filename, "requestEvent");
  switch(RequestedRegister)
  {
    case 0: {    
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

void command_handler (int command) {  
  writeToSD(filename, "command_handler");
  switch (command) {
    default: {
      break;
    }
  }  
}


// per implementation functions (different between uno and tiny)
void send_i2c_data16(uint16_t data){
  #ifdef ATTINY85
    TinyWireS.send(data & 0xff); // low byte
    TinyWireS.send(data >> 8);   // high byte
  #else
    Wire.write(data & 0xff); // low byte
    Wire.write(data >> 8);   // high byte
  #endif
}
byte receive_i2c_data8() {
  byte result = 0;
  #ifdef ATTINY85
    result = TinyWireS.receive();
  #else
    result = Wire.read();
  #endif
  return result;
}
uint16_t receive_i2c_data16() {
  uint16_t result = 0;
  #ifdef ATTINY85
    result = TinyWireS.receive() << 8 | TinyWireS.receive();
  #else
    result = Wire.read() << 8 | Wire.read();
  #endif
  return result;
}

void writeToSD(char* filename, char* str) {
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
#ifdef SD_CARD  
  File dataFile = SD.open(filename, FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(str);
    dataFile.close();
  }
  // if the file isn't open, pop up an error:
  else {   
  }  
#endif     
}
