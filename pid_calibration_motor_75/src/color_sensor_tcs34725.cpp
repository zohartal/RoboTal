#define COLOR_SENSOR_CPP
#ifdef COLOR_SENSOR_CPP
#include "color_sensor_tcs34725.h"

const int RED = 0;
const int GREEN = 1;
const int BLUE = 2;
const int C = 3;


void color_sensor_calibration(Adafruit_TCS34725 tcs, Adafruit_SSD1306 display, uint16_t *color_min, uint16_t *color_max){
    uint16_t readed_color[4];
    char string_for_display[5];
    unsigned long start_time = millis();
    display.clearDisplay();
    display.display();
    //tcs.getRawData(&readed_color[RED], &readed_color[GREEN], &readed_color[BLUE], &readed_color[C]);
    while(millis() - start_time < 5000){
        tcs.getRawData(&readed_color[RED], &readed_color[GREEN], &readed_color[BLUE], &readed_color[C]);
        for(int i = 0; i < 4; i++){
          //  tcs.getRawData(&readed_color[RED], &readed_color[GREEN], &readed_color[BLUE], &readed_color[C]);
            if((readed_color[i] > color_max[i])or(readed_color[i] < color_min[i])){
                if(readed_color[i] > color_max[i]){
                    color_max[i] = readed_color[i];
                }

                /*else{
                    color_min[i] = readed_color[i];
                }*/
            }
        }
    }
    display.clearDisplay();
    display.setCursor(0, 0);

    display.print ("RED: ");
    sprintf(string_for_display,"%05u ",color_min[RED]);
    display.print(string_for_display);
    sprintf(string_for_display,"%05u ",color_max[RED]);
    display.println(string_for_display);
    
    display.print ("GREEN: ");
    sprintf(string_for_display,"%05u ",color_min[GREEN]);
    display.print(string_for_display);
    sprintf(string_for_display,"%05u ",color_max[GREEN]);
    display.println(string_for_display);
    
    display.print ("BLUE: ");
    sprintf(string_for_display,"%05u ",color_min[BLUE]);
    display.print(string_for_display);
    sprintf(string_for_display,"%05u ",color_max[BLUE]);
    display.println(string_for_display);
    
    display.print ("C: ");
    sprintf(string_for_display,"%05u ",color_min[C]);
    display.print(string_for_display);
    sprintf(string_for_display,"%05u ",color_max[C]);
    display.println(string_for_display);
    
    display.display();
}


void color_display(Adafruit_TCS34725 tcs, Adafruit_SSD1306 display, uint16_t *color_min, uint16_t *color_max){
  uint16_t readed_color[4];
  char string_for_display[6];
  tcs.getRawData(&readed_color[RED], &readed_color[GREEN], &readed_color[BLUE], &readed_color[C]);

  display.clearDisplay();
  display.setCursor(0, 0);

  display.print ("RED: ");
  sprintf(string_for_display,"%05ld ",map( readed_color[RED], color_min[RED], color_max[RED], 0, 1000));
  display.println(string_for_display);
 
  
  display.print ("GREEN: ");
  sprintf(string_for_display,"%05ld ",map( readed_color[GREEN], color_min[GREEN], color_max[GREEN], 0, 1000));
  display.println(string_for_display);
  
  display.print ("BLUE: ");
  sprintf(string_for_display,"%05ld ",map( readed_color[BLUE], color_min[BLUE], color_max[BLUE], 0, 1000));
  display.println(string_for_display);

  display.print ("C: ");
  sprintf(string_for_display,"%05ld ",map( readed_color[C], color_min[C], color_max[C], 0, 1000));
  display.println(string_for_display);
    
  display.display();
}
#endif