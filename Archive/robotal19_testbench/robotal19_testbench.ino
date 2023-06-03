#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)


const int LED = 13;

const int BUTTON = 4;
const int BUTTON2 = 7;

// functional connections
const int MOTOR_A_PWM = 9;//12; // left motor Motor B PWM Speed
const int MOTOR_A_DIR = 10;//11; // left motor Motor B Direction
const int MOTOR_B_PWM = 5; // right motor Motor B PWM Speed
const int MOTOR_B_DIR = 6; // right motor Motor B Direction


const int S1 = A6;
const int S2 = A3;
const int S3 = A2;
const int S4 = A1;
const int S5 = A0;


// #define SCREEN
#ifdef SCREEN
  Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#endif


typedef enum motor_e {  LEFT,  RIGHT  } motor_t;
void stop_motor ( motor_t motor );
void read_sensor();

void setup() {
  
  int i;
  int motor_l_cnt = 0;
  int motor_r_cnt = 0;

  pinMode(S1,INPUT);
  pinMode(S2,INPUT);
  pinMode(S3,INPUT);
  pinMode(S4,INPUT);
  pinMode(S5,INPUT);
  
  pinMode( LED, OUTPUT );

  pinMode( MOTOR_A_DIR, OUTPUT );
  pinMode( MOTOR_A_PWM, OUTPUT );
  pinMode( MOTOR_B_DIR, OUTPUT );
  pinMode( MOTOR_B_PWM, OUTPUT );

  pinMode( BUTTON, INPUT );
  pinMode (BUTTON2, INPUT );

  Serial.begin(9600);  
  Serial.println("RoboTal 2019: Begin");

#ifdef SCREEN
 if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c)) { 
   digitalWrite(LED,HIGH);
   Serial.println("RoboTal 2019: Display unavailable");
  //  while(1){
  //    delay(1);
  //  }
 }  
 else {
   Serial.println("RoboTal 2019: Display started");
 }

    display.clearDisplay();
    display.display();
    display.setTextColor(WHITE);
    display.setCursor(0, 0); 
    display.println("start");
    display.display();
#endif
}

void loop() {    
    if(digitalRead (BUTTON2) == LOW){
      Serial.println("Button2 pressed<<>>");
      delay(500);

    }
    if(digitalRead (BUTTON) == LOW){
      Serial.println("Button1 pressed<<>>");
      delay(500);

    }    
    //mySerial.println("Iso");
    if(Serial.available() > 0){
      Serial.println("RoboTal 2019: <<>>");
      char c = Serial.read();
      if(c == 'd'){
        digitalWrite (LED, LOW);
        Serial.println("RoboTal 2019: LOW");
      }
      else if(c == 'D'){
        digitalWrite (LED, HIGH);
        Serial.println("RoboTal 2019: HIGH");
      }
      // else if(c == 's'){
      //   Serial.println("Isolirband 2019: Read Sensor");
      //   i =read_line_sensor();
      // }

      else if(c == 'r'){
        Serial.println("RoboTal 2019: B motor forward");
        digitalWrite( MOTOR_B_DIR, HIGH ); // direction = forward
        analogWrite( MOTOR_B_PWM, 128 ); // PWM speed          
        delay(2000);
        stop_motor(RIGHT); 
        stop_motor(LEFT);   
      }      
      else if(c == 'R'){
        Serial.println("RoboTal 2019: B motor backward");
        digitalWrite( MOTOR_B_DIR, LOW ); // direction = forward
        analogWrite( MOTOR_B_PWM, 128 ); // PWM speed          
        delay(2000);
        stop_motor(RIGHT); 
        stop_motor(LEFT);           
      }      
      else if(c == 'l'){
        Serial.println("RoboTal 2019: A motor forward");        

        digitalWrite( MOTOR_A_DIR, HIGH ); // direction = forward
        analogWrite( MOTOR_A_PWM, 128 ); // PWM speed                                       
        delay(2000);
        stop_motor(RIGHT); 
        stop_motor(LEFT);           
      }      
      else if(c == 'L'){
        Serial.println("RoboTal 2019: A motor backward");
        digitalWrite( MOTOR_A_DIR, LOW ); // direction = backward
        analogWrite( MOTOR_A_PWM, 128 ); // PWM speed              
        delay(2000);
        stop_motor(RIGHT); 
        stop_motor(LEFT);           
      }      

      else if(c == 'L'){
        Serial.println("RoboTal 2019: A motor backward");
        digitalWrite( MOTOR_A_DIR, LOW ); // direction = backward
        analogWrite( MOTOR_A_PWM, 128 ); // PWM speed              
        delay(2000);
        stop_motor(RIGHT); 
        stop_motor(LEFT);           
      }    
      
      else if(c == 's'){
        Serial.println("RoboTal 2019: read sensors");
        read_sensor();
      }  
      
    
    }
  delay(100);

}


void read_sensor() {
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


  Serial.print("after map: s1_value = ");
  Serial.print(s1_value);
  Serial.print(",s2_value = ");
  Serial.print(s2_value);
  Serial.print(",s3_value = ");
  Serial.print(s3_value);
  Serial.print(",s4_value = ");
  Serial.print(s4_value);
  Serial.print(",s5_value = ");
  Serial.println(s5_value);  
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
