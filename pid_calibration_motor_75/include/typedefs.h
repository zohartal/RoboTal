//typedef enum dir {F,B} motor_dir_t;
typedef enum motor_e {  LEFT,  RIGHT  } motor_t;
//typedef enum motor_direction_e {  FORWARD,  BACKWARD  } motor_direction_t;
typedef enum m_dir {CW, CCW} motor_real_dir_t;
typedef enum trkn { AUTO_CAL, COLOR_CAL, DEBUG_TRACK_N, DEBUG_TRACK_L, DEBUG_TRACK_R, EEPROM_DEBUG_TRACK, EEPROM_2_SERIAL, TRACK_1 } menu_number_t;
const int MENU_MAX = 8;
menu_number_t item_number = (menu_number_t) MENU_MAX;
typedef enum ts {R, L, N} turn_side_t;
