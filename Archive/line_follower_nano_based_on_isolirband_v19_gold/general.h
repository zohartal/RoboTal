
// UART command parameters
const long int BAUD_RATE =38400;
const int MAX_COMMAND_SIZE =64;
const int MAX_PAIRS =10;
const int UART_COMMAND_CHAR = ':';

typedef struct {
    char key;
    int value;
} CommandPair;

typedef struct {
    CommandPair pairs[MAX_PAIRS];
    int pairCount;
} ParsedCommand;

// Struct for key-function mapping
typedef struct {
    char key;
    void (*function)(char value);
    const char* description;
} CommandEntry;

void led_cli_access(int value);
void right_motor_cli_access(int value);
void left_motor_cli_access(int value);
void sensor_cli_access(int value);
void calibration_cli_access(int value);
void sensor_line_cli_access(int value);
void help_cli_access(int value);

// Command table
CommandEntry cli_cmd_l[] = {
    {'L', led_cli_access, "LED on/off [0/1]"},
    {'r', right_motor_cli_access, "Right motor speed [0-255]"},
    {'l', left_motor_cli_access, "Left motor speed [0-255]"},
    {'c', calibration_cli_access, "Sensor calibration [0]"},
    {'s', sensor_cli_access, "Sensor read [0-4]"},
    {'S', sensor_line_cli_access, "Sensor array read (repeat n times [1-100]"},
    {'h', help_cli_access, "Prnt this [0]"}
};

// Number of commands
#define NUM_COMMANDS (sizeof(cli_cmd_l) / sizeof(cli_cmd_l[0]))

// -- END UART -- //


typedef enum p_name { CALIBRATION, LINE_FOLLOWER, ENC_TR_1 } prog_number_t;

// FUNCTIONS //
void serial_cli();
float test_battary_level();
void stop_all();
void drive_motor ( motor_t motor_e , int spd);
void stop_motor ( motor_t motor );
prog_number_t update_prog_num(prog_number_t prog);
char* prog_num_to_str(prog_number_t prog);  
void handler();
void motor_r_interrupt();
void motor_l_interrupt();
void line_follower_loop();
void line_folower();
void encoder_playback(int* left_array, int* right_array,int max_index);
void encoder_follower (int target_r_cnt, int target_l_cnt, int basic_speed);
void blink_led (int t_low, int t_high, int repeat);
void exec_uart_cmd(ParsedCommand cmd_l);

// GLOBALS //
int Ierror;
int Perror_old;
unsigned long time;
int serial_en;

int motor_l_cnt;
int motor_r_cnt;  

unsigned long current_time = 0;
unsigned long previous_time = 0;
unsigned long time_difference = 0;
unsigned long push_time;
unsigned long time_flag;
int max_speed = 100;

const int PNAME_MAX = 2;
prog_number_t prog = LINE_FOLLOWER;


