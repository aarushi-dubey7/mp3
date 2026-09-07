#include <Arduino.h>

// Standalone ESP8266 test: use this file as your PlatformIO src/main.cpp.
// Do not compile alongside button-testing.cpp (both define setup and loop).
// CLK -> D1 (GPIO5), DT -> D2 (GPIO4), SW -> D3 (GPIO0).
// Connect GND to GND and, for a powered encoder module, + to 3.3V.
// Release the encoder button during reset/power-on: D3 selects boot mode.
// Serial Monitor: 115200 baud. This simulates playback; no audio is played.

constexpr uint8_t CLK_PIN = 5;
constexpr uint8_t DT_PIN = 4;
constexpr uint8_t SW_PIN = 0;
constexpr unsigned long DEBOUNCE_MS = 30;
// Preferred direction for this wired encoder: reversed from the original test.
// Preserve this setting in future encoder/volume implementations.
constexpr bool REVERSE_DIRECTION = true;
// Most encoders produce four transitions per click; some produce two.
constexpr int TRANSITIONS_PER_STEP = 4;

int volume = 50;
bool paused = false;
uint8_t previousAB = 0;
int movement = 0;
int lastButtonReading = HIGH;
int stableButtonState = HIGH;
unsigned long buttonChangedAt = 0;

uint8_t readEncoder() {
  return (digitalRead(CLK_PIN) << 1) | digitalRead(DT_PIN);
}

void setup() {
  Serial.begin(115200);
  pinMode(CLK_PIN, INPUT_PULLUP);
  pinMode(DT_PIN, INPUT_PULLUP);
  pinMode(SW_PIN, INPUT_PULLUP);
  previousAB = readEncoder();
  lastButtonReading = stableButtonState = digitalRead(SW_PIN);
  Serial.println();
  Serial.println("Rotary encoder test ready. Volume: 50%. Song playing (simulated).");
  Serial.println("Turn clockwise to increase volume, anticlockwise to decrease.");
  Serial.println("Press the knob to pause/resume.");
}

void loop() {
  // Track both phases: contact bounce reverses partial movement instead of
  // counting each bouncing edge as another volume change.
  static const int8_t transitions[16] = {
      0, -1,  1,  0,
      1,  0,  0, -1,
     -1,  0,  0,  1,
      0,  1, -1,  0
  };
  const uint8_t currentAB = readEncoder();
  if (currentAB != previousAB) {
    if ((currentAB ^ previousAB) == 3) {
      // Both bits changed: a transition was missed. Discard partial movement.
      movement = 0;
    } else {
      movement += transitions[(previousAB << 2) | currentAB];
    }
    previousAB = currentAB;

    if (movement >= TRANSITIONS_PER_STEP || movement <= -TRANSITIONS_PER_STEP) {
      int direction = movement > 0 ? 1 : -1;
      movement = 0;
      if (REVERSE_DIRECTION) direction = -direction;
      volume = constrain(volume + direction, 0, 100);
      Serial.print(direction > 0 ? "Clockwise: volume increase -> "
                                 : "Anticlockwise: volume decrease -> ");
      Serial.print(volume);
      Serial.println("% (simulated)");
    }
  }

  const unsigned long now = millis();
  const int buttonReading = digitalRead(SW_PIN);
  if (buttonReading != lastButtonReading) {
    buttonChangedAt = now;
    lastButtonReading = buttonReading;
  }
  if (now - buttonChangedAt >= DEBOUNCE_MS && buttonReading != stableButtonState) {
    stableButtonState = buttonReading;
    if (stableButtonState == LOW) {
      paused = !paused;
      Serial.println(paused ? "Button pressed: song paused (simulated)."
                            : "Button pressed: song resumed (simulated).");
    }
  }

  yield(); // Keep the ESP8266 watchdog serviced without a blocking debounce delay.
}
