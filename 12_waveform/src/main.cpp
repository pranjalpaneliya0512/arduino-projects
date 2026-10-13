
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDRESS 0x3C
#define SOUND_PIN A0

Adafruit_SSD1306 display(
    SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET
);

const int NUM_SAMPLES = 128;
int audioSamples[NUM_SAMPLES];

float displayLevel = 0;
float peakLevel = 0;

unsigned long lastPeakTime = 0;

void setup() {
    Serial.begin(9600);
    pinMode(SOUND_PIN, INPUT);

    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
        while (true) {}
    }

    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);

    display.setCursor(15, 19);
    display.println("REAL AUDIO WAVE");
    display.drawLine(15, 32, 112, 32, SSD1306_WHITE);
    display.setCursor(28, 42);
    display.println("SIGNAL MONITOR");
    display.display();

    delay(1000);
}

void loop() {
    // 1. Capture the actual microphone signal
    int minimum = 1023;
    int maximum = 0;
    long total = 0;

    for (int i = 0; i < NUM_SAMPLES; i++) {
        int value = analogRead(SOUND_PIN);
        audioSamples[i] = value;

        if (value < minimum) minimum = value;
        if (value > maximum) maximum = value;

        total += value;
    }

    // 2. Remove the microphone's DC offset
    int midpoint = total / NUM_SAMPLES;

    // Find the largest real signal variation
    int maxDeviation = 0;

    for (int i = 0; i < NUM_SAMPLES; i++) {
        int deviation = abs(audioSamples[i] - midpoint);

        if (deviation > maxDeviation) {
            maxDeviation = deviation;
        }
    }

    // 3. Measure sound level for the meter
    int amplitude = maximum - minimum;

    float rawLevel = constrain(
        (amplitude - 6) * 100.0 / 10.0,
        0.0,
        100.0
    );

    // Smooth the meter separately from the waveform
    if (rawLevel > displayLevel) {
        displayLevel = displayLevel * 0.4 + rawLevel * 0.6;
    } else {
        displayLevel = displayLevel * 0.88 + rawLevel * 0.12;
    }

    if (displayLevel > peakLevel) {
        peakLevel = displayLevel;
    }

    if (millis() - lastPeakTime >= 100) {
        peakLevel = max((float)displayLevel, peakLevel - 1.0);
        lastPeakTime = millis();
    }

    // 4. Draw a clean instrument-style screen
    display.clearDisplay();

    display.setTextSize(1);
    display.setCursor(0, 0);
    display.print("AUDIO");
    display.setCursor(42, 0);
    display.print("LIVE WAVE");
    display.setCursor(105, 0);
    display.print("IN");

    display.drawFastHLine(0, 10, 128, SSD1306_WHITE);

    // Waveform frame
    display.drawRect(0, 12, 128, 35, SSD1306_WHITE);

    const int centerY = 29;
    const int maxHeight = 15;

    // Auto-scale each captured waveform to make the actual
    // voltage changes visible, even at low sound levels.
    float scale = (float)maxHeight / max(2, maxDeviation);

    for (int x = 0; x < 127; x++) {
        int sample1 = audioSamples[x] - midpoint;
        int sample2 = audioSamples[x + 1] - midpoint;

        int y1 = centerY - (int)(sample1 * scale);
        int y2 = centerY - (int)(sample2 * scale);

        y1 = constrain(y1, 14, 45);
        y2 = constrain(y2, 14, 45);

        display.drawLine(x + 1, y1, x + 2, y2, SSD1306_WHITE);
    }

    // Subtle center reference marks
    for (int x = 4; x < 125; x += 8) {
        display.drawPixel(x, centerY, SSD1306_WHITE);
    }

    // Bottom sound-level meter
    display.setCursor(0, 51);
    display.print("LVL");

    display.drawRect(24, 51, 77, 8, SSD1306_WHITE);

    int width = constrain((int)(displayLevel * 0.71), 0, 71);

    if (width > 0) {
        display.fillRect(27, 53, width, 4, SSD1306_WHITE);
    }

    int peakX = 27 + constrain((int)(peakLevel * 0.71), 0, 71);
    display.drawFastVLine(peakX, 50, 10, SSD1306_WHITE);

    display.setCursor(106, 51);
    display.print((int)displayLevel);

    display.display();

    Serial.print("Amplitude: ");
    Serial.print(amplitude);
    Serial.print(" | Midpoint: ");
    Serial.print(midpoint);
    Serial.print(" | Peak deviation: ");
    Serial.println(maxDeviation);
}
