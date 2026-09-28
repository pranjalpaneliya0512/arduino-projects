#include <Arduino.h>
#include <Servo.h>

Servo myServo;

const int buttonPin = 2;
const int servoPin = 9;

bool servoState = false;
bool lastButtonState = HIGH;

void setup() {
    pinMode(buttonPin, INPUT_PULLUP);

    myServo.attach(servoPin);
    myServo.write(0);
}

void loop() {
    bool buttonState = digitalRead(buttonPin);

    // Button pressed
    if (lastButtonState == HIGH && buttonState == LOW) {
        servoState = !servoState;

        if (servoState) {
            myServo.write(180);
        } else {
            myServo.write(0);
        }

        delay(50); // debounce
    }

    lastButtonState = buttonState;
}