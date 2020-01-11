
#ifndef COLOR_SENSOR_H
#define COLOR_SENSOR_H


#ifndef ADAFRUIT_SSD1306_H
  #define ADAFRUIT_SSD1306_H
  #include <Adafruit_SSD1306.h>
#endif
#ifndef ADAFRUIT_TCS34725_H
  #define ADAFRUIT_TCS34725_H
  #include <Adafruit_TCS34725.h>
#endif

uint16_t color_min[4];
uint16_t color_max[4];
uint16_t readed_color[4];

const int RED = 0;
const int GREEN = 1;
const int BLUE = 2;
const int C = 3;

void color_sensor_calibration(Adafruit_TCS34725 tcs, Adafruit_SSD1306 display);

#endif