#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

// OLED dimensions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C  // I2C address for most 0.96" OLED modules

class DisplayHandler {
public:
  DisplayHandler();
  
  // Initialize the OLED display
  bool begin();
  
  // Update display with player state
  void updateTrack(uint16_t trackNum);
  void updateVolume(uint8_t vol);
  void updateStatus(bool isPlaying);
  void updateAll(uint16_t trackNum, uint8_t vol, bool isPlaying);
  
  // Raw display control
  void clear();
  void display();
  
private:
  Adafruit_SSD1306 oled;
  
  // Cached state (to avoid redrawing if nothing changed)
  uint16_t cachedTrack;
  uint8_t cachedVolume;
  bool cachedPlaying;
  
  // Drawing methods
  void drawTrackInfo();
  void drawVolumeBar();
  void drawStatusIcon();
};

#endif
