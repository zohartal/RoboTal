# Arduino CLI Commands for RoboTal

This file contains the commands used to compile and upload Arduino sketches using the `arduino-cli` bundled with the Arduino IDE on macOS.

## Configuration
- **Board**: Arduino Micro
- **FQBN**: `arduino:avr:micro`
- **CLI Path**: `/Applications/Arduino IDE.app/Contents/Resources/app/lib/backend/resources/arduino-cli`

## Common Commands

### 1. List Connected Boards
Use this to find the current port (e.g., `/dev/cu.usbmodem14401`).
```bash
"/Applications/Arduino IDE.app/Contents/Resources/app/lib/backend/resources/arduino-cli" board list
```

### 2. Compile Sketch
Replace `<SKETCH_PATH>` with the path to your `.ino` file.
```bash
"/Applications/Arduino IDE.app/Contents/Resources/app/lib/backend/resources/arduino-cli" compile --fqbn arduino:avr:micro "<SKETCH_PATH>"
```

### 3. Upload Sketch
Replace `<PORT>` with the port found in step 1 and `<SKETCH_PATH>` with the path to your `.ino` file.
```bash
"/Applications/Arduino IDE.app/Contents/Resources/app/lib/backend/resources/arduino-cli" upload -p <PORT> --fqbn arduino:avr:micro "<SKETCH_PATH>"
```

### 4. Compile and Upload (Combined)
```bash
"/Applications/Arduino IDE.app/Contents/Resources/app/lib/backend/resources/arduino-cli" compile --fqbn arduino:avr:micro "<SKETCH_PATH>" && \
"/Applications/Arduino IDE.app/Contents/Resources/app/lib/backend/resources/arduino-cli" upload -p <PORT> --fqbn arduino:avr:micro "<SKETCH_PATH>"
```

## Notes
- The port identifier (`/dev/cu.usbmodemXXXXX`) may change between connections or after a reset.
- If an upload fails with "No such file or directory", re-run `board list` to check if the port name changed.
