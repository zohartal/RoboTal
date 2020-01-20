//typedef enum dir {F,B} motor_dir_t;
typedef enum motor_e {  LEFT,  RIGHT  } motor_t;
//typedef enum motor_direction_e {  FORWARD,  BACKWARD  } motor_direction_t;
typedef enum m_dir {CW, CCW} motor_real_dir_t;
// typedef enum trkn { AUTO_CAL, COLOR_CAL, DEBUG_TRACK, TRACK_1/*, TRACK_2, TRACK_3_TEAM_A_TYPE_1, TRACK_3_TEAM_A_TYPE_2, TRACK_3_TEAM_B_TYPE_1, TRACK_3_TEAM_B_TYPE_2, TRACK_4, TRACK_5*/ } menu_number_t;
typedef enum trkn { AUTO_CAL, COLOR_CAL, DEBUG_TRACK, TRACK_1 } menu_number_t;
const int MENU_MAX = 3;
menu_number_t item_number = (menu_number_t) MENU_MAX;