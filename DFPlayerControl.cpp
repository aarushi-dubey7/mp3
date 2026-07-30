#include "DFPlayerControl.h"

DFPlayerControl::DFPlayerControl()
  : dfSerial(nullptr),
    currentTrack(1),
    currentVolume(15),
    playing(false),
    initialized(false) {
}

bool DFPlayerControl::begin() {
  // Create software serial on specified pins
  dfSerial = new SoftwareSerial(DFPLAYER_RX, DFPLAYER_TX);
  dfSerial->begin(9600);  // DFPlayer always uses 9600 baud
  
  delay(500);  // Wait for DFPlayer to stabilize
  
  // Initialize DFPlayer
  if (!dfPlayer.begin(*dfSerial)) {
    Serial.println("[DFPlayer] Initialization failed!");
    return false;
  }
  
  Serial.println("[DFPlayer] Initialized successfully");
  
  // Set initial volume
  dfPlayer.volume(currentVolume);
  
  initialized = true;
  return true;
}

void DFPlayerControl::play(uint16_t trackNumber) {
  if (!initialized) return;
  
  currentTrack = trackNumber;
  dfPlayer.play(trackNumber);
  playing = true;
  
  Serial.print("[DFPlayer] Playing track: ");
  Serial.println(trackNumber);
}

void DFPlayerControl::pause() {
  if (!initialized) return;
  dfPlayer.pause();
  playing = false;
  Serial.println("[DFPlayer] Paused");
}

void DFPlayerControl::resume() {
  if (!initialized) return;
  dfPlayer.start();
  playing = true;
  Serial.println("[DFPlayer] Resumed");
}

void DFPlayerControl::next() {
  if (!initialized) return;
  dfPlayer.next();
  playing = true;
  Serial.println("[DFPlayer] Next track");
}

void DFPlayerControl::previous() {
  if (!initialized) return;
  dfPlayer.previous();
  playing = true;
  Serial.println("[DFPlayer] Previous track");
}

void DFPlayerControl::setVolume(uint8_t vol) {
  if (!initialized) return;
  
  // Clamp volume to 0-30
  vol = constrain(vol, 0, 30);
  
  if (vol != currentVolume) {
    currentVolume = vol;
    dfPlayer.volume(vol);
    
    Serial.print("[DFPlayer] Volume set to: ");
    Serial.println(vol);
  }
}

void DFPlayerControl::update() {
  if (!initialized) return;
  
  // Read any feedback from DFPlayer (optional)
  // The DFPlayer sends status updates, but they're not critical for basic operation
  if (dfSerial->available()) {
    // You could parse feedback here if needed
    // For now, just clear the buffer
    while (dfSerial->available()) {
      dfSerial->read();
    }
  }
}
