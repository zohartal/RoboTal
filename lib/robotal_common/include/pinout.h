#ifndef ROBOTAL_PINOUT_H
  #define ROBOTAL_PINOUT_H


// PIN DEFINITION //
#ifdef ARDUINO_NANO
  const int S1 = A6;
#endif
#ifdef ARDUINO_LEONARDO
  const int S1 = A4;
#endif
const int S2 = A3;
const int S3 = A2;
const int S4 = A1;
const int S5 = A0;

const int MOTOR_A_DIR1 = 4; 
const int MOTOR_A_DIR2 = 6;
const int MOTOR_A_PWM  = 5; 
const int MOTOR_B_DIR1 = 7; 
const int MOTOR_B_DIR2 = 8; 
const int MOTOR_B_PWM  = 11;

const int ENC_A_VAL = 12;
const int ENC_A_INT = 1;
const int ENC_B_VAL = A5;
const int ENC_B_INT = 0;
//MOTOR B IS LEFT
//MOTOR A IS RIGHT

const int BUTTON = MOSI;
const int BUTTON2 = SCK;
//const int TX = SCK;// 15=sck on leonardo/micro
//const int RX = MISO;// 14=miso on leonardo/micro


#endif