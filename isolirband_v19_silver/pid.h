typedef enum dir {F,B} motor_dir_t;
typedef enum motor_e {  LEFT,  RIGHT  } motor_t;
typedef enum motor_direction_e {  FORWARD,  BACKWARD  } motor_direction_t;
int PID (int line_sensor_value, int Perror_old);

const int TURN_FACTOR = 1400;
const int BLACK = 250;
const int BASIC_DELAY = 0;
const int IDLE_DELAY = 0; // 20

//const int BASIC_SPEED = 25;
const int FACTOR_P = 40;
const int FACTOR_I = 1000;//57;
const int FACTOR_D = 30;
