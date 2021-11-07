#include <Arduino.h>
#include <SoftwareSerial.h>
#include "typedefs.h"
#include "pinout.h"
#include "constants.h"
#include "robotal_common.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h> 

#define MOTOR



uint16_t color_min[4] = {0, 0, 0, 0};
uint16_t color_max[4] = {0, 0, 0, 0};
uint16_t readed_color[4];



//void color_display();
void colored_line_follow(int line_color);
//void color_sensor_calibration(Adafruit_TCS34725 tcs, Adafruit_SSD1306 display);

int motor_l_cnt = 0;
int motor_r_cnt = 0;


char string_for_display[4];
char string_for_5c[5];
char string_for_myserial[10];
int Perror_old;
unsigned long time;
int black_sensors;
//char turn_side[1];
//int max_turn_side = 0;
int final_line_sensor_value;
int S_MIN_ARRAY[] = {1000 ,1000 ,1000 ,1000 ,1000};
int S_MAX_ARRAY[] = {0 ,0 ,0 ,0 ,0};
int SENSOR_ARRAY[5];

//const int BASIC_SPEED = 25;
unsigned long loop_start_time;
int loop_num = 0;
int mean_error = 0;
int total_error = 0;
turn_side_t turn_side = R;
track_number_t track;



void show_loop_time_results (Adafruit_SSD1306 display) {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println ("loop_num = ");
  sprintf(string_for_5c,"%05d",loop_num);
  display.println(string_for_5c);

  mean_error = total_error / loop_num;
  display.println ("mean_error = ");
  sprintf(string_for_5c,"%05d",mean_error);
  display.println(string_for_5c);

  display.display();
  stop_motors();
  while(digitalRead (BUTTON2) == HIGH){
    delay(1);
  }
  delay(500);
}



void automatic_calibration(Adafruit_SSD1306 display){
  digitalWrite(LED_BUILTIN, HIGH);
  unsigned long calibration_time = millis();
  unsigned long time_diff = millis()-calibration_time;
  while(time_diff < 3000){
      for(int i = 0; i < 5; i++){  
        time_diff = millis()-calibration_time;    
        int flag = update_min_max (SENSOR_ARRAY[i] , i);    
        if(i == 4){
          drive_motor(LEFT,15);
          drive_motor(RIGHT,13);
          delay(30);
          stop_motors();
          delay(50);
        }  
      }
    }

    digitalWrite(LED_BUILTIN, LOW);

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

// void calibration(){
    // int no_calibration_counter = 0;
    // digitalWrite(LED_BUILTIN , HIGH);
    // while(no_calibration_counter < 500){
    //   for(int i = 0; i < 5; i++){      
    //     int flag = update_min_max (SENSOR_ARRAY[i] , i);       
    //     if(flag == 0){
    //       no_calibration_counter = 0;
    //     }    
    //     else{
    //       no_calibration_counter++;
    //     }  
      
    //     if (flag == 0) {
    //     }     
    //   }
    // }
    // delay(1000);
    // digitalWrite(LED_BUILTIN , LOW);

    // display.clearDisplay();
    // display.setCursor(0, 0);

    // display.print ("s1: ");
    // sprintf(string_for_display,"%03d ",S_MIN_ARRAY[0]);
    // display.print(string_for_display);
    // sprintf(string_for_display,"%03d ",S_MAX_ARRAY[0]);
    // display.println(string_for_display);
    
    // display.print ("s2: ");
    // sprintf(string_for_display,"%03d ",S_MIN_ARRAY[1]);
    // display.print(string_for_display);
    // sprintf(string_for_display,"%03d ",S_MAX_ARRAY[1]);
    // display.println(string_for_display);
    
    // display.print ("s3: ");
    // sprintf(string_for_display,"%03d ",S_MIN_ARRAY[2]);
    // display.print(string_for_display);
    // sprintf(string_for_display,"%03d ",S_MAX_ARRAY[2]);
    // display.println(string_for_display);
    
    // display.print ("s4: ");
    // sprintf(string_for_display,"%03d ",S_MIN_ARRAY[3]);
    // display.print(string_for_display);
    // sprintf(string_for_display,"%03d ",S_MAX_ARRAY[3]);
    // display.println(string_for_display);
    
    // display.print ("s5: ");
    // sprintf(string_for_display,"%03d ",S_MIN_ARRAY[4]);
    // display.print(string_for_display);
    // sprintf(string_for_display,"%03d ",S_MAX_ARRAY[4]);
    // display.println(string_for_display);
    
    // display.display();
// }

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


// long PID (int line_sensor_value, int Perror_old) {
//   int Perror = line_sensor_value;
//   long Ierror;
//   int Derror = (Perror - Perror_old);
//   long f;

//   //if( abs(Perror) < 1100){
//     if( abs (Perror) < 100){
//       Ierror = 0;
//     }
//     else{
//       Ierror += Perror;
//     }
//     Ierror = constrain(Ierror,-V_MAX_MAP_VALUE,V_MAX_MAP_VALUE);
//   //}
//   //  else{
//   //    Ierror = 0;
//   //  }
//   //      max 50,              max 30              max 10
//   f = ((long)Perror) * FACTOR_P + ((long)Derror) * FACTOR_D + ((long)Ierror) * FACTOR_I;
//   return f;

// }

// int read_line_sensor() {

//   long p1 = 0;
//   long p2 = 0;
//   long p3 = 0;
//   long p4 = 0;
//   long p5 = 0;

//   int s1 = 0;
//   int s2 = 0;
//   int s3 = 0;
//   int s4 = 0;
//   int s5 = 0;

//   int read_sensor;

//   int sum = 0;
//   long psum = 0;

//   int line = 0;

//   int s1_value = 0;
//   int s2_value = 0;
//   int s3_value = 0;
//   int s4_value = 0;
//   int s5_value = 0;

//   s1_value = analogRead(S1);
//   s2_value = analogRead(S2);
//   s3_value = analogRead(S3);
//   s4_value = analogRead(S4);
//   s5_value = analogRead(S5);

//   s1_value = constrain(s1_value,S_MIN_ARRAY[0],S_MAX_ARRAY[0]);
//   s2_value = constrain(s2_value,S_MIN_ARRAY[1],S_MAX_ARRAY[1]);
//   s3_value = constrain(s3_value,S_MIN_ARRAY[2],S_MAX_ARRAY[2]); 
//   s4_value = constrain(s4_value,S_MIN_ARRAY[3],S_MAX_ARRAY[3]);
//   s5_value = constrain(s5_value,S_MIN_ARRAY[4],S_MAX_ARRAY[4]);
// /*
//   mySerial.print(",s1_value = ");
//   mySerial.print(s1_value);
//   mySerial.print(",s2_value = ");
//   mySerial.print(s2_value);
//   mySerial.print(",s3_value = ");
//   mySerial.print(s3_value);
//   mySerial.print(",s4_value = ");
//   mySerial.print(s4_value);
//   mySerial.print(",s5_value = ");
//   mySerial.println(s5_value);
// */

//   s1_value = map(s1_value,S_MIN_ARRAY[0],S_MAX_ARRAY[0],1000,0);
//   s2_value = map(s2_value,S_MIN_ARRAY[1],S_MAX_ARRAY[1],1000,0);
//   s3_value = map(s3_value,S_MIN_ARRAY[2],S_MAX_ARRAY[2],1000,0);
//   s4_value = map(s4_value,S_MIN_ARRAY[3],S_MAX_ARRAY[3],1000,0);
//   s5_value = map(s5_value,S_MIN_ARRAY[4],S_MAX_ARRAY[4],1000,0);
  
// /*
//   mySerial.print("after map: s1_value = ");
//   mySerial.print(s1_value);
//   mySerial.print(",s2_value = ");
//   mySerial.print(s2_value);
//   mySerial.print(",s3_value = ");
//   mySerial.print(s3_value);
//   mySerial.print(",s4_value = ");
//   mySerial.print(s4_value);
//   mySerial.print(",s5_value = ");
//   mySerial.println(s5_value);
// */
//   digitalWrite(LED_BUILTIN, LOW);

//   if(s1_value > S_WHITE){
//     s1 = 1;
//   }

//   if(s2_value > S_WHITE){
//     s2 = 1;
//   }

//   if(s3_value > S_WHITE){
//     s3 = 1;
//   }

//   if(s4_value > S_WHITE){
//     s4 = 1;
//   }

//   if(s5_value > S_WHITE){
//     s5 = 1;
//   }

//   bool flag = 0;
//   int black_sensors_counter = s1 +s2 +s3 +s4 +s5;

//   if(black_sensors_counter == 0){
//     return -1;
//   }
//   read_sensor = (s1 + (s2 * 2) + (s3 * 4) + (s4 * 8) + (s5 * 16));
//   black_sensors = read_sensor;

//     // display.clearDisplay();
//     // display.setCursor(0, 0);

//     // display.print ("s1: ");
//     // sprintf(string_for_display,"%03d ",s1_value);
//     // display.print(string_for_display);
    
//     // display.print ("s2: ");
//     // sprintf(string_for_display,"%03d ",s2_value);
//     // display.print(string_for_display);
    
//     // display.print ("s3: ");
//     // sprintf(string_for_display,"%03d ",s3_value);
//     // display.print(string_for_display);
    
//     // display.print ("s4: ");
//     // sprintf(string_for_display,"%03d ",s4_value);
//     // display.print(string_for_display);
    
//     // display.print ("s5: ");
//     // sprintf(string_for_display,"%03d ",s5_value);
//     // display.print(string_for_display);
    
//     // display.display();

//   for(int i = 0; i < 15; i++){
//     if(SPLIT_ARRAY[i] == black_sensors){
//       flag = 1;
//       break;
//     }
//   }

//   if(flag == 1){
//     if(turn_side != N){
//       if(turn_side == R){
//         s1_value = 0;
//         s2_value = 0;
//         switch(black_sensors){
//           case 23:
//             digitalWrite(LED_BUILTIN, HIGH);
//             s3_value = 0;
//             break;
          
//           case 22:
//             digitalWrite(LED_BUILTIN, HIGH);
//             s3_value = 0;
//             break;

//           case 20:
//             digitalWrite(LED_BUILTIN, HIGH);
//             s3_value = 0;
//             break;
//         }
//       }
//       else{
//         s5_value = 0;
//         s4_value = 0;
//         switch(black_sensors){
//           case 13:
//             digitalWrite(LED_BUILTIN, HIGH);
//             s3_value = 0;
//             break;
          
//           case 29:
//             digitalWrite(LED_BUILTIN, HIGH);
//             s3_value = 0;
//             break;

//           case 5:
//             digitalWrite(LED_BUILTIN, HIGH);
//             s3_value = 0;
//             break;
//         }
//       }
//     /* switch(black_sensors){
//         case 5:
//           if(turn_side == L){

//           }
//           else{

//           }

//         case 9:
//           if(turn_side == L){

//           }
//           else{

//           }

//         case 10:
//           if(turn_side == L){

//           }
//           else{

//           }

//         case 11:
//           if(turn_side == L){

//           }
//           else{

//           }

//         case 13:
//           if(turn_side == L){

//           }
//           else{

//           }

//         case 15:
//           if(turn_side == L){

//           }
//           else{

//           }

//         case 17:
//           if(turn_side == L){

//           }
//           else{

//           }

//         case 18:
//           if(turn_side == L){

//           }
//           else{

//           }

//         case 19:
//           if(turn_side == L){

//           }
//           else{

//           }

//         case 20:
//           if(turn_side == L){

//           }
//           else{

//           }

//         case 21:
//           if(turn_side == L){

//           }
//           else{

//           }

//         case 22:
//           if(turn_side == L){

//           }
//           else{

//           }

//         case 23:
//           if(turn_side == L){

//           }
//           else{

//           }

//         case 25:
//           if(turn_side == L){

//           }
//           else{

//           }

//         case 26:
//           if(turn_side == L){

//           }
//           else{

//           }

//         case 27:
//           if(turn_side == L){

//           }
//           else{

//           }

//         case 29:
//           if(turn_side == L){

//           }
//           else{

//           }

//         case 30:
//           if(turn_side == L){

//           }
//           else{

//           }

//         case 31:
//           if(turn_side == L){

//           }
//           else{

//           }

//       }*/
//     }
//   }
//     p1 = (long) s1_value * L_s1;
//     p2 = (long) s2_value * L_s2;
//     p3 = (long) s3_value * L_s3;
//     p4 = (long) s4_value * L_s4;
//     p5 = (long) s5_value * L_s5;

//   sum = s1_value + s2_value + s3_value + s4_value + s5_value;
// //  mySerial.print("sum = ");
// //  mySerial.println(sum);
//   psum = p1 + p2 + p3 + p4 + p5;
// //  mySerial.print("psum = ");
// //  mySerial.println(psum);

//   line = psum / sum;
//   line = line - 1000;
  
//   return line;
// }

// void split_handler (){
//   digitalWrite(LED_BUILTIN, HIGH);
//   drive_motor(RIGHT,50);
//   drive_motor(LEFT,50);
//   delay(80);
//   if(turn_side == R){
//     stop_motors();
//     drive_motor(RIGHT,50);
//   }
//   else{
//     stop_motors();
//     drive_motor(LEFT,50);
//   }
//   delay(60);
//   stop_motors();
//   // write_to_logger(-1,0,0,0,0);
//   digitalWrite(LED_BUILTIN, LOW);
// }

// void init_line_sensor() {

//   pinMode(S1,INPUT);
//   pinMode(S2,INPUT);
//   pinMode(S3,INPUT);
//   pinMode(S4,INPUT);
//   pinMode(S5,INPUT);

// }

int motor_speed_map(int spd){
  spd = map(spd,-100,100, -MAX_SPEED, MAX_SPEED);
  if((spd>-MIN_SPEED)and(spd<0)){
    spd = -MIN_SPEED;
  }
  if((spd<MIN_SPEED)and(spd>0)){
    spd = MIN_SPEED;
  }
  //if()
  return spd;
}

void stop_motors ( ) {
  digitalWrite( MOTOR_A_PWM, LOW );
  digitalWrite( MOTOR_A_DIR1, LOW );
  digitalWrite( MOTOR_A_DIR2, LOW );

  digitalWrite( MOTOR_B_PWM, LOW );
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



// void line_follower(){
//   //uint16_t readed_color[4];
//   int line_sensor_value;
//   long pid;
//   int motor_R_speed = 0;
//   int motor_L_speed = 0;
//   // int spl;
//   bool flag = 0;
//   //int split_number = 0;
  

//   digitalWrite(LED_BUILTIN,LOW);

//   line_sensor_value = read_line_sensor();
//   for(int i = 0; i < 15; i++){
//     if(SPLIT_ARRAY[i] == black_sensors){
//       flag = 1;
//       break;
//     }
//   }
  
//   // if((turn_side == N) or (flag == 0)){
//     if (line_sensor_value != -1){ // line detected
//       line_sensor_value-=2000;
//     /* spl = split();
//       if(spl == -1){ // no split
//         final_line_sensor_value = line_sensor_value;
//       }
//       else { // split detected
//         if(spl == -2){ // + shaped split
//             drive_motor(RIGHT,40);
//             drive_motor(LEFT,40);
//             delay(150);
//             stop_motors();
//         }
//         final_line_sensor_value = spl - 3000;
//       }*/
//       final_line_sensor_value = line_sensor_value;
//     }   
//     else{ //line not detected
//       digitalWrite(LED_BUILTIN,HIGH);//if led high there is no line
//       if(Perror_old >= 0){ //line lost right side
//         final_line_sensor_value = 2000;
//       }
//       else{ //line lost left side
//         final_line_sensor_value = -2000;
//       }
      
//     }

//     total_error += abs(final_line_sensor_value);

//     pid = PID(final_line_sensor_value,Perror_old);
//     pid = constrain(pid,-V_MAX_MAP_VALUE,V_MAX_MAP_VALUE);
//     Perror_old = final_line_sensor_value;

//     //unsigned long abs_pid = long_abs(pid);

//     if(pid > 0){
//       motor_L_speed = map(pid, 0, V_MAX_MAP_VALUE, MAX_SPEED, MIN_SPEED);
//       motor_R_speed = map(pid, 0, V_MAX_MAP_VALUE, MAX_SPEED, -MIN_SPEED);
//     }
//     else{
//       motor_R_speed = map(pid, 0, -V_MAX_MAP_VALUE, MAX_SPEED, MIN_SPEED);
//       motor_L_speed = map(pid, 0, -V_MAX_MAP_VALUE, MAX_SPEED, -MIN_SPEED);
//     }
//   // }
//   // else{
//   //   split_handler();
//   // }

// #ifdef SERIAL_ON

  
//   //time = millis();
//   mySerial.print(", v= ");
//  // mySerial.print(v);

//   sprintf(string_for_myserial,"%ld",pid);
//   mySerial.print(", pid= ");
//   mySerial.print(string_for_myserial);

 
//   sprintf(string_for_myserial,"%ld",abs_pid);
//   mySerial.print(", |pid|= ");
//   mySerial.print(string_for_myserial);
       
//   //mySerial.print(", vm= ");
//   mySerial.print(vm);

//   mySerial.print (" R = ");
//   mySerial.print (motor_R_speed);

//   mySerial.print (" L = ");
//   mySerial.println (motor_L_speed);
  
// #endif

//   /*tcs.getRawData(&readed_color[RED], &readed_color[GREEN], &readed_color[BLUE], &readed_color[C]);
//   if(((map( readed_color[BLUE], color_min[BLUE], color_max[BLUE], 0, 1000) > CS_BLACK_TH) and (map( readed_color[BLUE], color_min[BLUE], color_max[BLUE], 0, 1000) < CS_WHITE_TH))and(map( readed_color[RED], color_min[RED], color_max[RED], 0, 1000) > 500)){
//     stop_motors();
//     while (1){
//       delay(1);
//     }
    
//   }
// */

//   // write_to_logger(black_sensors, final_line_sensor_value, pid, motor_R_speed, motor_L_speed);

//   drive_motor(RIGHT, motor_R_speed);
//   drive_motor(LEFT, motor_L_speed);

//   loop_num++;
// }

// void write_to_logger(int d1, int d2, int d3, int d4, int d5) {
//   byte low_byte;
//   byte high_byte;
//   Wire.beginTransmission(LOGGER_I2C_ADDRESS);  
  
//   low_byte  = d1 & 0xff;
//   high_byte = d1 >> 8;          
//   Wire.write(high_byte);
//   Wire.write(low_byte);
  
//   low_byte  = d2 & 0xff;
//   high_byte = d2 >> 8;          
//   Wire.write(high_byte);
//   Wire.write(low_byte);

//   low_byte  = d3 & 0xff;
//   high_byte = d3 >> 8;          
//   Wire.write(high_byte);
//   Wire.write(low_byte);

//   low_byte  = d4 & 0xff;
//   high_byte = d4 >> 8;          
//   Wire.write(high_byte);
//   Wire.write(low_byte);
  
//   low_byte  = d5 & 0xff;
//   high_byte = d5 >> 8;          
//   Wire.write(high_byte);
//   Wire.write(low_byte);

//   Wire.endTransmission();
// }
// void motor_r_interrupt() {
//   int b = digitalRead(ENC_A_VAL);
//   if (b == HIGH) {
//     motor_r_cnt++;
//   }
//   else {
//     motor_r_cnt--;
//   }
// }

// void motor_l_interrupt() {
//   int b = digitalRead(ENC_B_VAL);
//   if (b == HIGH) {
//     motor_l_cnt++;
//   }
//   else {
//     motor_l_cnt--;
//   }
// }