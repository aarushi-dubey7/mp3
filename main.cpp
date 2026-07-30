#include <Arduino.h>
#include "Input.h"
#include "DFPlayerControl.h"
#include "Display.h"

// ===== Global Objects =====
InputHandler input;
DFPlayerControl dfPlayer;
DisplayHandler display;

// ===== Callback Functions =====
void onPlayButtonPressed() {
  Serial.println("[Main] Play/Pause button pressed");
  
  if (dfPlayer.isPlaying()) {
    dfPlayer.pause();
  } else {
    dfPlayer.resume();
  }
  
  // Update display
  display.updateStatus(dfPlayer.isPlaying());
}

void onNextButtonPressed() {
  Serial.println("[Main] Next button pressed");
  dfPlayer.next();
  
  // Update display (track will be determined by DFPlayer)
  display.updateTrack(dfPlayer.getCurrentTrack());
  display.updateStatus(true);
}

void onPrevButtonPressed() {
  Serial.println("[Main] Previous button pressed");
  dfPlayer.previous();
  
  // Update display
  display.updateTrack(dfPlayer.getCurrentTrack());
  display.updateStatus(true);
}

void onVolumeChanged(uint8_t newVolume) {
  Serial.print("[Main] Volume changed to: ");
  Serial.println(newVolume);
  
  // Update DFPlayer volume
  dfPlayer.setVolume(newVolume);
  
  // Update display
  display.updateVolume(newVolume);
}

// ===== Setup =====
void setup() {
  // Initialize serial for debugging
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n\n=== ESP32 MP3 Player Starting ===");
  
  // Initialize display first (gives visual feedback)
  if (!display.begin()) {
    Serial.println("ERROR: Display initialization failed!");
    while (1) delay(1000);
  }
  
  // Initialize DFPlayer
  if (!dfPlayer.begin()) {
    Serial.println("ERROR: DFPlayer initialization failed!");
    display.clear();
    display.display();
    while (1) delay(1000);
  }
  
  // Initialize input handler
  input.begin();
  
  // Register button callbacks
  input.onPlayPressed(onPlayButtonPressed);
  input.onNextPressed(onNextButtonPressed);
  input.onPrevPressed(onPrevButtonPressed);
  input.onVolumeChanged(onVolumeChanged);
  
  // Initialize display with starting state
  display.updateAll(dfPlayer.getCurrentTrack(), dfPlayer.getCurrentVolume(), dfPlayer.isPlaying());
  
  Serial.println("=== Setup Complete ===\n");
}

// ===== Main Loop =====
void loop() {
  // Update input (checks buttons and potentiometer)
  input.update();
  
  // Update DFPlayer (handles serial feedback if any)
  dfPlayer.update();
  
  // Small delay to prevent overwhelming the CPU
  delay(10);
}
