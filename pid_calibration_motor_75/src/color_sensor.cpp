//#include <Wire.h>
#ifdef COLOR_SENSOR_CPP
#include "color_sensor.h"
 // const int BLACK = 1500;
  //const int WHITE = 5000;
  //const int YELLOW = 3500;
  //const int GREEN = 1300;
  //const int RED = 1300;



 /* if (tcs.begin()) {
    //Serial.println("Found sensor");
  } else {
    //Serial.println("No TCS34725 found ... check your connections");
    while (1);
  }
  */
  // Now we're ready to get readings!


//void loop(void) {
 // uint16_t r, g, b, c, colorTemp , lux;

  //tcs.getRawData(&r, &g, &b, &c);
 // tcs.getRawData(&tmp_color[0], &tmp_color[1], &tmp_color[2], &tmp_color[3]);
  // colorTemp = tcs.calculateColorTemperature(r, g, b);
// colorTemp = tcs.calculateColorTemperature_dn40(r, g, b, c);
 // lux = tcs.calculateLux(r, g, b);

 /* if(c < BLACK){
    //Serial.println(" BLACK");
    drive_motor(RIGHT,20);
    drive_motor(LEFT,5);
  }
  else if(c > WHITE){
    //Serial.println("WHITE");
    drive_motor(LEFT,20);
    drive_motor(RIGHT,5);
  }
  else if (c > YELLOW){
    stop_motor(RIGHT);
    stop_motor(LEFT);
    //Serial.println("YELLOW");
  }
  else if ((g > GREEN)and (r < RED)){
    //Serial.println("GREEN");
  }

  else if (( r > RED ) and (g < GREEN)){
    //Serial.println("RED");
  }
  else{
    //Serial.println("-1");
  }*/
//}

void color_sensor_calibration(Adafruit_TCS34725 tcs, Adafruit_SSD1306 display){
    char string_for_display[5];
    unsigned long start_time = millis();
    tcs.getRawData(&readed_color[RED], &readed_color[GREEN], &readed_color[BLUE], &readed_color[C]);
    while(millis() - start_time < 5000){
        for(int i = 0; i < 4; i++){
            tcs.getRawData(&readed_color[RED], &readed_color[GREEN], &readed_color[BLUE], &readed_color[C]);
            if((readed_color[i] > color_max[i])or(readed_color[i] < color_min[i])){
                if(readed_color[i] > color_max[i]){
                    color_max[i] = readed_color[i];
                }

                else{
                    color_min[i] = readed_color[i];
                }
            }
        }
    }
    display.clearDisplay();
    display.setCursor(0, 0);

    display.print ("RED: ");
    sprintf(string_for_display,"%05d ",color_min[RED]);
    display.print(string_for_display);
    sprintf(string_for_display,"%03d ",color_max[RED]);
    display.println(string_for_display);
    
    display.print ("GREEN: ");
    sprintf(string_for_display,"%05d ",color_min[GREEN]);
    display.print(string_for_display);
    sprintf(string_for_display,"%05d ",color_max[GREEN]);
    display.println(string_for_display);
    
    display.print ("BLUE: ");
    sprintf(string_for_display,"%05d ",color_min[BLUE]);
    display.print(string_for_display);
    sprintf(string_for_display,"%05d ",color_max[BLUE]);
    display.println(string_for_display);
    
    display.print ("C: ");
    sprintf(string_for_display,"%05d ",color_min[C]);
    display.print(string_for_display);
    sprintf(string_for_display,"%05d ",color_max[C]);
    display.println(string_for_display);
    
    display.display();
}

/*void drive_motor ( motor_t motor_e , int spd) 
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
            spd += 65;
            digitalWrite( MOTOR_A_DIR, LOW ); // direction = forward
            analogWrite( MOTOR_A_PWM, spd ); // PWM speed      
          break;
          
          case BACKWARD:
            motor_speed = 190 - spd;
            digitalWrite( MOTOR_A_DIR, HIGH ); // direction = backward
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
*/
/*
  Serial.print("Color Temp: "); Serial.print(colorTemp, DEC); Serial.print(" K - ");
  Serial.print("Lux: "); Serial.print(lux, DEC); Serial.print(" - ");
  Serial.print("R: "); Serial.print(r, DEC); Serial.print(" ");
  Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
  Serial.print("B: "); Serial.print(b, DEC); Serial.print(" ");
  Serial.print("C: "); Serial.print(c, DEC); Serial.print(" ");
  Serial.println(" ");*/
#endif