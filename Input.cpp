#include "Input.h"

InputHandler::InputHandler()
  : currentVolume(15),
    lastVolume(15),
    lastPotReadTime(0),
    playCallback(nullptr),
    nextCallback(nullptr),
    prevCallback(nullptr),
    volumeCallback(nullptr) {
  
  buttons[0] = {BTN_PLAY, HIGH, 0, false};
  buttons[1] = {BTN_NEXT, HIGH, 0, false};
  buttons[2] = {BTN_PREV, HIGH, 0, false};
}

void InputHandler::begin() {
  pinMode(BTN_PLAY, INPUT_PULLUP);
  pinMode(BTN_NEXT, INPUT_PULLUP);
  pinMode(BTN_PREV, INPUT_PULLUP);
  
  // ADC pin doesn't need pinMode on ESP32
}

void InputHandler::update() {
  // Update buttons
  debounceButton(buttons[0], playCallback);
  debounceButton(buttons[1], nextCallback);
  debounceButton(buttons[2], prevCallback);
  
  // Update potentiometer every 100ms to avoid noise
  unsigned long now = millis();
  if (now - lastPotReadTime > 100) {
    lastPotReadTime = now;
    
    int raw = analogRead(POT_VOL);  // 0-4095 on ESP32
    uint8_t newVol = map(raw, 0, 4095, 0, 30);
    
    // Only trigger callback if volume changed significantly (hysteresis)
    if (abs(newVol - currentVolume) > 0) {
      currentVolume = newVol;
      if (volumeCallback) {
        volumeCallback(currentVolume);
      }
    }
  }
}

void InputHandler::debounceButton(ButtonState& btn, ButtonCallback callback) {
  uint8_t reading = digitalRead(btn.pin);
  unsigned long now = millis();
  
  // If the button state changed
  if (reading != btn.lastState) {
    btn.lastDebounceTime = now;
  }
  
  // If enough time has passed, consider the button stable
  if ((now - btn.lastDebounceTime) > DEBOUNCE_MS) {
    if (reading != btn.isPressed) {
      btn.isPressed = reading;
      
      // Button is now LOW (pressed) — trigger callback
      if (btn.isPressed == LOW && callback != nullptr) {
        callback();
      }
    }
  }
  
  btn.lastState = reading;
}

void InputHandler::onPlayPressed(ButtonCallback cb) {
  playCallback = cb;
}

void InputHandler::onNextPressed(ButtonCallback cb) {
  nextCallback = cb;
}

void InputHandler::onPrevPressed(ButtonCallback cb) {
  prevCallback = cb;
}

void InputHandler::onVolumeChanged(void (*cb)(uint8_t)) {
  volumeCallback = cb;
}
