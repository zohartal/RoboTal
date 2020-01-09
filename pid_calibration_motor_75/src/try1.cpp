#include <Arduino.h>

void try1() {
    int i=0;
    for (i=0;i<10;i++){
        delay(250);
        digitalWrite(LED_BUILTIN , LOW);
        delay(250);
        digitalWrite(LED_BUILTIN , HIGH);
    }
}