#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
#include <Adafruit_BMP085.h>
#include <DHT.h>

#define SOIL_SENSOR A0  // Define the analog pin

#define DHTPIN 2
#define DHTTYPE DHT11

// Initialize devices
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

RTC_DS3231 rtc;  // Updated to DS3231
Adafruit_BMP085 bmp;
DHT dht(DHTPIN, DHTTYPE);

// Button pins
#define BUTTON_NEXT 4
#define BUTTON_PREV 5
#define BUTTON_GND  7

// Screen states
enum Screen { TIME_DATE, PRESSURE, HUMIDITY_TEMP, SOIL };
Screen currentScreen = TIME_DATE;

void setup() {
    lcd.init();
    lcd.backlight();
    
    pinMode(BUTTON_NEXT, INPUT_PULLUP);
    pinMode(BUTTON_PREV, INPUT_PULLUP);
    pinMode(BUTTON_GND, OUTPUT);
    digitalWrite(BUTTON_GND,LOW);
    
    if (!rtc.begin()) {
        lcd.print("RTC Error");
        while (1);
    }

    if (rtc.lostPower()) {
        lcd.print("RTC Reset");
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // Set time to compile time
        delay(2000);
    }

    if (!bmp.begin()) {
        lcd.print("BMP180 Error");
        while (1);
    }

    dht.begin();
}

void loop() {
    if (digitalRead(BUTTON_NEXT) == LOW) {
        currentScreen = (Screen)((currentScreen + 1) % 4);
        delay(200); // Debounce
    }

    if (digitalRead(BUTTON_PREV) == LOW) {
        currentScreen = (Screen)((currentScreen - 1 + 4) % 4); // Go back
        delay(200);
    }

    lcd.clear();
    switch (currentScreen) {
        case TIME_DATE:
            displayTimeDate();
            break;
        case PRESSURE:
            displayPressure();
            break;
        case HUMIDITY_TEMP:
            displayHumidityTemp();
            break;
        case SOIL:
            displaySoil();
            break;            
    }
    delay(1000); // Small delay to avoid flickering
}

String twoDigits(int number) {
    if (number < 10) return "0" + String(number);
    return String(number);
}

void displayTimeDate() {
    DateTime now = rtc.now();
    lcd.setCursor(0, 0);
    lcd.print(now.year());
    lcd.print('/');
    lcd.print(twoDigits(now.month()));
    lcd.print('/');
    lcd.print(twoDigits(now.day()));
    
    lcd.setCursor(0, 1);
    lcd.print(twoDigits(now.hour()));
    lcd.print(':');
    lcd.print(twoDigits(now.minute()));
    lcd.print(':');
    lcd.print(twoDigits(now.second()));
}

void displayPressure() {
    lcd.setCursor(0, 0);
    lcd.print("Pressure:");
    lcd.setCursor(0, 1);
    lcd.print(bmp.readPressure() / 100.0);
    lcd.print(" mBar");
}

void displayHumidityTemp() {
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(dht.readTemperature());
    lcd.print("C");

    lcd.setCursor(0, 1);
    lcd.print("Humidity: ");
    lcd.print(dht.readHumidity());
    lcd.print("%");
}

void displaySoil() {
    int moistureRaw = analogRead(SOIL_SENSOR);
    int moisturePercent = map(moistureRaw, 1023, 300, 0, 100);  // Adjust range as needed
    lcd.setCursor(0, 0);
    lcd.print("Soil Moisture:");
    lcd.setCursor(0, 1);
    lcd.print(moisturePercent);
    lcd.print("%");  
}

