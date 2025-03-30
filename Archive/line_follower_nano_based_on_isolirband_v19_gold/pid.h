typedef enum dir {F,B} motor_dir_t;
typedef enum motor_e {  LEFT,  RIGHT  } motor_t;
typedef enum motor_direction_e {  FORWARD,  BACKWARD  } motor_direction_t;
int PID (int line_sensor_value, int Perror_old);

const int MAX_SPEED = 50;
const int TURN_FACTOR = 400;
// const int BLACK = 250;
// const int BASIC_DELAY = 0;
const int IDLE_DELAY = 20; // 20

//const int BASIC_SPEED = 25;
const int FACTOR_P = 10;
const int FACTOR_I = 2000;//57;
const int FACTOR_D = 100;

const int MAX_IERROR_VAL = 5000;