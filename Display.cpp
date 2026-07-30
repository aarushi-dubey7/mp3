#include "Display.h"

DisplayHandler::DisplayHandler()
  : oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1),
    cachedTrack(0),
    cachedVolume(0),
    cachedPlaying(false) {
}

bool DisplayHandler::begin() {
  if (!oled.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println("[Display] OLED initialization failed!");
    return false;
  }
  
  Serial.println("[Display] OLED initialized");
  
  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(0, 0);
  oled.println("MP3 Player");
  oled.println("Initializing...");
  oled.display();
  
  return true;
}

void DisplayHandler::updateTrack(uint16_t trackNum) {
  if (trackNum != cachedTrack) {
    cachedTrack = trackNum;
    drawTrackInfo();
    oled.display();
  }
}

void DisplayHandler::updateVolume(uint8_t vol) {
  if (vol != cachedVolume) {
    cachedVolume = vol;
    drawVolumeBar();
    oled.display();
  }
}

void DisplayHandler::updateStatus(bool isPlaying) {
  if (isPlaying != cachedPlaying) {
    cachedPlaying = isPlaying;
    drawStatusIcon();
    oled.display();
  }
}

void DisplayHandler::updateAll(uint16_t trackNum, uint8_t vol, bool isPlaying) {
  oled.clearDisplay();
  
  cachedTrack = trackNum;
  cachedVolume = vol;
  cachedPlaying = isPlaying;
  
  drawTrackInfo();
  drawVolumeBar();
  drawStatusIcon();
  
  oled.display();
}

void DisplayHandler::clear() {
  oled.clearDisplay();
  oled.display();
}

void DisplayHandler::display() {
  oled.display();
}

void DisplayHandler::drawTrackInfo() {
  oled.setTextSize(2);
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(0, 0);
  oled.print("Track ");
  oled.println(cachedTrack);
  
  oled.setTextSize(1);
  oled.setCursor(0, 20);
  oled.println("---");
}

void DisplayHandler::drawVolumeBar() {
  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(0, 35);
  oled.print("Vol: ");
  oled.print(cachedVolume);
  oled.println("/30");
  
  // Draw a simple volume bar using characters
  oled.setCursor(0, 45);
  int barLen = map(cachedVolume, 0, 30, 0, 16);  // 16 character width
  for (int i = 0; i < 16; i++) {
    if (i < barLen) {
      oled.print("|");
    } else {
      oled.print("-");
    }
  }
}

void DisplayHandler::drawStatusIcon() {
  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(100, 35);
  
  if (cachedPlaying) {
    oled.println("PLAY");
  } else {
    oled.println("STOP");
  }
}
