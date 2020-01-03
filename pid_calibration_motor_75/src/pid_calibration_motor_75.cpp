#ifndef ARDUINO_NANO
  #define ARDUINO_LEONARDO
#endif
#ifdef ARDUINO_NANO
  #ifdef ARDUINO_LEONARDO
    ERROR!!!!!
  #endif
#endif
  
#define MOTOR
#define SERIAL_ON

//#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SoftwareSerial.h>
#include "typedefs.h"
#include "pinout.h"
#include "constants.h"
#include "function_declarations.h"

char string_for_display[4];
char string_for_myserial[10];
int Perror_old;
unsigned long time;
int black_sensors;
//char turn_side[1];
//int max_turn_side = 0;
int final_line_sensor_value;
int S_MIN_ARRAY[] = {1000 ,1000 ,1000 ,1000 ,1000};
int S_MAX_ARRAY[] = {0 ,0 ,0 ,0 ,0};


//const int BASIC_SPEED = 25;


SoftwareSerial mySerial(RX,TX); // RX, TX
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {

  pinMode( LED_BUILTIN, OUTPUT );

  pinMode( MOTOR_A_DIR1, OUTPUT );
  pinMode( MOTOR_A_DIR2, OUTPUT );
  pinMode( MOTOR_A_PWM,  OUTPUT );
  pinMode( MOTOR_B_DIR1, OUTPUT );
  pinMode( MOTOR_B_DIR2, OUTPUT );
  pinMode( MOTOR_B_PWM,  OUTPUT );

  pinMode( BUTTON, INPUT_PULLUP );
  pinMode( BUTTON2, INPUT_PULLUP );

  pinMode( TX, OUTPUT );
  pinMode( RX, INPUT );

  stop_motors();
  //stop_motor(LEFT);

  mySerial.begin(38400);
  mySerial.println("start");

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
  
  digitalWrite(LED_BUILTIN,HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN,LOW);
  delay(500);
  digitalWrite(LED_BUILTIN,HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN,LOW);
  while(digitalRead (BUTTON) == HIGH){
    if(digitalRead (BUTTON2) == LOW){
      delay(500);
      calibration();
    }
  }
  digitalWrite(LED_BUILTIN,LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  int line_sensor_value;
  long pid;
  int motor_R_speed = 0;
  int motor_L_speed = 0;
  int split_number = 0;
  unsigned long time = millis();

  digitalWrite(LED_BUILTIN,LOW);

  line_sensor_value = read_line_sensor();
  if (line_sensor_value != -1){
    line_sensor_value-=2000;
    final_line_sensor_value = line_sensor_value;
    mySerial.print(" line_sensor_value = ");
    mySerial.print(final_line_sensor_value);
    mySerial.print(" time = ");
    //mySerial.println(time);
  }
  else {
    digitalWrite(LED_BUILTIN,HIGH);//if led high there is no line
    if(Perror_old >= 0){
      final_line_sensor_value = 2000;
    }
    else{
      final_line_sensor_value = -2000;
    }
    
  }
  pid = PID(final_line_sensor_value,Perror_old);
  constrain(pid,-V_MAX_MAP_VALUE,V_MAX_MAP_VALUE);
  Perror_old = final_line_sensor_value;

  unsigned long abs_pid = long_abs(pid);
  long v = map(abs_pid,0,V_MAX_MAP_VALUE,MAX_SPEED,0);
  unsigned long vm = abs_pid/TURN_FACTOR;

  v = constrain(v,MIN_SPEED,MAX_SPEED);
  //vm = constrain(vm,0,100);

  //v = 50; // [AT] make v constant, for easier PID settings
  //motor_L_speed = map(pid,-50,50,-50,100);
  //motor_R_speed = map(pid,50,-50,-50,100);

  if(pid > 0){
    motor_L_speed = abs_pid/25000;//v;
    map(motor_L_speed,0,80,MIN_SPEED,MAX_SPEED);
    motor_R_speed = MIN_SPEED;//v - vm;
  }
  else{
    motor_L_speed = MIN_SPEED;
    motor_R_speed = abs_pid/25000;
    map(motor_R_speed,0,80,MIN_SPEED,MAX_SPEED);
  }

#ifdef SERIAL_ON

  
  //time = millis();
  mySerial.print(", v= ");
 // mySerial.print(v);

  sprintf(string_for_myserial,"%ld",pid);
  mySerial.print(", pid= ");
  mySerial.print(string_for_myserial);

 
  sprintf(string_for_myserial,"%ld",abs_pid);
  mySerial.print(", |pid|= ");
  mySerial.print(string_for_myserial);
       
  //mySerial.print(", vm= ");
  mySerial.print(vm);

  mySerial.print (" R = ");
  mySerial.print (motor_R_speed);

  mySerial.print (" L = ");
  mySerial.println (motor_L_speed);
  
#endif

  if (IDLE_DELAY > 0) {
    stop_motors();
    //stop_motor(LEFT);
    delay(IDLE_DELAY);
  }

  drive_motor(RIGHT, motor_R_speed);
  drive_motor(LEFT, motor_L_speed);

}

void calibration(){
    int no_calibration_counter = 0;
    digitalWrite(LED_BUILTIN , HIGH);
    while(no_calibration_counter < 500){
      for(int i = 0; i < 5; i++){      
        int flag = update_min_max (SENSOR_ARRAY[i] , i);       
        if(flag == 0){
          no_calibration_counter = 0;
        }    
        else{
          no_calibration_counter++;
        }  
      
        if (flag == 0) {
        }     
      }
    }
    delay(1000);
    digitalWrite(LED_BUILTIN , LOW);

    display.clearDisplay();
    display.setCursor(0, 0);

    display.print ("s1: ");
    sprintf(string_for_display,"%03d ",S_MIN_ARRAY[0]);
    display.print(string_for_display);
    sprintf(string_for_display,"%03d ",S_MAX_ARRAY[0]);
    display.println(string_for_display);
    
    display.print ("s2: ");
    sprintf(string_for_display,"%03d ",S_MIN_ARRAY[1]);
    display.print(string_for_display);
    sprintf(string_for_display,"%03d ",S_MAX_ARRAY[1]);
    display.println(string_for_display);
    
    display.print ("s3: ");
    sprintf(string_for_display,"%03d ",S_MIN_ARRAY[2]);
    display.print(string_for_display);
    sprintf(string_for_display,"%03d ",S_MAX_ARRAY[2]);
    display.println(string_for_display);
    
    display.print ("s4: ");
    sprintf(string_for_display,"%03d ",S_MIN_ARRAY[3]);
    display.print(string_for_display);
    sprintf(string_for_display,"%03d ",S_MAX_ARRAY[3]);
    display.println(string_for_display);
    
    display.print ("s5: ");
    sprintf(string_for_display,"%03d ",S_MIN_ARRAY[4]);
    display.print(string_for_display);
    sprintf(string_for_display,"%03d ",S_MAX_ARRAY[4]);
    display.println(string_for_display);
    
    display.display();
}

int update_min_max (uint8_t sn, int n){ 
  int s;
  int flag = -1;
    s = analogRead (sn);
    if (s > S_MAX_ARRAY[n]){
      S_MAX_ARRAY[n] = s;
      flag = 0;
    }
    if (s < S_MIN_ARRAY[n]){
      S_MIN_ARRAY[n] = s;
      flag = 0;
    }  
    return flag;
}

long PID (int line_sensor_value, int Perror_old) {
  int Perror = line_sensor_value;
  long Ierror;
  int Derror = (Perror - Perror_old);
  long f;

  //if( abs(Perror) < 1100){
    if( abs (Perror) < 20){
      Ierror = 0;
    }
    else{
      Ierror += Perror;
    }
    Ierror = constrain(Ierror,-2000*FACTOR_P/FACTOR_I,2000*FACTOR_P/FACTOR_I);
  //}
  //  else{
  //    Ierror = 0;
  //  }
  //      max 50,              max 30              max 10
  f = ((long)Perror) * FACTOR_P + ((long)Derror) * FACTOR_D + ((long)Ierror) * FACTOR_I;
  /*mySerial.print(", P = ");
  mySerial.print(Perror);*/
  /*mySerial.print(", I = ");
  mySerial.print(Ierror);
  mySerial.print(", D = ");
  mySerial.print(Derror);
  mySerial.print(", F = ");
  mySerial.print(f);
  f = constrain(f,-50,50);*/
  return f;

}

int read_line_sensor() {

  long p1 = 0;
  long p2 = 0;
  long p3 = 0;
  long p4 = 0;
  long p5 = 0;

  int s1 = 0;
  int s2 = 0;
  int s3 = 0;
  int s4 = 0;
  int s5 = 0;

  int read_sensor;

  int sum = 0;
  long psum = 0;

  int line = 0;

  int s1_value = 0;
  int s2_value = 0;
  int s3_value = 0;
  int s4_value = 0;
  int s5_value = 0;

  s1_value = analogRead(S1);
  s2_value = analogRead(S2);
  s3_value = analogRead(S3);
  s4_value = analogRead(S4);
  s5_value = analogRead(S5);

  s1_value = constrain(s1_value,S_MIN_ARRAY[0],S_MAX_ARRAY[0]);
  s2_value = constrain(s2_value,S_MIN_ARRAY[1],S_MAX_ARRAY[1]);
  s3_value = constrain(s3_value,S_MIN_ARRAY[2],S_MAX_ARRAY[2]); 
  s4_value = constrain(s4_value,S_MIN_ARRAY[3],S_MAX_ARRAY[3]);
  s5_value = constrain(s5_value,S_MIN_ARRAY[4],S_MAX_ARRAY[4]);
/*
  mySerial.print(",s1_value = ");
  mySerial.print(s1_value);
  mySerial.print(",s2_value = ");
  mySerial.print(s2_value);
  mySerial.print(",s3_value = ");
  mySerial.print(s3_value);
  mySerial.print(",s4_value = ");
  mySerial.print(s4_value);
  mySerial.print(",s5_value = ");
  mySerial.println(s5_value);
*/

  s1_value = map(s1_value,S_MIN_ARRAY[0],S_MAX_ARRAY[0],0,1000);
  s2_value = map(s2_value,S_MIN_ARRAY[1],S_MAX_ARRAY[1],0,1000);
  s3_value = map(s3_value,S_MIN_ARRAY[2],S_MAX_ARRAY[2],0,1000);
  s4_value = map(s4_value,S_MIN_ARRAY[3],S_MAX_ARRAY[3],0,1000);
  s5_value = map(s5_value,S_MIN_ARRAY[4],S_MAX_ARRAY[4],0,1000);
  
/*
  mySerial.print("after map: s1_value = ");
  mySerial.print(s1_value);
  mySerial.print(",s2_value = ");
  mySerial.print(s2_value);
  mySerial.print(",s3_value = ");
  mySerial.print(s3_value);
  mySerial.print(",s4_value = ");
  mySerial.print(s4_value);
  mySerial.print(",s5_value = ");
  mySerial.println(s5_value);
*/
  if((s1_value < S_WHITE) and (s2_value < S_WHITE) and (s3_value < S_WHITE) and (s4_value < S_WHITE) and (s5_value < S_WHITE)){
    black_sensors = 0;
    return -1;
  }
  if(s1_value > S_WHITE){
    s1 = 1;
  }

  if(s2_value > S_WHITE){
    s2 = 1;
  }

  if(s3_value > S_WHITE){
    s3 = 1;
  }

  if(s4_value > S_WHITE){
    s4 = 1;
  }

  if(s5_value > S_WHITE){
    s5 = 1;
  }

  read_sensor = (s1 + (s2 * 2) + (s3 * 4) + (s4 * 8) + (s5 * 16));
  black_sensors = read_sensor;

  p1 = (long) s1_value * L_s1;
  p2 = (long) s2_value * L_s2;
  p3 = (long) s3_value * L_s3;
  p4 = (long) s4_value * L_s4;
  p5 = (long) s5_value * L_s5;

  sum = s1_value + s2_value + s3_value + s4_value + s5_value;
//  mySerial.print("sum = ");
//  mySerial.println(sum);
  psum = p1 + p2 + p3 + p4 + p5;
//  mySerial.print("psum = ");
//  mySerial.println(psum);

  line = psum / sum;
  line = line - 1000;
  
  return line;
}

void init_line_sensor() {

  pinMode(S1,INPUT);
  pinMode(S2,INPUT);
  pinMode(S3,INPUT);
  pinMode(S4,INPUT);
  pinMode(S5,INPUT);

}


void stop_motors ( ) {
  digitalWrite( MOTOR_A_PWM, HIGH );
  digitalWrite( MOTOR_A_DIR1, LOW );
  digitalWrite( MOTOR_A_DIR2, LOW );

  digitalWrite( MOTOR_B_PWM, HIGH );
  digitalWrite( MOTOR_B_DIR1, LOW );
  digitalWrite( MOTOR_B_DIR2, LOW );  
}

void drive_motor ( motor_t motor_e , int spd) 
{
  int motor_speed;
  int dir1_pin;
  int dir2_pin;
  int pwm_pin;
  motor_real_dir_t dir = CW;

  spd = constrain(spd,-100,100);
  int speed = abs(spd);
  motor_speed = map(speed,0,100,10,255);
  
  if(motor_e == LEFT){
    dir1_pin = MOTOR_B_DIR1;
    dir2_pin = MOTOR_B_DIR2;
    pwm_pin  = MOTOR_B_PWM;    
    if (spd > 0){
      dir = CCW;
    }
  }
  else {
    dir1_pin = MOTOR_A_DIR1;
    dir2_pin = MOTOR_A_DIR2;
    pwm_pin  = MOTOR_A_PWM;    
    if (spd < 0){
      dir = CCW;
    }
  }    
    
#ifdef MOTOR
//---------------------------------//
// MOTOR_A = RIGHT, MOTOR_B = LEFT //
//---------------------------------//
  if (dir == CW){
     digitalWrite( dir1_pin, HIGH );
     digitalWrite( dir2_pin, LOW );
  }
  else {
     digitalWrite( dir1_pin, LOW );
     digitalWrite( dir2_pin, HIGH );    
  }
  analogWrite( pwm_pin, motor_speed );
#endif
}


long long_abs(long x){
  if(x >= 0){
    return x;
  }
  else{
    return -x;
  }
}
