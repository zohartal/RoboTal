//#include "line_sensor.h"
//#include "pid.h"
//#include "calibration.h"
#include "display.h"
#include <SoftwareSerial.h>
//#include "sensor_monitor.c"

//#include <arduino.h>
//#define ARDUINO_MICRO
//#define ARDUINO_NANO
#ifndef ARDUINO_NANO
  #define ARDUINO_LEONARDO
#endif
#ifdef ARDUINO_NANO
  #ifdef ARDUINO_LEONARDO
    ERROR!!!!!
  #endif
#endif
  
#define SERIAL_ON
#define SCREEN
#define MOTOR
#define PRINT_ENC

//const int RX = 7;
//const int TX = 4;
const int TX = SCK;// 15=sck on leonardo/micro
const int RX = MISO;// 14=miso on leonardo/micro

SoftwareSerial mySerial(RX,TX); // RX, TX
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int LED = 13;

const int BATT_PIN = A5;

//int Ierror;
//int Perror_old;
unsigned long curr_time;

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

const int BUTTON = MOSI; //A5;
const int BUTTON2 = 4;
const int BUTTON3 = 7;
#ifdef ARDUINO_NANO
  const int MOTOR_R_ENC_A = 3; // right motor
  const int MOTOR_L_ENC_A = 2; // left motor
#endif
#ifdef ARDUINO_LEONARDO
  const int MOTOR_R_ENC_A = 0; // right motor
  const int MOTOR_L_ENC_A = 1; // left motor
#endif

const int MOTOR_R_ENC_B = 8; // right motor
const int MOTOR_L_ENC_B = 6; // left motor

// functional connections
//const int MOTOR_A_PWM = 9; // left motor Motor B PWM Speed
//const int MOTOR_A_DIR = 10; // left motor Motor B Direction
//const int MOTOR_B_PWM = 11; // right motor Motor B PWM Speed
//const int MOTOR_B_DIR = 12; // right motor Motor B Direction


float test_battary_level();

void motor_r_interrupt();
void motor_l_interrupt();

int motor_l_cnt;
int motor_r_cnt;  

unsigned long current_time = 0;
unsigned long previous_time = 0;
unsigned long time_difference = 0;
unsigned long push_time;
unsigned long time_flag;
int max_speed = 100;

void setup() {

  int i;
  motor_l_cnt = 0;
  motor_r_cnt = 0;

//  Ierror = 0;
  
  pinMode( LED, OUTPUT );

  //pinMode( MOTOR_A_DIR, OUTPUT );
//  pinMode( MOTOR_A_PWM, OUTPUT );
//  pinMode( MOTOR_B_DIR, OUTPUT );
//  pinMode( MOTOR_B_PWM, OUTPUT );

  pinMode( MOTOR_R_ENC_A, INPUT );
  pinMode( MOTOR_R_ENC_B, INPUT );
  pinMode( MOTOR_L_ENC_A, INPUT );
  pinMode( MOTOR_L_ENC_B, INPUT );

  pinMode( BUTTON, INPUT_PULLUP );
  pinMode (BUTTON2, INPUT_PULLUP );
  pinMode (BUTTON3, INPUT_PULLUP );
  
  attachInterrupt(digitalPinToInterrupt(MOTOR_R_ENC_A), motor_r_interrupt, RISING);
  attachInterrupt(digitalPinToInterrupt(MOTOR_L_ENC_A), motor_l_interrupt, RISING);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c)) { 
    digitalWrite(LED,HIGH);
    while(1){
      delay(1);
    }
  }
  
  // set the data rate for the SoftwareSerial port
  //mySerial.begin(9600);
  mySerial.begin(38400);
  mySerial.println("Isolirband 2019: Begin. Battery=");
  float batt = test_battary_level();
  mySerial.println(batt);
  #ifdef SCREEN
    display.clearDisplay();
    display.display();
    display.setTextColor(WHITE);
    display.setCursor(0, 0); 
    display.println("Encoder recorder. ");
    display.print("Battery: ");
    display.println(batt);
    display.display();
  #endif

  while(digitalRead (BUTTON) == HIGH){
    
    if(digitalRead (BUTTON2) == LOW){
      delay(500);
    }

    if(digitalRead (BUTTON3) == LOW){
      delay(500);
    }

    delay(1);
  }
  push_time = millis();
  delay(500);
  motor_l_cnt = 0;
  motor_r_cnt = 0;

  mySerial.println("Button pressed");
      
  time_flag = push_time;
  //sensor_monitor();
  digitalWrite (LED,HIGH);
}

int tmp_i = 0;
void loop() {
//  int line_value = read_line_sensor();
  int line_sensor_value;
  int pid;
  int motor_R_speed = 0;
  int motor_L_speed = 0;
  int split_number = 0;
  unsigned long curr_time = millis();
  unsigned long time_from_start = curr_time - push_time;

  if(digitalRead (BUTTON2 ) == LOW){
    digitalWrite (LED,LOW);
    motor_r_cnt = 0;
    motor_l_cnt = 0;  
  }
  

    mySerial.print("t= ");
    mySerial.print(curr_time);
    mySerial.print(", l= ");
    mySerial.print(motor_l_cnt);
    mySerial.print(", r= ");
    mySerial.println(motor_r_cnt);
    
#ifdef PRINT_ENC
      
  /*  if (tmp_i > 20) {
      display.clearDisplay();
      display.setCursor(0, 0);
      sprintf(string_for_display,"r: %d ",motor_r_cnt);
      display.print(string_for_display);
      sprintf(string_for_display,"l: %d ",motor_l_cnt);
      display.println(string_for_display);
      display.display();  

      tmp_i = 0;
//      time_flag = curr_time;
  }*/
  tmp_i++;
#endif
}

void motor_r_interrupt() {
  int b = digitalRead(MOTOR_R_ENC_B);
  if (b == HIGH) {
    motor_r_cnt++;
  }
  else {
    motor_r_cnt--;
  }
}

void motor_l_interrupt() {
  int b = digitalRead(MOTOR_L_ENC_B);
  if (b == HIGH) {
    motor_l_cnt--;
  }
  else {
    motor_l_cnt++;
  }
}

float test_battary_level() {
  float res;
  int batt_level = analogRead(BATT_PIN);
  res = (float) batt_level * 5 /1024;
  return res;
}
