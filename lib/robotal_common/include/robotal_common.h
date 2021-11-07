#include "typedefs.h"
#include "pinout.h"
#include "constants.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h> 

void show_loop_time_results(Adafruit_SSD1306 display);
void automatic_calibration(Adafruit_SSD1306 display);
int update_min_max (uint8_t sn, int n);

void drive_motor ( motor_t motor_e , int spd);
void stop_motors ();
