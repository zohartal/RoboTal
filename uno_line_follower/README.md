# Uno line follower

Arduino Uno sketch for the RoboTal line follower.

## Board

- Board: Arduino Uno
- FQBN: `arduino:avr:uno`
- Sketch: `uno_line_follower.ino`

## Current program

`uno_line_follower.ino` currently:

- Shows a splash screen on the OLED at startup.
- Reads the 5 IR sensors (A1-A5) and prints them to the serial monitor (9600 baud) every 0.5 s.
- Counts tachometer pulses on D2 (left) and D3 (right) with `RISING` interrupts, and shows the raw counts on the OLED. Counts only go up (no direction).
- The OLED runs in U8x8 text mode (7x14 font) and is updated on every `loop()` pass. Only characters that changed are rewritten, so it refreshes much faster than a full-frame redraw. The refresh rate has not been measured.
- Shows the two button states on the OLED (`+` pushed, `-` not pushed), e.g. `A0:+  D4:-`.

## Pinout

| Function | Pin | Notes |
| --- | --- | --- |
| IR sensor 1 | `A1` | Analog input |
| IR sensor 2 | `A2` | Analog input |
| IR sensor 3 | `A3` | Analog input |
| IR sensor 4 | `A4` | Analog input |
| IR sensor 5 | `A5` | Analog input |
| Left motor En | `D9` | PWM speed control |
| Left motor In1 | `D8` | Direction control |
| Left motor In2 | `D7` | Direction control |
| Right motor En | `D10` | PWM speed control |
| Right motor In1 | `D11` | Direction control |
| Right motor In2 | `D12` | Direction control |
| Tachometer L | `D2` | External interrupt (`INT0`) |
| Tachometer R | `D3` | External interrupt (`INT1`) |
| I2C - SCL | `D6` | Software I2C clock (0.96" OLED at `0x3C`) |
| I2C - SDA | `D5` | Software I2C data (0.96" OLED at `0x3C`) |
| Button 1 | `A0` | Digital input, `INPUT_PULLUP`, button to GND (pressed = LOW) |
| Button 2 | `D4` | Digital input, `INPUT_PULLUP`, button to GND (pressed = LOW) |
| Onboard LED | `D13` (`LED_BUILTIN`) | Shared with the SPI SCK header |

## Build

CLI bundled with the Arduino IDE on macOS. List boards first to get the port (`/dev/cu.usbmodem...`):

```bash
"/Applications/Arduino IDE.app/Contents/Resources/app/lib/backend/resources/arduino-cli" board list
```

Compile:

```bash
"/Applications/Arduino IDE.app/Contents/Resources/app/lib/backend/resources/arduino-cli" compile --fqbn arduino:avr:uno "uno_line_follower.ino"
```

Upload (replace `<PORT>`):

```bash
"/Applications/Arduino IDE.app/Contents/Resources/app/lib/backend/resources/arduino-cli" upload -p <PORT> --fqbn arduino:avr:uno "uno_line_follower.ino"
```

## Notes

- Upload port seen on this Mac: `/dev/cu.usbserial-A5069RR4` (FTDI FT232R). It can change between connections.
- OLED (`0x3C`, software I2C on D5/D6) and both tachometers are confirmed working on the hardware.
- Tachometers: slotted IR sensor modules, counted on `RISING` edges. **20 counts per wheel revolution (360 deg)**, so one count is 18 degrees of wheel rotation.
- IR sensors read about 760-780 on the surface used in the first test (not yet calibrated for line vs. background).
