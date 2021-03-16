#ifndef GSA_BUZZER_H_
#define GSA_BUZZER_H_

#include <Arduino.h>

#define GSA_BUZZER_ALERT_TIMEOUT_BATTERIE 5000
#define GSA_BUZZER_ALERT_TIMEOUT_CELL 10000

class GsaBuzzer
{
  private:
    uint8_t port;

    unsigned long lastCellAlert;
    unsigned long lastBatterieAlert;

  public:
    GsaBuzzer(uint8_t port);

    void playBootTone();

    void cellAlert();
    void betterieAlert();

  private:
    void toneShort();
    void toneLong();
    void pauseShort();
    void pauseLong();
};

#endif