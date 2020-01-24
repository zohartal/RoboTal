#include <Arduino.h>
#include <Wire.h>
#include "cs01_register_space.h"
#include "i2c_master_main.h"

void setup()
{
  Wire.begin(); // join i2c bus (address optional for master)

  Serial.begin(9600);
  pinMode(LED_BUILTIN,OUTPUT);
  Serial.println("I2C master is up");
}

  


byte x = 0;
int v;
int led = 0;
String str = "";
char b;

int ind1;
int ind2;
String cmd;
String reg;
String data;
long int reg_n;
long int data_n;
int counter1=0;
int counter2=0;
int counter3=0;

void loop()
{  
  if ((Serial.available() > 0) && (b != '\n')) {
    // get incoming byte:
    b = Serial.read();
    str += b;    
    Serial.write(b);
    digitalWrite(LED_BUILTIN,led);
    led = (led==0) ? 1:0;
    //Serial.println(str);
  }
  
   if (b == '\n') {
         str.trim();         
        //  Serial.println(str);           
         
        // parse command
        ind1 = str.indexOf(',');  //finds location of first ,
        cmd = str.substring(0, ind1); 
        ind2 = str.indexOf(',', ind1+1 );   //finds location of second ,
        reg = str.substring(ind1+1, ind2);   //captures second data String        
        data = str.substring(ind2+1);
        reg_n = reg.toInt();
        data_n = data.toInt();

        // Serial.print("ind1=");
        // Serial.print(ind1);
        // Serial.print("| ind2=");
        // Serial.print(ind2);
        // Serial.print("| cmd=");
        // Serial.print(cmd);
        // Serial.print("| reg=");
        // Serial.print(reg);
        // Serial.print("| data=");
        // Serial.println(data);
        // Serial.print("| reg=");
        // Serial.print(reg_n);
        // Serial.print("| data=");
        // Serial.println(data_n);
        if (cmd.equals("w")) {
          handle_write((int)reg_n,(int)data_n);
        }
        if (cmd.equals("r")) {
          handle_read((int)reg_n);
        }
        //  switch (str) {
        //    case "w":
        //    case "W":
        if (cmd.equals("l")) {
          write_to_logger ( (int) reg_n, (int) data_n) 
        }

         str = "";
         b = 0;         
         while (Serial.available() == 0) {
         }
   }
}

int color_sensor_read16(int i2c_address, char sensor_address){
  int result = 0;
  int b0;
  int b1;
  Wire.beginTransmission(i2c_address);    
  Wire.write(sensor_address);  
  Wire.endTransmission();
  Wire.requestFrom(i2c_address, 2); // request 1 byte in return  
  b0 = Wire.read(); // receive a byte
  b1 = Wire.read(); // receive a byte  
  result = (b1<<8) + b0;  
  return result;
}

void color_sensor_write16(int i2c_address, char sensor_address, int data){
  byte low_byte = data & 0xff;
  byte high_byte = data >> 8;  
  Wire.beginTransmission(i2c_address);  
  Wire.write(sensor_address);
  Wire.write(high_byte);
  Wire.write(low_byte);
  Wire.endTransmission();
}

void handle_write( int reg, int data){
   Serial.print("handle_write(): reg=");
   Serial.print(reg);
   Serial.print(", data=");
   Serial.println(data);
   color_sensor_write16(4, reg, data);
   Serial.println("handle_write(): done");
}

void handle_read( int reg){
  Serial.print("handle_read(): reg=");
  Serial.print(reg);  
  int data = color_sensor_read16(4, reg); 
  Serial.print(", data=");
  Serial.println(data);  

}

void write_to_logger (int da0,int da1) {
  int data[5] = {da0,da1,0,counter1,counter2};
  Wire.beginTransmission(11);  
  for (int i = 0;i<5;i++) {
    byte low_byte = data[i] & 0xff;
    byte high_byte = data[i] >> 8;          
    Wire.write(high_byte);
    Wire.write(low_byte);
    counter2++;
  }
  Wire.endTransmission();
  counter1++;
}