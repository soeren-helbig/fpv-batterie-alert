#ifndef GSA_BATTERIE_H_
#define GSA_BATTERIE_H_

#include <Arduino.h>
#include <GsaBuzzer.h>

#define GSA_BATTERIE_CELL_COUNT 4

#define READ_COUNT 10
#define READ_TIMEOUT 1000

#define GSA_BATTERIE_CELL_1 A1
#define GSA_BATTERIE_CELL_2 A2
#define GSA_BATTERIE_CELL_3 A3
#define GSA_BATTERIE_CELL_4 A6

class GsaBatterieMonitor
{
  private:
    unsigned long lastRead;
    
    uint8_t ports[GSA_BATTERIE_CELL_COUNT];
    
    float cellVoltages[GSA_BATTERIE_CELL_COUNT];
    
    float voltageDeviderCorretion[GSA_BATTERIE_CELL_COUNT];
    float scaling[GSA_BATTERIE_CELL_COUNT];

    GsaBuzzer *buzzer;

    float alertBatterie;
    float alertCell;

  public:
    GsaBatterieMonitor(GsaBuzzer *buzzer);   

    void readCellVoltages();

    float getCellVoltage(uint8_t cellNumber);
    float getAverageCellVoltage();
    float getBatterieVoltage();

    float getAlertCell();
    float getAlertBatterie();

  private:
    void readCellVoltage(uint8_t cellNumber);
};

#endif