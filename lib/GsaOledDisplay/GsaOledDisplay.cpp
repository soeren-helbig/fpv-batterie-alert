#ifndef GSA_OLED_DISPLAY_CPP_
#define GSA_OLED_DISPLAY_CPP_

#include "GsaOledDisplay.h"

GsaOledDisplay::GsaOledDisplay(GsaBatterieMonitor *batterieMonitor) 
{
    this->batterieMonitor = batterieMonitor;
    this->viewMode = GSA_VIEW_MODE_VOLTAGE;
    this->lastModeChange = millis();
    this->display = new Adafruit_SSD1306(GSA_SCREEN_WIDTH, GSA_SCREEN_HEIGHT, &Wire, GSA_OLED_RESET);

    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if(!this->display->begin(SSD1306_SWITCHCAPVCC, 0, true, true)) {
        Serial.println(F("SSD1306 allocation failed"));
        for(;;); // Don't proceed, loop forever
    }
};

void GsaOledDisplay::showBootsScreen()
{
    // this->display->display();
    // delay(2000);

    this->display->clearDisplay();
    this->display->fillRoundRect(10, 1, 108, 31, 4, SSD1306_WHITE);

    this->display->setTextSize(1);      // Normal 1:1 pixel scale
    this->display->setTextColor(SSD1306_BLACK); // Draw white text
    this->display->setCursor(55, 6);     // Start at top-left corner
    this->display->cp437(true);         // Use full 256 char 'Code Page 437' font

    this->display->write('F');
    this->display->display();
    delay(500);

    this->display->write('P');
    this->display->display();
    delay(500);

    this->display->write('V');
    this->display->write('\n');
    this->display->display();
    delay(500);

    this->display->setCursor(22, 17);
    this->display->println("Batterie Alarm");
    this->display->display();
};

void GsaOledDisplay::updateDisplay()
{
    this->updateViewMode();
    this->batterieMonitor->readCellVoltages();

    switch (this->viewMode)
    {
        case GSA_VIEW_MODE_VOLTAGE:
            this->showVoltage();
            break;
        case GSA_VIEW_MODE_CELL_VOLTAGE:
            this->showCellVoltage();
            break;
        case GSA_VIEW_MODE_ALARMS:
            this->showAlarms();
            break;
    }
};

void GsaOledDisplay::updateViewMode()
{
    unsigned long now = millis();

    if (this->lastModeChange + GSA_MODE_DURATION > now) {
        return;
    }

    switch (this->viewMode)
    {
        case GSA_VIEW_MODE_VOLTAGE:
            this->viewMode = GSA_VIEW_MODE_CELL_VOLTAGE;
            break;
        case GSA_VIEW_MODE_CELL_VOLTAGE:
            this->viewMode = GSA_VIEW_MODE_ALARMS;
            break;
        case GSA_VIEW_MODE_ALARMS:
            this->viewMode = GSA_VIEW_MODE_VOLTAGE;
            break;
    }

    this->lastModeChange = millis();
};

void GsaOledDisplay::showCellVoltage()
{
    this->display->clearDisplay();

    for (uint8_t cellNumber = 1; cellNumber <= 4; cellNumber++) {
        this->drawCellVoltageItem(cellNumber);
    }

    this->display->display();
}

void GsaOledDisplay::drawCellVoltageItem(uint8_t number)
{
    uint8_t width = 50,
        height = 14,
        xOffset = (number == 1 || number == 3) ? 10 : 70,
        yOffset = (number < 3) ? 1 : 17;
    
    this->display->fillRoundRect(xOffset, yOffset, width, height, 2, SSD1306_WHITE);

    this->display->setTextSize(1);      // Normal 1:1 pixel scale
    this->display->setTextColor(SSD1306_BLACK); // Draw white text
    this->display->setCursor(xOffset + 8, yOffset + 4);

    this->display->print(this->batterieMonitor->getCellVoltage(number - 1), 2);
    this->display->println(" V");
}

void GsaOledDisplay::showVoltage()
{
    this->display->clearDisplay();

    this->drawVoltageItem(1, "Voltage", this->batterieMonitor->getBatterieVoltage());
    this->drawVoltageItem(2, "Cell/avg", this->batterieMonitor->getAverageCellVoltage());

    this->display->display();
};

void GsaOledDisplay::drawVoltageItem(uint8_t number, String label, float voltage)
{
    uint8_t width = 112,
        height = 14,
        xOffset = 10,
        yOffset = (number == 1) ? 1 : 17;
    
    this->display->fillRoundRect(xOffset, yOffset, width, height, 2, SSD1306_WHITE);

    this->display->setTextSize(1);      // Normal 1:1 pixel scale
    this->display->setTextColor(SSD1306_BLACK); // Draw white text
    this->display->setCursor(xOffset + 8, yOffset + 4);

    this->display->print(label);
    this->display->print(": ");
    this->display->print(voltage);
    this->display->print(" V");
}

void GsaOledDisplay::showAlarms()
{
    this->display->clearDisplay();

    this->drawAlarmItem(1, this->batterieMonitor->getAlertBatterie());
    this->drawAlarmItem(2, this->batterieMonitor->getAlertCell());

    this->display->display();
};

void GsaOledDisplay::drawAlarmItem(uint8_t number,float voltage)
{
    uint8_t width = 112,
        height = 14,
        xOffset = 10,
        yOffset = (number == 1) ? 1 : 17;
    
    this->display->fillRoundRect(xOffset, yOffset, width, height, 2, SSD1306_WHITE);

    this->display->setTextSize(1);      // Normal 1:1 pixel scale
    this->display->setTextColor(SSD1306_BLACK); // Draw white text
    this->display->setCursor(xOffset + 8, yOffset + 4);

    this->display->print("Alert ");
    this->display->print(number);
    (voltage < 10)
        ? this->display->print(":  ")
        : this->display->print(": ");
    this->display->print(voltage);
    this->display->print(" V");
}

#endif