#include <Arduino.h>
#include <Servo.h>

Servo myServo;

void setup() {
  myServo.attach(9);
}

void loop() {

  // 0° → 180°
  for (int angle = 0; angle <= 180; angle++) {
    myServo.write(angle);
    delay(15);
  }

  delay(500);

  // 180° → 0°
  for (int angle = 180; angle >= 0; angle--) {
    myServo.write(angle);
    delay(15);
  }

  delay(500);
}