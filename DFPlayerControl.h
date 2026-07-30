#ifndef DFPLAYER_CONTROL_H
#define DFPLAYER_CONTROL_H

#include <Arduino.h>
#include <SoftwareSerial.h>
#include "DFRobotDFPlayerMini.h"

// Pin definitions for DFPlayer UART
#define DFPLAYER_RX 16
#define DFPLAYER_TX 17

class DFPlayerControl {
public:
  DFPlayerControl();
  
  // Initialize the DFPlayer
  bool begin();
  
  // Playback control
  void play(uint16_t trackNumber);
  void pause();
  void resume();
  void next();
  void previous();
  
  // Volume control (0-30)
  void setVolume(uint8_t vol);
  
  // Query state
  uint16_t getCurrentTrack() const { return currentTrack; }
  uint8_t getCurrentVolume() const { return currentVolume; }
  bool isPlaying() const { return playing; }
  
  // Call this periodically to check for DFPlayer feedback
  void update();
  
private:
  SoftwareSerial* dfSerial;
  DFRobotDFPlayerMini dfPlayer;
  
  // State tracking
  uint16_t currentTrack;
  uint8_t currentVolume;
  bool playing;
  bool initialized;
};

#endif
