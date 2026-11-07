// 0.96" I2C OLED found at 0x3C with SDA on D5 and SCL on D6.
// A5 does not toggle, so this does not use the hardware I2C pins.
//
// The OLED uses U8x8 text mode and only rewrites characters that changed, so
// the status screen can refresh much faster than a full-frame redraw.

#include <avr/wdt.h>
#include <U8x8lib.h>

const uint8_t OLED_SDA = 5;
const uint8_t OLED_SCL = 6;
const uint8_t OLED_ADDRESS = 0x3C;

const uint8_t IR_PINS[] = {A1, A2, A3, A4, A5};
const uint8_t IR_COUNT = sizeof(IR_PINS) / sizeof(IR_PINS[0]);
const unsigned long IR_READ_INTERVAL_MS = 500;

const uint8_t TACH_L_PIN = 2;  // INT0
const uint8_t TACH_R_PIN = 3;  // INT1

// Buttons wired between the pin and GND. Pressed reads LOW.
const uint8_t BUTTON_1_PIN = A0;
const uint8_t BUTTON_2_PIN = 4;

volatile unsigned long tach_l_count = 0;
volatile unsigned long tach_r_count = 0;

U8X8_SSD1306_128X64_NONAME_SW_I2C display(OLED_SCL, OLED_SDA, U8X8_PIN_NONE);

const uint8_t LINE_COUNT = 3;
const uint8_t LINE_ROWS[LINE_COUNT] = {0, 3, 6};  // 7x14 font is 2 tile rows tall
const uint8_t LINE_WIDTH = 16;
char shown_text[LINE_COUNT][LINE_WIDTH + 1];

void tach_l_isr() { tach_l_count++; }
void tach_r_isr() { tach_r_count++; }

// 32-bit reads are not atomic on AVR, so block the ISRs briefly.
void read_encoders(unsigned long &left, unsigned long &right) {
  noInterrupts();
  left = tach_l_count;
  right = tach_r_count;
  interrupts();
}

bool button_pressed(uint8_t pin) {
  return digitalRead(pin) == LOW;
}

// Writes only the characters of line `index` that differ from what is shown.
void draw_line(uint8_t index, const char *text) {
  char padded[LINE_WIDTH + 1];
  snprintf(padded, sizeof(padded), "%-16s", text);
  for (uint8_t i = 0; i < LINE_WIDTH; i++) {
    if (padded[i] != shown_text[index][i]) {
      display.drawGlyph(i, LINE_ROWS[index], padded[i]);
      shown_text[index][i] = padded[i];
    }
  }
}

void draw_status(unsigned long left, unsigned long right,
                 bool button_1, bool button_2) {
  char text[24];
  snprintf(text, sizeof(text), "L:%10lu", left);
  draw_line(0, text);
  snprintf(text, sizeof(text), "R:%10lu", right);
  draw_line(1, text);
  snprintf(text, sizeof(text), "A0:%c  D4:%c",
           button_1 ? '+' : '-', button_2 ? '+' : '-');
  draw_line(2, text);
}

int read_ir_sensor(uint8_t pin) {
  return analogRead(pin);
}

void read_all_ir_sensors() {
  for (uint8_t i = 0; i < IR_COUNT; i++) {
    int value = read_ir_sensor(IR_PINS[i]);
    if (i > 0) {
      Serial.print(' ');
    }
    Serial.print('A');
    Serial.print(i + 1);
    Serial.print('=');
    Serial.print(value);
  }
  Serial.println();
}

void setup() {
  MCUSR = 0;
  wdt_disable();
  Serial.begin(9600);
  for (uint8_t i = 0; i < IR_COUNT; i++) {
    pinMode(IR_PINS[i], INPUT);
  }

  display.setI2CAddress(OLED_ADDRESS << 1);
  display.begin();
  display.setFont(u8x8_font_7x14_1x2_f);
  display.drawString(1, 3, "RoboLine v1.0");

  Serial.println("OLED 0x3C SDA=D5 SCL=D6 text sent");

  pinMode(BUTTON_1_PIN, INPUT_PULLUP);
  pinMode(BUTTON_2_PIN, INPUT_PULLUP);
  pinMode(TACH_L_PIN, INPUT_PULLUP);
  pinMode(TACH_R_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(TACH_L_PIN), tach_l_isr, RISING);
  attachInterrupt(digitalPinToInterrupt(TACH_R_PIN), tach_r_isr, RISING);

  delay(1000);  // keep the splash screen visible
  display.clear();
  for (uint8_t i = 0; i < LINE_COUNT; i++) {
    memset(shown_text[i], ' ', LINE_WIDTH);
    shown_text[i][LINE_WIDTH] = '\0';
  }
}

void loop() {
  static unsigned long last_ir_ms = 0;
  unsigned long now = millis();

  if (now - last_ir_ms >= IR_READ_INTERVAL_MS) {
    last_ir_ms = now;
    read_all_ir_sensors();
  }

  // Runs every pass: only changed characters are sent to the OLED.
  unsigned long left, right;
  read_encoders(left, right);
  draw_status(left, right,
              button_pressed(BUTTON_1_PIN), button_pressed(BUTTON_2_PIN));
}
