#include "line_sensor.h"
#include "pid.h"
#include "calibration.h"
//#include "display.h"
//#include "color_sensor.h"
#include <SoftwareSerial.h>
#include <Servo.h>


//#include <arduino.h>
//#define ARDUINO_MICRO
#define ARDUINO_NANO
#ifndef ARDUINO_NANO
  #define ARDUINO_LEONARDO
#endif
#ifdef ARDUINO_NANO
  #ifdef ARDUINO_LEONARDO
    ERROR!!!!!
  #endif
#endif
  
#define SERIAL_ON
//#define SCREEN
#define MOTOR
//#define COLOR_SENSOR

//const int RX = 7;
//const int TX = 4;
const int TX = 8;// 15=sck on leonardo/micro
const int RX = 9;// 14=miso on leonardo/micro

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position


SoftwareSerial mySerial(RX,TX); // RX, TX
#ifdef SCREEN
  Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#endif
#ifdef COLOR_SENSOR
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);
#endif

const int LED = 13;
const int SERVO = 5;
const int SIDE_SENSOR = 6;

int Ierror;
int Perror_old;
int no_sensor_counter = 0;
unsigned long time;

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

const uint8_t SENSOR_ARRAY[] = {S1 ,S2 ,S3 ,S4 ,S5 };

//const int LED = 13;

const int BUTTON = 4;
const int BUTTON2 = 7;
#ifdef ARDUINO_NANO
  const int MOTOR_R_ENC_A = 3; // right motor
  const int MOTOR_L_ENC_A = 2; // left motor
#endif
#ifdef ARDUINO_LEONARDO
  const int MOTOR_R_ENC_A = 1; // right motor
  const int MOTOR_L_ENC_A = 0; // left motor
#endif

const int MOTOR_R_ENC_B = 8; // right motor
const int MOTOR_L_ENC_B = 6; // left motor

// functional connections
const int MOTOR_A_PWM = 10; // left motor Motor B PWM Speed
const int MOTOR_A_DIR = 9; // left motor Motor B Direction
const int MOTOR_B_PWM = 11; // right motor Motor B PWM Speed
const int MOTOR_B_DIR = 12; // right motor Motor B Direction

void drive_motor ( motor_t motor_e , int spd);
void stop_motor ( motor_t motor );
void read_color_sensor();
int split();
void drop_egg();

void motor_r_interrupt();
void motor_l_interrupt();

int motor_l_cnt;
int motor_r_cnt;  

unsigned long current_time = 0;
unsigned long previous_time = 0;
unsigned long time_difference = 0;

void setup() {
  
  int i;
  int motor_l_cnt = 0;
  int motor_r_cnt = 0;

  Ierror = 0;
  
  pinMode( LED, OUTPUT );

  pinMode( MOTOR_A_DIR, OUTPUT );
  pinMode( MOTOR_A_PWM, OUTPUT );
  pinMode( MOTOR_B_DIR, OUTPUT );
  pinMode( MOTOR_B_PWM, OUTPUT );

  pinMode( MOTOR_R_ENC_A, INPUT );
  pinMode( MOTOR_R_ENC_B, INPUT );
  pinMode( MOTOR_L_ENC_A, INPUT );
  pinMode( MOTOR_L_ENC_B, INPUT );

  pinMode( BUTTON, INPUT_PULLUP );
  pinMode (BUTTON2, INPUT_PULLUP );

  myservo.attach(SERVO);  // attaches the servo on pin 5 to the servo object
  
  stop_motor(RIGHT); 
  stop_motor(LEFT);
  for (i=0;i<5; i++) {
    digitalWrite(LED,HIGH);
    delay(500);
    digitalWrite(LED,LOW);    
    delay(500);
  }
  digitalWrite(LED,LOW);

//  attachInterrupt(digitalPinToInterrupt(MOTOR_R_ENC_A), motor_r_interrupt, RISING);
//  attachInterrupt(digitalPinToInterrupt(MOTOR_L_ENC_A), motor_l_interrupt, RISING);

  
//  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c)) { 
//    digitalWrite(LED,HIGH);
//    while(1){
//      delay(1);
//    }
//  }
  
  // set the data rate for the SoftwareSerial port
  //mySerial.begin(9600);
  mySerial.begin(38400);
  mySerial.println("Isolirband 2019: Begin");
  #ifdef SCREEN
    display.clearDisplay();
    display.display();
    display.setTextColor(WHITE);
    display.setCursor(0, 0); 
    display.println("start");
    display.display();
  #endif

  #ifdef COLOR_SENSOR
    if (tcs.begin()) {
      display.clearDisplay();
      display.setCursor(0,0);
      display.println("Found sensor");
      display.display();
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1);
  }
  #endif
    
  stop_motor(RIGHT); 
  stop_motor(LEFT);

  init_line_sensor();


//  for(i=0;i<8;i++) {
//    digitalWrite( LED, HIGH );    
//    delay(100);
//    digitalWrite( LED, LOW );
//    delay(100);
//  }
// for (i=-100;i<100;i+=10){
//   drive_motor (RIGHT,i);
// delay (500);
//stop_motor(RIGHT);
//delay (500);
// }
//  drive_motor(RIGHT, -10);
//    delay(2000);
//    stop_motor(RIGHT);
//   drive_motor(LEFT, -10);
//   delay(2000);
//   stop_motor(LEFT);
//    digitalWrite(LED, HIGH);
//    while(1){
//    delay(1);
   // }
   myservo.write(180);
   
  while(digitalRead (BUTTON) == HIGH){
    
    if(digitalRead (BUTTON2) == LOW){
      delay(500);
      calibration();
    }
    
    //mySerial.println("Iso");
    /*if(mySerial.available() > 0){
      char c = mySerial.read();
      if(c == 'l'){
        digitalWrite (LED, LOW);
        mySerial.println("Isolirband 2019: LOW");
      }
      else if(c == 'L'){
        digitalWrite (LED, HIGH);
        mySerial.println("Isolirband 2019: HIGH");
      }
      if(c == 'S'){
        mySerial.println("Isolirband 2019: STOP");
        while(1){
          delay(1);
        }
      }      
    }*/
    delay(1);
  }
  delay(500);

  mySerial.println("Button pressed");
//  digitalWrite( MOTOR_A_DIR, LOW ); // direction = forward
//  for (i=0;i<250;i+=10){
//    
//  
//  analogWrite( MOTOR_A_PWM, i ); // PWM speed
//  delay(500);
//  stop_motor(LEFT);
//  delay(500);
//  }
  drop_zone();
  while(1){
//     read_color_sensor(); 
  
 }

}

void loop() {
//  int line_value = read_line_sensor();
  int line_sensor_value;
  int pid;
  int motor_R_speed = 0;
  int motor_L_speed = 0;

  if(mySerial.available() > 0){
      char c = mySerial.read();
      if (c == 'S'){
        stop_motor(RIGHT);
        stop_motor(LEFT);
        mySerial.print("stopped by user");
        while (1){
          delay(1);
        }
      }
  }
  line_sensor_value = read_line_sensor(); 
  if (line_sensor_value != -1){
    line_sensor_value-=2000;
    int spl = split();
    if(spl == -1){
      final_line_sensor_value = line_sensor_value;
    }

    else{
      final_line_sensor_value = spl - 3000;
    }
    mySerial.print(" spl = ");
    mySerial.print(spl);
    mySerial.print(" final_line_sensor_value = ");
    mySerial.print(final_line_sensor_value);
    mySerial.print(" time = ");
    unsigned long time = millis();
    mySerial.print(time);
    no_sensor_counter = 0;
  }
  else {
    no_sensor_counter ++;
    if(no_sensor_counter >= 100){
      stop_motor(RIGHT);
      stop_motor(LEFT);
//  #ifdef SERIAL_ON
//      mySerial.print("stopped by no_sensor_counter");
//  #endif
      while(1){
        delay(10000);
        #ifdef SERIAL_ON
          mySerial.print("stopped by no_sensor_counter");
        #endif
      }
    }

    if(Perror_old >= 0){
      final_line_sensor_value = 2000;
    }
    else{
      final_line_sensor_value = -2000;
    }
  }
  pid = PID(final_line_sensor_value,Perror_old);
  Perror_old = final_line_sensor_value;

  //motor_L_speed = (50 + pid);
  //motor_R_speed = (50 - pid);

  unsigned long abs_pid = abs(pid);
  int v = map(abs_pid,0,50,50,0); // ZOHAR changed for faster motor
  unsigned long vm = (abs_pid*TURN_FACTOR)/1000;

  v = constrain(v,10,100);
  //vm = constrain(vm,0,100);
  
  //motor_L_speed = map(pid,-50,50,-50,100);
  //motor_R_speed = map(pid,50,-50,-50,100);

  if(pid > 0){
    motor_L_speed = v;
    motor_R_speed = v - vm;
  }
  else{
    motor_L_speed = v - vm;
    motor_R_speed = v;
  }

#ifdef SERIAL_ON

  //time = millis();
  mySerial.print(", v= ");
  mySerial.print(v);
  
  mySerial.print(", |pid|= ");
  mySerial.print((int)abs_pid);
    
  mySerial.print(", vm= ");
  mySerial.println(vm);
  
#endif

  if (IDLE_DELAY > 0) {
    stop_motor(RIGHT);
    stop_motor(LEFT);
    delay(IDLE_DELAY);
  }

  drive_motor(RIGHT, motor_R_speed);
  drive_motor(LEFT, motor_L_speed);
  
  
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
  mySerial.print(" black_sensors = ");
  mySerial.print(read_sensor);

  p1 = (long) s1_value * L_s1;
  p2 = (long) s2_value * L_s2;
  p3 = (long) s3_value * L_s3;
  p4 = (long) s4_value * L_s4;
  p5 = (long) s5_value * L_s5;

  sum = s1_value + s2_value + s3_value + s4_value + s5_value;

  psum = p1 + p2 + p3 + p4 + p5;


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


void read_color_sensor(){
  uint16_t r, g, b, c, colorTemp , lux;

  const int CS_BLACK = 600;
  const int CS_WHITE = 5000;
  const int CS_YELLOW = 3500;
  const int CS_GREEN = 1111;
  const int CS_RED = 1145;
#ifdef COLOR_SENSOR

  tcs.getRawData(&r, &g, &b, &c);
  // colorTemp = tcs.calculateColorTemperature(r, g, b);
  colorTemp = tcs.calculateColorTemperature_dn40(r, g, b, c);
  lux = tcs.calculateLux(r, g, b);

  if(b < CS_BLACK){
      display.clearDisplay();
      display.setCursor(0,0);
      display.println("black");
      display.display();
    drive_motor(RIGHT,20);
    drive_motor(LEFT,5);
    return;
  }
  else if(c > CS_WHITE){
      display.clearDisplay();
      display.setCursor(0,0);
      display.println("white");
      display.display();
    drive_motor(RIGHT,5);
    drive_motor(LEFT,20);
    return;
  }
  else if (c > CS_YELLOW){
      display.clearDisplay();
      display.setCursor(0,0);
      display.println("-1");
      display.display();
    stop_motor(RIGHT);
    stop_motor(LEFT);
    return;
  }
  else if ((g > CS_GREEN)and (r < CS_RED)){
    return;
  }

  else if (( r > CS_RED ) and (g < CS_GREEN)){
    return;
  }
#endif
}

void calibration(){
    int no_calibration_counter = 0;
    digitalWrite(LED , HIGH);
    mySerial.println("calibration");
    while(no_calibration_counter < 500){
      for(int i = 0; i < 5; i++){      
        int flag = update_min_max (SENSOR_ARRAY[i] , i);       
        if(flag == 0){
          no_calibration_counter = 0;
          mySerial.print(i);
          mySerial.println ("calibrated");
        
        }    
        else{
          no_calibration_counter++;
        }  
      
        if (flag == 0) {
        }     
      }
    }
    delay(1000);
    digitalWrite(LED , LOW);
  #ifdef SCREEN
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
#endif
    mySerial.print("calibration done");
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

int split (){
  int split_counter = 0;
 // return -1;
  switch(black_sensors){
    case 5:

   /* case 7:
      if(TURN_SIDE[split_counter] == 'L'){
        return 1000;
      }
      else{
        return 3000;
      }
*/
    case 9:
      if(TURN_SIDE[split_counter] == 'L'){
      return 1000;
      }
      else{
        return 4000;
      }
    case 10:
      if(TURN_SIDE[split_counter] == 'L'){
      return 2000;
      }
      else{
        return 4000;
      }
    case 11:
      if(TURN_SIDE[split_counter] == 'L'){
      return 1000;
      }
      else{
        return 4000;
      }
    case 13:
      if(TURN_SIDE[split_counter] == 'L'){
      return 1000;
      }
      else{
        return 4000;
      }
  /*  case 14:
      if(TURN_SIDE[split_counter] == 'L'){
      return 2000;
      }
      else{
        return 4000;
      }
    case 15:
      if(TURN_SIDE[split_counter] == 'L'){
      return 1000;
      }
      else{
        return 4000;
      }*/
    case 17:
      if(TURN_SIDE[split_counter] == 'L'){
      return 1000;
      }
      else{
        return 5000;
      }
    case 18:
      if(TURN_SIDE[split_counter] == 'L'){
      return 2000;
      }
      else{
        return 5000;
      }
    case 19:
      if(TURN_SIDE[split_counter] == 'L'){
      return 1000;
      }
      else{
        return 5000;
      }
    case 20:
      if(TURN_SIDE[split_counter] == 'L'){
      return 3000;
      }
      else{
        return 5000;
      }
    case 21:
      if(TURN_SIDE[split_counter] == 'L'){
      return 1000;
      }
      else{
        return 5000;
      }
    case 22:
      if(TURN_SIDE[split_counter] == 'L'){
      return 2000;
      }
      else{
        return 5000;
      }
    case 23:
      
    case 25:
      if(TURN_SIDE[split_counter] == 'L'){
      return 1000;
      }
      else{
        return 5000;
      }
    case 26:
      if(TURN_SIDE[split_counter] == 'L'){
      return 2000;
      }
      else{
        return 5000;
      }
    case 27:
      if(TURN_SIDE[split_counter] == 'L'){
      return 1000;
      }
      else{
        return 5000;
      }
    /*case 28:
      if(TURN_SIDE[split_counter] == 'L'){
      return 3000;
      }
      else{
        return 5000;
      }
    case 29:
      if(TURN_SIDE[split_counter] == 'L'){
      return 1000;
      }
      else{
        return 5000;
      }
    case 30:
      if(TURN_SIDE[split_counter] == 'L'){
      return 2000;
      }
      else{
        return 5000;
      }
    case 31:
      if(TURN_SIDE[split_counter] == 'L'){
        return 1000;
      }
      else{
        return 5000;
      }*/
      default:
        return -1;
  }
}


int PID (int line_sensor_value, int Perror_old) {
  int Perror = line_sensor_value;
  int Derror = (Perror - Perror_old);
  int f;

  if( abs(Perror) < 300){
    if( abs (Perror) < 20){
      Ierror = 0;
    }
    else{
      Ierror += Perror;
    }
  }
  else{
    Ierror = 0;
  }
//      max 50,              max 30              max 10
  f = Perror / FACTOR_P + Derror / FACTOR_D + Ierror / FACTOR_I;
  mySerial.print(", P = ");
  mySerial.print(Perror);
  mySerial.print(", I = ");
  mySerial.print(Ierror);
  mySerial.print(", D = ");
  mySerial.print(Derror);
  mySerial.print(", F = ");
  mySerial.print(f);
  f = constrain(f,-50,50);
  return f;

}

void drive_motor ( motor_t motor_e , int spd) 
{
  int motor_speed;
  motor_dir_t dir;

  constrain(spd,-100,100);
  int speed = abs(spd);

  if(spd > 0){
    dir = F;
  }
  else if (spd < 0) {
    dir = B;
  }
  else {
    stop_motor(motor_e);
    return;
  }
#ifdef MOTOR
  if (spd > 190) {
    spd = 190;
  }
  if (spd < 0) {
    spd = 0;
  }
  switch (motor_e) {
      case LEFT:
        switch (dir) {
          case FORWARD:
            //spd += 65;
            motor_speed = 190 - spd;
            digitalWrite( MOTOR_A_DIR, HIGH ); // direction = forward
            analogWrite( MOTOR_A_PWM, motor_speed ); // PWM speed      
          break;
          
          case BACKWARD:
            motor_speed = 190 - spd;
            digitalWrite( MOTOR_A_DIR, LOW ); // direction = backward
            analogWrite( MOTOR_A_PWM, motor_speed ); // PWM speed              
          break;        
        }    
        break;
        
      case RIGHT:
        switch (dir) {
          case FORWARD:
            motor_speed = 190 - spd;
            digitalWrite( MOTOR_B_DIR, HIGH ); // direction = forward
            analogWrite( MOTOR_B_PWM, motor_speed ); // PWM speed
          break;
          
          case BACKWARD:
            spd += 65;
            digitalWrite( MOTOR_B_DIR, LOW ); // direction = backward
            analogWrite( MOTOR_B_PWM, spd ); // PWM speed
          break;        
        }    
        break;  
    }   
#endif    
}


void stop_motor ( motor_t motor ) 
{
switch (motor) {
//---------------------------------//
// MOTOR_A = LEFT, MOTOR_B = RIGHT //
//---------------------------------//
    case RIGHT:
      // MOTOR B STOP
      digitalWrite( MOTOR_B_DIR, HIGH );
      digitalWrite( MOTOR_B_PWM, HIGH );     
      break;
    case LEFT:
      // MOTOR A STOP
      digitalWrite( MOTOR_A_DIR, HIGH );
      digitalWrite( MOTOR_A_PWM, HIGH ); 
      break;
  }   
}

/*
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

*/

void drop_zone() {
   while (digitalRead (SIDE_SENSOR) == LOW) {
     drive_motor (LEFT,30);
     drive_motor (RIGHT, 30);
     delay(10);
     stop_motor(LEFT);
     stop_motor(RIGHT);
   }
    stop_motor(LEFT);
    stop_motor(RIGHT);
    drop_egg();
}


void drop_egg() {
  /*for (pos = 0; pos <= 180; pos += 5) { // goes from 0 degrees to 180 degrees
    // in steps of 5 degree
    myservo.write(90);              // tell servo to go to position in variable 'pos'
    delay(5);                       // waits 15ms for the servo to reach the position
  }*/
  for (pos = 180; pos >= 45; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(10);                       // waits 15ms for the servo to reach the position
  }
delay (250);
myservo.write(180);
}
