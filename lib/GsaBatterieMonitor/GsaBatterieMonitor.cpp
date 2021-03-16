#ifndef GSA_BATTERIE_CPP_
#define GSA_BATTERIE_CPP_

#include <GsaBatterieMonitor.h>

GsaBatterieMonitor::GsaBatterieMonitor(GsaBuzzer *buzzer)
{
    this->alertBatterie = 14.4;
    this->alertCell = 3.6;

    this->buzzer = buzzer;
    this->lastRead = 0;

    this->ports[0] = GSA_BATTERIE_CELL_1;
    this->ports[1] = GSA_BATTERIE_CELL_2;
    this->ports[2] = GSA_BATTERIE_CELL_3;
    this->ports[3] = GSA_BATTERIE_CELL_4;

    this->voltageDeviderCorretion[0] = 1.00;
    this->voltageDeviderCorretion[1] = 1.19;
    this->voltageDeviderCorretion[2] = 1.13;
    this->voltageDeviderCorretion[3] = 1.10;

    this->scaling[0] = 1.02;
    this->scaling[1] = 1.01;
    this->scaling[2] = 1.03;
    this->scaling[3] = 0.98;

    for (uint8_t index = 0; index < GSA_BATTERIE_CELL_COUNT; index++) {
        pinMode(this->ports[index], INPUT);
    }

    this->readCellVoltages();

    for (uint8_t index = 0; index < GSA_BATTERIE_CELL_COUNT; index++) {
        Serial.print("Cell ");
        Serial.print(index);
        Serial.print(" - Voltage: ");
        Serial.println(this->cellVoltages[index]);
    }
};

void GsaBatterieMonitor::readCellVoltages()
{
    unsigned long now = millis();

    if (this->lastRead != 0 && this->lastRead + READ_TIMEOUT > now) {
        return;
    }

    for (uint8_t index = 0; index < GSA_BATTERIE_CELL_COUNT; index++) {
        this->readCellVoltage(index);

        if (this->cellVoltages[index] < this->getAlertCell()) {
            this->buzzer->cellAlert();
        }
    }

    if (this->getBatterieVoltage() < this->getAlertBatterie()) {
        this->buzzer->betterieAlert();
    }

    this->lastRead = now;
};

void GsaBatterieMonitor::readCellVoltage(uint8_t cellNumber)
{
    int analogValue;

    if (cellNumber >= GSA_BATTERIE_CELL_COUNT) {
        return;
    }

    analogValue = analogRead(this->ports[cellNumber]);

    this->cellVoltages[cellNumber] = (((float) analogValue * 5.0) / 1024) * this->voltageDeviderCorretion[cellNumber] * this->scaling[cellNumber];
};

float GsaBatterieMonitor::getCellVoltage(uint8_t cellNumber)
{
    if (cellNumber >= GSA_BATTERIE_CELL_COUNT) {
        return 0.0;
    }

    return this->cellVoltages[cellNumber];
}

float GsaBatterieMonitor::getAverageCellVoltage()
{
    return this->getBatterieVoltage() / GSA_BATTERIE_CELL_COUNT;
};

float GsaBatterieMonitor::getBatterieVoltage()
{
    float batterieVoltage = 0.0;

    for (uint8_t index = 0; index < GSA_BATTERIE_CELL_COUNT; index++) {
        batterieVoltage += this->cellVoltages[index];
    }

    return batterieVoltage;
};

float GsaBatterieMonitor::getAlertCell()
{
    return this->alertCell;
};

float GsaBatterieMonitor::getAlertBatterie()
{
    return this->alertBatterie;
};

#endif