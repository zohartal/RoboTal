// 0.96" I2C OLED found at 0x3C with SDA on D5 and SCL on D6.
// A5 does not toggle, so this does not use the hardware I2C pins.

#include <avr/wdt.h>
#include <U8g2lib.h>

const uint8_t OLED_SDA = 5;
const uint8_t OLED_SCL = 6;
const uint8_t OLED_ADDRESS = 0x3C;

const uint8_t IR_PINS[] = {A1, A2, A3, A4, A5};
const uint8_t IR_COUNT = sizeof(IR_PINS) / sizeof(IR_PINS[0]);
const unsigned long IR_READ_INTERVAL_MS = 500;

U8G2_SSD1306_128X64_NONAME_1_SW_I2C display(
    U8G2_R0, OLED_SCL, OLED_SDA, U8X8_PIN_NONE);

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
  display.setFont(u8g2_font_7x14_tf);
  display.firstPage();
  do {
    display.drawStr(18, 39, "RoboLine v1.0");
  } while (display.nextPage());

  Serial.println("OLED 0x3C SDA=D5 SCL=D6 text sent");
}

void loop() {
  static unsigned long last_read_ms = 0;
  unsigned long now = millis();
  if (now - last_read_ms >= IR_READ_INTERVAL_MS) {
    last_read_ms = now;
    read_all_ir_sensors();
  }
}
