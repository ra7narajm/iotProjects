#include <Arduino.h>

#include "pin_config.h"
#include "encoder.h"

const int8_t knobDirection[] = { 0, -1, 1, 0, 1, 0, 0, -1, -1, 0, 0, 1, 0, 1, -1, 0 };

void IRAM_ATTR RotaryEncoder::readEncoderISR(void) {
    unsigned long now = millis();
    
    portENTER_CRITICAL_ISR(&(this->encoderMux));
    {
        int8_t newState = (digitalRead(_pinB) << 1 | digitalRead(_pinA));

        if (__encoderState._oldState != newState) {
            __encoderState._position += knobDirection[(newState | (__encoderState._oldState << 2)) & 0x0f];
            __encoderState._oldState = newState;

            __encoderState._positionExt = __encoderState._position >> 2;

            __encoderState._positionExtTimePrev = __encoderState._positionExtTime;
            __encoderState._positionExtTime = now;
        }
    }
    portEXIT_CRITICAL_ISR(&(this->encoderMux));
}

RotaryEncoder::RotaryDirection RotaryEncoder::getDirection(void) {
    RotaryEncoder::RotaryDirection returnDir = RotaryEncoder::NOROTATION;
    if (__encoderState._positionExtPrev > __encoderState._positionExt) {
        returnDir = RotaryEncoder::COUNTERCLOCKWISE;
    } else if (__encoderState._positionExtPrev < __encoderState._positionExt) {
        returnDir = RotaryEncoder::CLOCKWISE;
    }
    __encoderState._positionExtPrev = __encoderState._positionExt;
    return returnDir;
}