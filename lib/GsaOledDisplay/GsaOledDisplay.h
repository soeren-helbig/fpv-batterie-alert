#ifndef GSA_OLED_DISPLAY_H_
#define GSA_OLED_DISPLAY_H_

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <GsaBatterieMonitor.h>

#define GSA_SCREEN_WIDTH 128            // OLED display width, in pixels
#define GSA_SCREEN_HEIGHT 32            // OLED display height, in pixels

#define GSA_OLED_RESET     4           // Reset pin # (or -1 if sharing Arduino reset pin)
#define GSA_SCREEN_ADDRESS 0x3C         // See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

#define GSA_VIEW_MODE_CELL_VOLTAGE 1
#define GSA_VIEW_MODE_VOLTAGE 2
#define GSA_VIEW_MODE_ALARMS 3

#define GSA_MODE_DURATION 3000

class GsaOledDisplay
{
  private:
    Adafruit_SSD1306* display;
    uint8_t viewMode;
    unsigned long lastModeChange;
    GsaBatterieMonitor *batterieMonitor;

  public:
    GsaOledDisplay(GsaBatterieMonitor *batterieMonitor);

    void showBootsScreen();
    void updateDisplay();

  private:
    void updateViewMode();
    
    void showCellVoltage();
    void drawCellVoltageItem(uint8_t number);

    void showVoltage();
    void drawVoltageItem(uint8_t number, String label, float voltage);

    void showAlarms();
    void drawAlarmItem(uint8_t number,float voltage);
};

#endif