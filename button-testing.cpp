#include <Arduino.h>

// ESP8266 NodeMCU / Wemos D7 is GPIO13. Connect the button to D7 and GND.
constexpr uint8_t BUTTON_PIN = 13;
constexpr unsigned long DEBOUNCE_MS = 30;

int lastReading = HIGH;
int stableState = HIGH;
unsigned long lastChangeTime = 0;

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.println("Button test ready. Press the button connected to D7.");
}

void loop() {
  const int reading = digitalRead(BUTTON_PIN);
  const unsigned long now = millis();

  if (reading != lastReading) {
    lastChangeTime = now;
    lastReading = reading;
  }

  // Wait for the contacts to settle so each press produces one message.
  if (now - lastChangeTime >= DEBOUNCE_MS && reading != stableState) {
    stableState = reading;

    // The pull-up holds the input HIGH; pressing the button connects it to GND.
    if (stableState == LOW) {
      Serial.println("Button pressed!");
    }
  }

  delay(1);
}
