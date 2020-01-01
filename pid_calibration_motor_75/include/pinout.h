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

const int MOTOR_A_DIR1 = 1; 
const int MOTOR_A_DIR2 = 12;
const int MOTOR_A_PWM  = 11; 
const int MOTOR_B_DIR1 = 0; 
const int MOTOR_B_DIR2 = 10; 
const int MOTOR_B_PWM  = 9;

const int BUTTON = MOSI;
const int BUTTON2 = 4;
const int TX = SCK;// 15=sck on leonardo/micro
const int RX = MISO;// 14=miso on leonardo/micro
