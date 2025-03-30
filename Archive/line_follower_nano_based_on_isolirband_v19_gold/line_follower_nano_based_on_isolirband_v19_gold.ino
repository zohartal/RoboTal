#include "line_sensor.h"
#include "pid.h"
#include "calibration.h"
#include "display.h"
// #include <SoftwareSerial.h>
// #include "encoder_playback_arrays.h"
#include "general.h"
//#include "sensor_monitor.c"

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


// GENERAL DEFINES //

#define SCREEN
#define MOTOR
//#define PRINT_ENC  
// #define SERIAL_ON
#define SERIAL_HW
#ifdef SERIAL_HW
    #define SERIAL Serial
#else
    #define SERIAL mySerial
    const int TX = SCK;// 15=sck on leonardo/micro
    const int RX = MISO;// 14=miso on leonardo/micro
    SoftwareSerial mySerial(RX,TX); // RX, TX    
#endif

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


// PINOUT //
// const int BUTTON = MOSI; //A5;
const int BUTTON0 = 4;
const int BUTTON1 = 7;
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
const int MOTOR_A_PWM = 9; // left motor Motor B PWM Speed
const int MOTOR_A_DIR = 10; // left motor Motor B Direction
const int MOTOR_B_PWM = 11; // right motor Motor B PWM Speed
const int MOTOR_B_DIR = 12; // right motor Motor B Direction

const int LED = 13;
const int BATT_PIN = A7;

#ifdef ARDUINO_NANO
  const int S1 = A6;
#endif
#ifdef ARDUINO_LEONARDO
  // const int S1 = A4;
#endif
const int S2 = A3;
const int S3 = A2;
const int S4 = A1;
const int S5 = A0;

const uint8_t SENSOR_ARRAY[] = {S1 ,S2 ,S3 ,S4 ,S5 };

int line_sensor_value;
int global_cnt = 10;
int global_line_val = 0;

void setup() {

  int i;
  // motor_l_cnt = 0;
  // motor_r_cnt = 0;

  // Ierror = 0;
  
  pinMode( LED, OUTPUT );

  pinMode( MOTOR_A_DIR, OUTPUT );
  pinMode( MOTOR_A_PWM, OUTPUT );
  pinMode( MOTOR_B_DIR, OUTPUT );
  pinMode( MOTOR_B_PWM, OUTPUT );

  pinMode( MOTOR_R_ENC_A, INPUT );
  pinMode( MOTOR_R_ENC_B, INPUT );
  pinMode( MOTOR_L_ENC_A, INPUT );
  pinMode( MOTOR_L_ENC_B, INPUT );

  pinMode( BUTTON0, INPUT_PULLUP );
  pinMode (BUTTON1, INPUT_PULLUP );
  
  // attachInterrupt(digitalPinToInterrupt(MOTOR_R_ENC_A), motor_r_interrupt, RISING);
  // attachInterrupt(digitalPinToInterrupt(MOTOR_L_ENC_A), motor_l_interrupt, RISING);
  pinMode(BATT_PIN,INPUT);


  blink_led(150,150,3);

  
  // set the data rate for the SoftwareSerial port
  serial_en = 0;
  #ifdef SERIAL_ON
    SERIAL.begin(9600);
    serial_en = 1;
    SERIAL.println("Begin.");
  #endif

  float batt = test_battery_level();
  SERIAL.print("BAT=");
  SERIAL.print(batt);
  SERIAL.println("%");


  #ifdef SCREEN   
  if (serial_en) SERIAL.println(F("SSD1306 allocation start"));
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
      if (serial_en) SERIAL.println(F("SSD1306 allocation failed"));
  // if(!display.begin()) {     
    while(1){
      delay(1000);
      blink_led(50,50,20);
    }
  }
  #endif

  //mySerial.begin(38400);
  //mySerial.println("Isolirband 2019: Begin. Battery=");
 
  //mySerial.println(batt);
  #ifdef SCREEN
    if (serial_en) SERIAL.println("Starting OLED display.");

    display.clearDisplay();
    display.display();
    display.setTextColor(WHITE);
    display.setCursor(0, 0); 
    display.println(F("start. "));
    display.print("Battery: ");
    display.println(batt);
    display.println(F("v2025"));
    display.display();
    if (serial_en) SERIAL.println("Starting OLED display DONE.");
  #endif
 
  stop_motor(RIGHT); 
  stop_motor(LEFT);

  init_line_sensor();

  delay(500);
  motor_l_cnt = 0;
  motor_r_cnt = 0;
//  stage_6();
//  while(1){
//    digitalWrite(LED,HIGH);
//  }
  
  //mySerial.println("Button pressed");
  //encoder_follower(300,0,50);
  
  //mySerial.println(turn_side[0]);
  // time_flag = push_time;
 // stage = T_5_STAGE_4;
 // encoder_follower (300,400,40);
 // delay(1000);
 // encoder_follower (500,200,35);
  //while(1){
  //}

}




void loop() {
  if (serial_en==1){
    #ifdef SERIAL_ON
    serial_cli();
    #endif
  }
  main_menu();
  // line_follower();
}


int parseCommand(const char *input, ParsedCommand *parsedCommand) {
    // Validate input
    if (input == NULL || parsedCommand == NULL) {
        return -1; // Error: null pointers
    }
    if (input[0] != UART_COMMAND_CHAR) {
        return -2; // Error: does not start with 'c'
    }

    // Initialize parsedCommand
    parsedCommand->pairCount = 0;

    // Pointer for parsing
    const char *ptr = input + 1; // Skip the 'c'
    while (*ptr != '\0' && *ptr != '\n' && parsedCommand->pairCount < MAX_PAIRS) {
        // Extract key
        char key = *ptr;
        if (!isalpha(key)) { // Ensure the key is a valid character
            Serial.print("error -3. char=");
            Serial.println(*ptr);
            return -3; // Error: invalid key
        }
        ptr++;

        // Extract integer value
        char buffer[16] = {0}; // Temporary buffer for the integer part
        int bufIndex = 0;
        while (isdigit(*ptr) && bufIndex < sizeof(buffer) - 1) {
            buffer[bufIndex++] = *ptr++;
        }

        if (bufIndex == 0) { // No digits found after key
            return -4; // Error: missing integer value
        }
        int value = atoi(buffer);

        // Add the parsed pair to the list
        parsedCommand->pairs[parsedCommand->pairCount].key = key;
        parsedCommand->pairs[parsedCommand->pairCount].value = value;
        parsedCommand->pairCount++;

        // Skip the comma, if present
        if (*ptr == ',') {
            ptr++;
        } else if ((*ptr != '\0') && (*ptr != '\n'))  {
            Serial.print("error -5. char=");
            Serial.println(*ptr);
            return -5; // Error: unexpected character
        }
    }

    if ((*ptr != '\0') && (*ptr != '\n'))  { // If there is leftover data in the buffer
        return -6; // Error: command too long or improperly terminated
    }

    return 0; // Success
}

void serial_cli() {
    int l_result = 0;
    // Read data from Serial when available
    while (SERIAL.available() > 0) {
        char incomingChar = SERIAL.read();
        // Add the character to the command buffer if there's space
        if (bufferIndex < MAX_COMMAND_SIZE - 1) {
            commandBuffer[bufferIndex++] = incomingChar;
        }

        // Null-terminate the string
        commandBuffer[bufferIndex] = '\0';

        // Check for command termination (newline)
        if (incomingChar == '\n') {
            if (commandBuffer[0] == UART_COMMAND_CHAR) {
                ParsedCommand parsed_cmd;

                // Parse the command
                l_result = parseCommand(commandBuffer, &parsed_cmd);
                if (l_result == 0) {
                    exec_uart_cmd(parsed_cmd);
                } else {
                    // Print error message
                    SERIAL.print("Error parsing command. Code: ");
                    SERIAL.println(l_result);
                }
            } else {
                SERIAL.println("Unknown command. Command string must start with colon ':' (UART_COMMAND_CHAR)");
            }
            // Reset the buffer for the next command
            bufferIndex = 0;
            memset(commandBuffer, 0, MAX_COMMAND_SIZE);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////
void exec_uart_cmd(ParsedCommand cmd_l) {
    int ie=0;
    CommandPair cur_cmd;
    SERIAL.println("exec_uart_cmd. Start. NUM_COMMANDs=");
    SERIAL.println(NUM_COMMANDS);
    for (int j=0; j<cmd_l.pairCount; j++) {
        cur_cmd = cmd_l.pairs[j];
        for (ie = 0; ie < NUM_COMMANDS; ie++) { // parse all commands forthe one requested by the user
          // SERIAL.print("cli_cmd_l[i].key=");
          // SERIAL.println(cli_cmd_l[i].key);
          if (cli_cmd_l[ie].key == cur_cmd.key) {
              SERIAL.println("matched!");
              cli_cmd_l[ie].function(cur_cmd.value); // execute the selected command with the provided value
          }
        }
    }
    SERIAL.println("exec_uart_cmd. End");
}

void led_cli_access(int value) {
  SERIAL.print("led_cli_access. val=");
  SERIAL.println(value);
  if (value == 0){
    digitalWrite(LED, LOW);
  } else {
    digitalWrite(LED, HIGH);  
  }
}
void right_motor_cli_access(int value){
  int mapme;
  mapme = map(value,0,255,-100,100);
  SERIAL.print("right_motor_cli_access. val=");
  SERIAL.print(value);
  SERIAL.print(", mapped=");
  SERIAL.println(mapme);
  drive_motor(RIGHT,mapme);
  delay(1000);
  stop_motor(RIGHT);
}
void left_motor_cli_access(int value){
  int mapme;
  mapme = map(value,0,255,-100,100);  
  SERIAL.print("left_motor_cli_access. val=");
  SERIAL.print(value);
  SERIAL.print(", mapped=");
  SERIAL.println(mapme);
  drive_motor(LEFT,mapme);
  delay(1000);
  stop_motor(LEFT);  
}
void sensor_cli_access(int value){
  int s;
  s = analogRead(SENSOR_ARRAY[value]);
  SERIAL.print("sensor_cli_access. S[");
  SERIAL.print(value);
  SERIAL.print("]=");
  SERIAL.println(s);  
}
void sensor_line_cli_access(int value){ //TODO FIXME
  int s;

  for (int i=0;i<value;i++) {
      s = read_line_sensor();
      SERIAL.print(i);
      SERIAL.print("- sensor_cli_access. line=");
      SERIAL.println(s);
      delay(100);
  } 
}

void calibration_cli_access(int value) {
  SERIAL.println("calibration_cli_access.");
  calibration();
}

void button_cli_access(int value) {
  int b0;
  int b1;
  for (int i=0;i<value;i++) {      
      b0 = digitalRead (BUTTON0);
      b1 = digitalRead (BUTTON1);
      SERIAL.print(i);
      SERIAL.print("- button_cli_access. BUTTON0=");
      SERIAL.print(b0);
      SERIAL.print(". BUTTON1=");
      SERIAL.print(b1);    
      SERIAL.println(".");
      delay(100);
  } 
}

// Function to print help screen
void help_cli_access(int value) {
    SERIAL.println("\nAvailable Commands:\n");
    for (int i = 0; i < NUM_COMMANDS; i++) {
        SERIAL.print(cli_cmd_l[i].key);
        SERIAL.print(" - ");
        SERIAL.print(cli_cmd_l[i].description);
        SERIAL.println(".");
        
    }
}

void main_menu() {   
    if(digitalRead (BUTTON0) == LOW){
      prog = update_prog_num(prog);
      #ifdef SCREEN
      display.clearDisplay();
      display.display();
      display.setCursor(0, 0); 
      display.println(prog_num_to_str(prog));
      display.println(prog);
      display.display();
      #endif
      if (serial_en) SERIAL.println(prog_num_to_str(prog));
      delay(500);
    }  

    if(digitalRead (BUTTON1) == LOW){
      switch(prog){
          case CALIBRATION :            
            calibration();
            break;
          case LINE_FOLLOWER :
            line_follower_loop();            
            stop_motor(LEFT);
            stop_motor(RIGHT);
            break;
          case ENC_TR_1 :
              blink_led(50,50,20);
            break;     
        }
    }  
    delay(1);
}

void line_follower_loop(){
  unsigned long update_time;
  unsigned long time = millis();
  update_time = millis();
  #ifdef SCREEN
    display.clearDisplay();
    display.display();
    display.setTextColor(WHITE);
    display.setCursor(0, 0);     
    display.println("line_follower_loop");
    display.display(); 
    delay(1000);
  #endif 
    while (digitalRead (BUTTON0) == HIGH){
        line_follower();
        time = millis();
        if (time - update_time > 10000) {
          update_display();
          update_time = time;
        }
    } 
}

void line_follower(){
  int i;
  int pid;
  int motor_R_speed = 0;
  int motor_L_speed = 0;
  int split_number = 0;

  line_sensor_value = read_line_sensor(); 
  if (line_sensor_value == -512){    
    if(Perror_old >= 0){
      line_sensor_value = 500;
    }
    else{
      line_sensor_value = -500;
    }
  }
  
  pid = PID(line_sensor_value,Perror_old);
  Perror_old = line_sensor_value;

  //motor_L_speed = (50 + pid);
  //motor_R_speed = (50 - pid);

  unsigned long abs_pid = abs(pid);
  int v = map(abs_pid,0,50,MAX_SPEED,0);
  unsigned long vm = (abs_pid*TURN_FACTOR)/1000;

  v = constrain(v,20,100);
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
  #ifdef SCREEN
    display.clearDisplay(); 
    display.display(); 
    display.setCursor(0, 0);     
    display.print("ln=");
    display.print(line_sensor_value);
    display.print(", PID=");
    display.println(pid);
    display.print("R=");
    display.print(motor_R_speed);
    display.print(", L=");
    display.println(motor_L_speed);            
    display.display(); 
  #endif 
#ifdef SERIAL_ON

  //time = millis();
  SERIAL.print(", line= ");
  SERIAL.print(line_sensor_value);  
  SERIAL.print(", v= ");
  SERIAL.print(v);
  
  SERIAL.print(", |pid|= ");
  SERIAL.print((int)abs_pid);
    
  SERIAL.print(", vm= ");
  SERIAL.print(vm);
  SERIAL.print(", R=");
  SERIAL.print(motor_R_speed);
  SERIAL.print(", L=");
  SERIAL.println(motor_L_speed);

#endif

  if (IDLE_DELAY > 0) {
    stop_motor(RIGHT);
    stop_motor(LEFT);
    delay(IDLE_DELAY);
  }

  drive_motor(RIGHT, motor_R_speed);
  drive_motor(LEFT, motor_L_speed);

// #ifdef PRINT_ENC
//   if (time - time_flag > 1000) {
//       display.clearDisplay();
//       display.setCursor(0, 0);
//       sprintf(string_for_display,"r: %d ",motor_r_cnt);
//       display.print(string_for_display);
//       sprintf(string_for_display,"l: %d ",motor_l_cnt);
//       display.println(string_for_display);
//       display.display();  
  
//       time_flag = time;
//   }
// #endif
  return;
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

  s1_value = map(s1_value,S_MIN_ARRAY[0],S_MAX_ARRAY[0],1000,0);
  s2_value = map(s2_value,S_MIN_ARRAY[1],S_MAX_ARRAY[1],1000,0);
  s3_value = map(s3_value,S_MIN_ARRAY[2],S_MAX_ARRAY[2],1000,0);
  s4_value = map(s4_value,S_MIN_ARRAY[3],S_MAX_ARRAY[3],1000,0);
  s5_value = map(s5_value,S_MIN_ARRAY[4],S_MAX_ARRAY[4],1000,0);  

  // SERIAL.print("after map: s1_value = ");
  // SERIAL.print(s1_value);
  // SERIAL.print(",s2_value = ");
  // SERIAL.print(s2_value);
  // SERIAL.print(",s3_value = ");
  // SERIAL.print(s3_value);
  // SERIAL.print(",s4_value = ");
  // SERIAL.print(s4_value);
  // SERIAL.print(",s5_value = ");
  // SERIAL.print(s5_value);
  // SERIAL.print(". ");

  p1 = (long) s1_value * L_s1;
  p2 = (long) s2_value * L_s2;
  p3 = (long) s3_value * L_s3;
  p4 = (long) s4_value * L_s4;
  p5 = (long) s5_value * L_s5;

  sum = s1_value + s2_value + s3_value + s4_value + s5_value;

  // no line detected
  if (sum < NO_DETECT_TH) {
    return -512;
  }


  psum = p1 + p2 + p3 + p4 + p5;


  line = psum / sum;
  global_line_val = line;
  line = (line - 3000)/4; //range=[-500,500]
  line = constrain(line,-500,500);
  return line;
}

void init_line_sensor() {

  pinMode(S1,INPUT);
  pinMode(S2,INPUT);
  pinMode(S3,INPUT);
  pinMode(S4,INPUT);
  pinMode(S5,INPUT);

}


void calibration(){
    int no_calibration_counter = 0;
    digitalWrite(LED , HIGH);
    #ifdef SCREEN
    display.clearDisplay();
    display.setCursor(0, 0);    
    display.println("CALIBRATION"); 
    display.display();
    #endif

    while(no_calibration_counter < 500){
      for(int i = 0; i < 5; i++){      
        int flag = update_min_max (SENSOR_ARRAY[i] , i);       
        if(flag == 0){
          no_calibration_counter = 0;
        }    
        else{
          no_calibration_counter++;
        }      
      }
      delay(10);
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
#ifdef SERIAL_ON
    SERIAL.println("calibration done");
    for (int i; i<5;i++){
       SERIAL.print("S[");
       SERIAL.print(i);
       SERIAL.print("]:[");
       SERIAL.print(S_MIN_ARRAY[i]);
       SERIAL.print("-");
       SERIAL.print(S_MAX_ARRAY[i]);
       SERIAL.println("]");
    } 
#endif
    //mySerial.print("calibration done");
    return;
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

prog_number_t update_prog_num(prog_number_t prog){
  if(prog == PNAME_MAX){
    return 0;
  }
  else{
    return  ((int)prog)+1 ;
  }
}

char* prog_num_to_str(prog_number_t prog) {
  switch(prog){
    case 0 :    return "CALIBRATION";
    case 1 :    return "LINE_FOLLOWER";
    case 2 :    return "ENCODER_P1";
  }
}


int PID (int line_sensor_value, int Perror_old) {
  int Perror = line_sensor_value;
  int Derror = (Perror - Perror_old);
  int f=0;

  if( abs(Ierror) < MAX_IERROR_VAL){
    if( abs (Perror) < 20){
      Ierror = 0;
    }
    else{
      Ierror += Perror;
    }
  }
  else {
    Ierror = 0; // reset the I value
  }
//      max 50,              max 30              max 10
  f = Perror / FACTOR_P + Derror / FACTOR_D + Ierror / FACTOR_I;
  SERIAL.print(", P = ");
  SERIAL.print(Perror);
  SERIAL.print(", I = ");
  SERIAL.print(Ierror);
  SERIAL.print(", D = ");
  SERIAL.print(Derror);
  SERIAL.print(", F = ");
  SERIAL.print(f);
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
//---------------------------------//
// MOTOR_A = LEFT, MOTOR_B = RIGHT //
//---------------------------------//
  if(dir == B){
    if(motor_e == RIGHT){
       motor_speed = map(speed,0,100,50,255);
       #ifdef SERIAL_ON 
        //  SERIAL.print(", ");
        //  SERIAL.println("r-b");
       #endif
       digitalWrite( MOTOR_B_DIR, LOW );
       analogWrite( MOTOR_B_PWM, motor_speed );
    }
    else{
      motor_speed = map(speed,0,100,50,255);;
      #ifdef SERIAL_ON 
        // SERIAL.print(", ");
        // SERIAL.println("l-b");
      #endif
      digitalWrite( MOTOR_A_DIR, LOW );
      analogWrite( MOTOR_A_PWM, motor_speed );
      #ifdef SERIAL_ON
       SERIAL.println(motor_speed);
      #endif 
    }
  }
  else{
    if(motor_e == RIGHT){
      motor_speed = map(speed,0,100,200,0);;
      #ifdef SERIAL_ON 
        // SERIAL.print(", ");
        // SERIAL.println("r-f");
      #endif
      digitalWrite( MOTOR_B_DIR, HIGH ); 
      analogWrite( MOTOR_B_PWM, motor_speed ); 
    }
    else{
     motor_speed = map(speed,0,100,200,0);;
      #ifdef SERIAL_ON 
        // SERIAL.print(", ");
        // SERIAL.println("l-f");
      #endif
      digitalWrite( MOTOR_A_DIR, HIGH );
      analogWrite( MOTOR_A_PWM, motor_speed );
    }
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


// void motor_r_interrupt() {
//   int b = digitalRead(MOTOR_R_ENC_B);
//   if (b == HIGH) {
//     motor_r_cnt++;
//   }
//   else {
//     motor_r_cnt--;
//   }
// }

// void motor_l_interrupt() {
//   int b = digitalRead(MOTOR_L_ENC_B);
//   if (b == HIGH) {
//     motor_l_cnt--;
//   }
//   else {
//     motor_l_cnt++;
//   }
// }


float test_battery_level() {
  float res;
  int batt_level = analogRead(BATT_PIN);
  SERIAL.println(batt_level);

  res = ((float)batt_level/820)*100; //820 is equivilent to 4v/5v (input max value is 4v while the analog in measures up to 5v)
  SERIAL.println(res);

  return res;
}

// void stop_all() {
//   stop_motor(LEFT);
//   stop_motor(RIGHT);
//   while (1) {
//   }
// }


void update_display() {
  #ifdef SCREEN
    display.clearDisplay();
    display.display();
    display.setTextColor(WHITE);
    display.setCursor(0, 3);     
    display.print("Battery: ");
    display.println(test_battery_level());    
    display.display();  
  #endif
  if (serial_en) {
    SERIAL.print("BAT=");
    SERIAL.println(test_battery_level());
  };
}


void blink_led (int t_low, int t_high, int repeat) {
  int i;
  for (i=0;i<repeat;i++){
    digitalWrite (LED, LOW);
    delay(t_low);
    digitalWrite (LED, HIGH);
    delay(t_high);
  }
}