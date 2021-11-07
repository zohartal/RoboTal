#ifndef ROBOTAL_CLI_INTERFACE_H
  #define ROBOTAL_CLI_INTERFACE_H

// const int LOGGER_I2C_ADDRESS = 11;
// typedef enum motor_e {  LEFT,  RIGHT  } motor_t;
//typedef enum motor_direction_e {  FORWARD,  BACKWARD  } motor_direction_t;
// typedef enum m_dir {CW, CCW} motor_real_dir_t;


int color_sensor_read16(int i2c_address, char sensor_address);
void color_sensor_write16(int i2c_address, char sensor_address, int data);
void handle_write( int reg, int data);
void handle_read( int reg);
void write_to_logger (int data_list[], int length);
void blink(long how_long);

void write_to_screen(String text_to_display);

void cli_drive_motor ( motor_t motor_e , int spd);
void drive_motor ( motor_t motor_e , int spd);
void stop_motors ();
void button_read(int button_num);

int motor_speed_map(int spd);
long long_abs(long x);

#endif
