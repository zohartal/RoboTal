typedef enum dir {F,B} motor_dir_t;
typedef enum motor_e {  LEFT,  RIGHT  } motor_t;
typedef enum motor_direction_e {  FORWARD,  BACKWARD  } motor_direction_t;

const int TURN_FACTOR = 500;
const int BLACK = 250;
const int BASIC_DELAY = 0;
const int IDLE_DELAY = 0; // 20

//const int BASIC_SPEED = 25;
const int FACTOR_P = 40;
const int FACTOR_I = 200;
const int FACTOR_D = 80;
