#ifndef ROBOTAL_CONSTANTS_H
  #define ROBOTAL_CONSTANTS_H
// CONSTANTS //
const int LOGGER_I2C_ADDRESS = 11;

// PID parameters //
const long V_MAX_MAP_VALUE = 2000000;
//const long TURN_FACTOR = 20000;//59000
const long FACTOR_P = 3700;//650
const long FACTOR_I = 300;//100;//57;120
const long FACTOR_D = 0;//380//2800
const int S_WHITE = 350;


const int MAX_SPEED = 40;
const int MIN_SPEED = 20;

// unsigned long total_error;
// unsigned long mean_error;

//const int BASIC_DELAY = 0;
const int IDLE_DELAY = 0; // 20

// sensor settings
const int L_s1 = 1000;
const int L_s2 = 2000;
const int L_s3 = 3000;
const int L_s4 = 4000;
const int L_s5 = 5000;

// const uint8_t SENSOR_ARRAY[] = {S1 ,S2 ,S3 ,S4 ,S5 };

//color sensor settings
const int RED = 0;
const int GREEN = 1;
const int BLUE = 2;
const int C = 3;
const int CS_WHITE_TH = 500;
const int CS_BLACK_TH = 300;

const unsigned long DEBUG_LOOP_TIME = 20000;

const int SPLIT_ARRAY[15] {5, 9, 10, 11, 13, 17, 18, 19, 20, 22, 23, 25, 26, 27, 29};

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)

#endif