#include <Arduino.h>

#define CLK 2
#define DT 3
#define SW 4

#define RED 9
#define GREEN 10
#define BLUE 11

int lastCLK;
int color = 0;

void setColor(int r, int g, int b) {
  analogWrite(RED, r);
  analogWrite(GREEN, g);
  analogWrite(BLUE, b);
}

void setup() {
  pinMode(CLK, INPUT_PULLUP);
  pinMode(DT, INPUT_PULLUP);
  pinMode(SW, INPUT_PULLUP);

  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);

  lastCLK = digitalRead(CLK);

  setColor(255, 0, 0);
}

void loop() {

  int currentCLK = digitalRead(CLK);

  if (currentCLK != lastCLK) {

    if (digitalRead(DT) != currentCLK) {
      color++;
    } else {
      color--;
    }

    if (color > 7) color = 0;
    if (color < 0) color = 7;

    switch (color) {

      case 0:
        setColor(255, 0, 0);       // Red
        break;

      case 1:
        setColor(0, 255, 0);       // Green
        break;

      case 2:
        setColor(0, 0, 255);       // Blue
        break;

      case 3:
        setColor(255, 255, 0);     // Yellow
        break;

      case 4:
        setColor(255, 0, 255);     // Purple
        break;

      case 5:
        setColor(0, 255, 255);     // Cyan
        break;

      case 6:
        setColor(255, 255, 255);   // White
        break;

      case 7:
        setColor(255, 100, 0);     // Orange
        break;
    }

    delay(5);
  }

  lastCLK = currentCLK;
}