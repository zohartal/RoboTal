#include <Arduino.h>
#include <Wire.h>
#include <robotal_common.h>
// #include "typedefs.h"
#include "cli_interface.h"
// #include "pinout.h"
// #include "constants.h"
#include <Adafruit_GFX.h>

//#include <EEPROM.h>

#ifndef ADAFRUIT_SSD1306_H
  #define ADAFRUIT_SSD1306_H
  #include <Adafruit_SSD1306.h> 
#endif

/*
Command syntax:

<cmd>,<reg>[,<data>]

cmd:
r - read
  Example:
    r,<reg>

w - write
  Example:
    w,<reg>,<data>



*/


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup()
{
  Wire.begin(); // join i2c bus (address optional for master)

  Serial.begin(9600);
  pinMode(LED_BUILTIN,OUTPUT);

  pinMode( MOTOR_A_DIR1, OUTPUT );
  pinMode( MOTOR_A_DIR2, OUTPUT );
  pinMode( MOTOR_A_PWM,  OUTPUT );
  pinMode( MOTOR_B_DIR1, OUTPUT );
  pinMode( MOTOR_B_DIR2, OUTPUT );
  pinMode( MOTOR_B_PWM,  OUTPUT );

  pinMode( BUTTON, INPUT_PULLUP );
  pinMode( BUTTON2, INPUT_PULLUP );


  stop_motors();

  digitalWrite(LED_BUILTIN,HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN,LOW);
  delay(500);
  digitalWrite(LED_BUILTIN,HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN,LOW);

//   attachInterrupt(digitalPinToInterrupt(ENC_A_INT), motor_r_interrupt, RISING);
//   attachInterrupt(digitalPinToInterrupt(ENC_B_INT), motor_l_interrupt, RISING);

  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c)) { 
    digitalWrite(LED_BUILTIN,HIGH);
    while(1){
      delay(1);
    }
  }


  display.clearDisplay();
  display.display();
  display.setTextColor(WHITE);
  display.setCursor(0, 0); 
  display.println("start. ");
  display.display();



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
String parameter1;
String parameter2;
long int reg_n;
long int data_n;
int counter1=0;
int counter2=0;
int counter3=0;

void loop()
{  
  //int data_list[2];
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
        parameter1 = str.substring(ind1+1, ind2);   //captures second data String        
        parameter2 = str.substring(ind2+1);
        data_n = parameter2.toInt();
//i2c interface (add)
        if (cmd.equals("w")) {
          reg_n = parameter1.toInt();
          handle_write((int)reg_n,(int)data_n);
        }
        else if (cmd.equals("r")) {
          reg_n = parameter1.toInt();
          handle_read((int)reg_n);
        }
        else if (cmd.equals("wts")) {
          write_to_screen(parameter1);
        }
        else if (cmd.equals("blink")){
          reg_n = parameter1.toInt();
          blink (reg_n);
        }
        else if (cmd.equals("drive")) {
          int speed = parameter2.toInt();
          if (parameter1.equals("R")){
            cli_drive_motor(RIGHT,speed);
          }
          else if (parameter1.equals("L")){
            cli_drive_motor(LEFT,speed);
          }
          else if(parameter1.equals("RL")){
            cli_drive_motor(RIGHT,speed);
            cli_drive_motor(LEFT,speed);
          }  
        }
        else if (cmd.equals("button_test")){
          reg_n = parameter1.toInt();
          button_read(reg_n);
        }
        else {
          Serial.println("Illegal command");
        }


        //  switch (str) {
        //    case "w":
        //    case "W":
        // if (cmd.equals("l")) {
        //   data_list[0] = (int) reg_n;
        //   data_list[1] = (int) data_n;
        //   write_to_logger (data_list,2) ;
        // }

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
   //color_sensor_write16(4, reg, data);
   Serial.println("handle_write(): done");
}

void handle_read( int reg){
  Serial.print("handle_read(): reg=");
  Serial.print(reg);  
  Serial.println("handle_read(): done");
  //int data = color_sensor_read16(4, reg); 
  //Serial.print(", data=");
  //Serial.println(data);  

}

void blink(long how_long){
  unsigned long time = millis ();
  unsigned long target_time;

  Serial.println("blink: start"); 
  
  how_long = constrain(how_long, 0, 5000);
  target_time = (unsigned long) how_long;
  
  while (millis () < time + (target_time * 1000)) {
    digitalWrite(LED_BUILTIN , HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN , LOW);
    delay(1000);
  }
  digitalWrite(LED_BUILTIN , LOW);
  Serial.println("blink(): done");
}

// void write_to_logger (int data_list[],int length) {
//   // Serial.println("write_to_logger(): start");
//   Wire.beginTransmission(LOGGER_I2C_ADDRESS);  
//   // Serial.println("write_to_logger() 1");
//   for (int i = 0;i<length;i++) {    
//     Serial.print("write_to_logger() low=");
//     byte low_byte = data_list[i] & 0xff;
//     byte high_byte = data_list[i] >> 8;          
//     Serial.print(low_byte);
//     Serial.print(", high=");
//     Serial.println(high_byte);
//     Wire.write(high_byte);
//     Wire.write(low_byte);
//     counter2++;

//   }
//   //int time = millis();
//   Wire.endTransmission();
//   counter1++;
// }


void write_to_screen(String text_to_display) {
    Serial.println(text_to_display);  
    display.clearDisplay();
    display.setCursor(0, 0); 
    display.println(text_to_display);
    display.display();
    Serial.println("write_to_screen(): done");
}

void cli_drive_motor ( motor_t motor_e , int spd){
  Serial.println("drive_motor(): start");
  drive_motor ( motor_e , spd);
  Serial.println("drive_motor(): done");
}
void button_read(int button_num){
  unsigned long time = millis ();
  int button_status;
  while (millis () < time + 10000) {
    if (button_num == 1){
      button_status = digitalRead(BUTTON);
    }
    else if (button_num == 2){
      button_status = digitalRead(BUTTON2);
    }
    Serial.print("button=");
    Serial.print(button_num);
    Serial.print(", status=");
    Serial.println(button_status);
    delay(200);
  }
  Serial.println("button_read(): done");
}