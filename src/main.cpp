#include <Arduino.h>
#include <GsaBatterieMonitor.h>
#include <GsaOledDisplay.h>
#include <GsaBuzzer.h>

GsaOledDisplay* display;
GsaBatterieMonitor* batterieMonitor;
GsaBuzzer* buzzer;

void setup() 
{
  Serial.begin(115200);

  buzzer = new GsaBuzzer(13);

  batterieMonitor = new GsaBatterieMonitor(buzzer);

  display = new GsaOledDisplay(batterieMonitor);
  display->showBootsScreen();

  buzzer->playBootTone();
}

void loop() 
{
  display->updateDisplay();
}