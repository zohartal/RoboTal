
#ifndef COLOR_SENSOR_H
#define COLOR_SENSOR_H

#include <Adafruit_SSD1306.h>
#include <Adafruit_TCS34725.h>

void color_sensor_calibration(Adafruit_TCS34725 tcs, Adafruit_SSD1306 display, uint16_t *color_min, uint16_t *color_max);
void color_display(Adafruit_TCS34725 tcs, Adafruit_SSD1306 display, uint16_t *color_min, uint16_t *color_max);
#endif