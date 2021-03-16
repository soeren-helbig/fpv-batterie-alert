#ifndef GSA_BUZZER_CPP_
#define GSA_BUZZER_CPP_

#include "GsaBuzzer.h"

GsaBuzzer::GsaBuzzer(uint8_t port)
{
    this->lastCellAlert = 0;
    this->lastBatterieAlert = 0;
 
    this->port = port;
 
    pinMode(this->port, OUTPUT);
};

void GsaBuzzer::playBootTone()
{
    this->toneLong();
    this->pauseShort();
    this->toneShort();
    this->pauseShort();
    this->toneShort();
    this->pauseLong();
    this->toneLong();
};

void GsaBuzzer::cellAlert()
{
    unsigned long now = millis();

    if (this->lastCellAlert != 0 && this->lastCellAlert + GSA_BUZZER_ALERT_TIMEOUT_CELL > now) {
        return;
    }

    this->toneShort();
    this->pauseShort();

    this->toneShort();
    this->pauseShort();

    this->toneShort();
    this->pauseShort();

    this->toneShort();

    this->lastCellAlert = now;
};

void GsaBuzzer::betterieAlert()
{
    unsigned long now = millis();

    if (this->lastBatterieAlert != 0 && this->lastBatterieAlert + GSA_BUZZER_ALERT_TIMEOUT_BATTERIE > now) {
        return;
    }

    this->toneLong();
    this->pauseLong();

    this->toneLong();

    this->lastBatterieAlert = now;
};

void GsaBuzzer::toneShort()
{
    digitalWrite(this->port, HIGH);
    delay(50);
    digitalWrite(this->port, LOW);
};

void GsaBuzzer::toneLong()
{
    digitalWrite(this->port, HIGH);
    delay(100);
    digitalWrite(this->port, LOW);
};

void GsaBuzzer::pauseShort()
{
    delay(50);
};

void GsaBuzzer::pauseLong()
{
    delay(100);
};

#endif