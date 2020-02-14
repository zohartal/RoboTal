
void init_line_sensor();
void calibration ();
int update_min_max (uint8_t sn, int n);
long PID (int line_sensor_value, int Perror_old);
int read_line_sensor();
void drive_motor ( motor_t motor_e , int spd);
void stop_motors ();
long long_abs(long x);
int motor_speed_map(int spd);
void show_loop_time_results();
void automatic_calibration();
String menu_num_to_str(menu_number_t item);
menu_number_t update_menu_item (menu_number_t item_number);
void line_follower();
void write_to_logger(int d1, int d2, int d3);