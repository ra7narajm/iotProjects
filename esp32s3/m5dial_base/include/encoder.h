#ifndef __ROTARY_ENCODER_H_
#define __ROTARY_ENCODER_H_
#include <Arduino.h>

//based on https://github.com/igorantolic/ai-esp32-rotary-encoder/
// and https://github.com/m5stack/M5Dial/blob/master/src/utility/Encoder.h
/*
    pinB and pinA rotation sequence,
    [0 0, 0 1, 1 1, 1 0, 0 0] --- fwd
    state table
    old B   old A   B       A       Decimal     Direction
    0       0       0       1       1           Fwd
    0       1       1       1       7
    1       1       1       0       14
    1       0       0       0       8

    0       0       0       0       0           No Rotation
    0       1       0       1       5
    1       1       1       1       15
    1       0       1       0       10

    0       0       1       0       2           Reverse
    1       0       1       1       11
    1       1       0       1       13
    0       1       0       0       4

                                    3           No Rotation
                                    6
                                    9
                                    12
    knobDirection[]
*/

class RotaryEncoder {
    private:
    //bool isEnabled;
    uint8_t _pinA;
    uint8_t _pinB;

    portMUX_TYPE encoderMux = portMUX_INITIALIZER_UNLOCKED;

    void (*isrCallback)();      //ISR for read-encoder

    struct {
        //encoder state
        volatile int8_t _oldState;

        volatile long _position;         // Internal position (4 times _positionExt)
        volatile long _positionExt;      // External position
        volatile long _positionExtPrev;  // External position (used only for direction checking)

        unsigned long _positionExtTime;      // The time the last position change was detected.
        unsigned long _positionExtTimePrev;  // The time the previous position change was detected.
    } __encoderState;

    public:
    enum RotaryDirection {
        NOROTATION = 0,
        CLOCKWISE = 1,
        COUNTERCLOCKWISE = -1,
    };

    RotaryEncoder(uint8_t pinA, uint8_t pinB, void (*_isrCallback)(void)) : _pinA(pinA), _pinB(pinB), isrCallback(_isrCallback) {
        pinMode(_pinA, INPUT_PULLUP);
        pinMode(_pinB, INPUT_PULLUP);

        delay(100);     //settle down

        __encoderState._oldState = (digitalRead(_pinB) << 1 | digitalRead(_pinA));

        attachInterrupt(digitalPinToInterrupt(_pinA), isrCallback, CHANGE);
        attachInterrupt(digitalPinToInterrupt(_pinB), isrCallback, CHANGE);
    }

    RotaryEncoder(uint8_t pinA, uint8_t pinB) : _pinA(pinA), _pinB(pinB) {
        pinMode(_pinA, INPUT_PULLUP);
        pinMode(_pinB, INPUT_PULLUP);

        delay(100);     //settle down

        __encoderState._oldState = (digitalRead(_pinB) << 1 | digitalRead(_pinA));
    }

    void setupISR(void (*isrCallback)(void)) {
        attachInterrupt(digitalPinToInterrupt(_pinA), isrCallback, CHANGE);
        attachInterrupt(digitalPinToInterrupt(_pinB), isrCallback, CHANGE);
    }

    void IRAM_ATTR readEncoderISR(void);

    RotaryDirection getDirection(void);
    long getPosition(void) {
        return __encoderState._position;
    }
};
#endif //__ROTARY_ENCODER_H_