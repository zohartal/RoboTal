# Uno line follower

Arduino Uno sketch for the RoboTal line follower.

## Board

- Board: Arduino Uno
- FQBN: `arduino:avr:uno`
- Sketch: `uno_line_follower.ino`

## Current program

`uno_line_follower.ino` blinks the onboard LED so the board and upload path can be checked before the line-follower logic is added.

## Pinout

| Function | Pin | Notes |
| --- | --- | --- |
| Onboard LED | D13 (`LED_BUILTIN`) | Blink test. Shared with the SPI SCK header. |

Line-follower pins (sensors, motors, buttons) go in this table as the hardware is wired.

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

-
