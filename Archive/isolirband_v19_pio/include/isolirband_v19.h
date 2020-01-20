// Board definition
//#define ROBOTAL_V18  // this define means we are using the 2018 robot, with 5 sensors setup (AKA ref_1)
#define ROBOTAL_V19  // this define means we are using the 2019 robot, as run on the contest


#ifdef ROBOTAL_V18
  #ifdef ROBOTAL_V19
    ERROR!!!!!
  #endif
#endif

//------------------------------------------------------------------------------------------------------
// Pinout
const int LED = 13;
const int BATT_PIN = A5;
#ifdef ROBOTAL_V18
  const int S1 = A6;
#endif
#ifdef ROBOTAL_V19
  const int S1 = A4;
#endif
const int S2 = A3;
const int S3 = A2;
const int S4 = A1;
const int S5 = A0;

const int TX = SCK;// 15=sck on leonardo/micro
const int RX = MISO;// 14=miso on leonardo/micro

const int BUTTON = MOSI; //A5;
const int BUTTON2 = 4;
const int BUTTON3 = 7;
#ifdef ROBOTAL_V18
  const int MOTOR_R_ENC_A = 3; // right motor
  const int MOTOR_L_ENC_A = 2; // left motor
#endif
#ifdef ROBOTAL_V19
  const int MOTOR_R_ENC_A = 0; // right motor
  const int MOTOR_L_ENC_A = 1; // left motor
#endif

const int MOTOR_R_ENC_B = 8; // right motor
const int MOTOR_L_ENC_B = 6; // left motor

// functional connections
const int MOTOR_A_PWM = 9; // left motor Motor B PWM Speed
const int MOTOR_A_DIR = 10; // left motor Motor B Direction
const int MOTOR_B_PWM = 11; // right motor Motor B PWM Speed
const int MOTOR_B_DIR = 12; // right motor Motor B Direction

//------------------------------------------------------------------------------------------------------
// Global variables and arrays

const uint8_t SENSOR_ARRAY[] = {S1 ,S2 ,S3 ,S4 ,S5 };

const char TRACK_1_L[] = {'R'};
const char TRACK_2_L[] = {'L'};

const char TRACK_3_TEAM_A_TYPE_1_L[] = {'R'};
const char TRACK_3_TEAM_A_TYPE_2_L[] = {'L','R','R','R'};
const char TRACK_3_TEAM_B_TYPE_1_L[] = {'L','L'};
const char TRACK_3_TEAM_B_TYPE_2_L[] = {'L','R','L','L','L'};

const char TRACK_4_L[] = {'L','L','R','R','L','L'};
const char TRACK_5_L[] = {'L','R'};

//const String TRKN_STR_L[] = {"TRACK_1", "TRACK_2", "TRACK_3_TEAM_A_TYPE_1", "TRACK_3_TEAM_A_TYPE_2", "TRACK_3_TEAM_A_TYPE_3", "TRACK_3_TEAM_B_TYPE_1", "TRACK_3_TEAM_B_TYPE_2", "TRACK_3_TEAM_B_TYPE_3", "TRACK_4", "TRACK_5" };
typedef enum trkn { TRACK_1, TRACK_2, TRACK_3_TEAM_A_TYPE_1, TRACK_3_TEAM_A_TYPE_2, TRACK_3_TEAM_B_TYPE_1, TRACK_3_TEAM_B_TYPE_2, TRACK_4, TRACK_5 } track_number_t;
const int TRKN_MAX = 7;
track_number_t track = TRACK_5;
typedef enum stg { T_5_STAGE_1, T_5_STAGE_2, T_5_STAGE_3, T_5_STAGE_4, T_5_STAGE_5, T_5_STAGE_6, T_5_STAGE_7, T_5_STAGE_8, T_5_STAGE_9 } stage_number_t;
stage_number_t stage = T_5_STAGE_1;


//------------------------------------------------------------------------------------------------------
//function definitions

float test_battary_level();

void drive_motor ( motor_t motor_e , int spd);
void stop_motor ( motor_t motor );
int split();
track_number_t update_track(track_number_t);
char* track_num_to_str(track_number_t track);
void turn_handler(int);
void handler();

void motor_r_interrupt();
void motor_l_interrupt();

void line_folower();
void encoder_playback(int* left_array, int* right_array,int max_index);

void encoder_follower (int target_r_cnt, int target_l_cnt, int basic_speed);
bool stop_condition (stage_number_t curernt_stage);
void stage_6 ();
