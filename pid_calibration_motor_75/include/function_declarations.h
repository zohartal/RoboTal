
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