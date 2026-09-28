#include <Arduino.h>

#define HALL_PIN 2

void setup() {
    Serial.begin(9600);
    pinMode(HALL_PIN, INPUT);
}

void loop() {
    int sensor = digitalRead(HALL_PIN);

    if (sensor == LOW) {
        Serial.println("MAGNET DETECTED");
    } else {
        Serial.println("NO MAGNET");
    }

    delay(200);
}