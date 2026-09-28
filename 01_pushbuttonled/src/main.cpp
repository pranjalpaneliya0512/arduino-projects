#include <Arduino.h>

#define BUTTON 2
#define LED 8

bool ledState = LOW;
bool lastButton = HIGH;

void setup() {
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(LED, OUTPUT);

  digitalWrite(LED, LOW);
}

void loop() {
  bool button = digitalRead(BUTTON);

  if (lastButton == HIGH && button == LOW) {
    ledState = !ledState;
    digitalWrite(LED, ledState);

    delay(200);
  }

  lastButton = button;
}