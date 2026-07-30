#ifndef INPUT_H
#define INPUT_H

#include <Arduino.h>

// Pin definitions
#define BTN_PLAY 25
#define BTN_NEXT 26
#define BTN_PREV 27
#define POT_VOL 34

// Debounce timing (milliseconds)
#define DEBOUNCE_MS 20
#define HOLD_TIME_MS 50

// Function pointer types for button callbacks
typedef void (*ButtonCallback)();

class InputHandler {
public:
  // Constructor
  InputHandler();
  
  // Initialize buttons and potentiometer
  void begin();
  
  // Call this in your main loop
  void update();
  
  // Register callbacks for button events
  void onPlayPressed(ButtonCallback cb);
  void onNextPressed(ButtonCallback cb);
  void onPrevPressed(ButtonCallback cb);
  void onVolumeChanged(void (*cb)(uint8_t));  // Passes new volume 0-30
  
  // Get current state
  uint8_t getVolume() const { return currentVolume; }
  
private:
  // Button state tracking
  struct ButtonState {
    uint8_t pin;
    bool lastState;
    unsigned long lastDebounceTime;
    bool isPressed;
  };
  
  ButtonState buttons[3];  // PLAY, NEXT, PREV
  
  // Potentiometer
  uint8_t currentVolume;
  uint8_t lastVolume;
  unsigned long lastPotReadTime;
  
  // Callbacks
  ButtonCallback playCallback;
  ButtonCallback nextCallback;
  ButtonCallback prevCallback;
  void (*volumeCallback)(uint8_t);
  
  // Internal methods
  void debounceButton(ButtonState& btn, ButtonCallback callback);
};

#endif
