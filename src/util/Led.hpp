#pragma once
#include <Arduino.h>

class Led {
public:
    Led(uint8_t pin);

    void
    setLit(bool);

private:
    uint8_t _pin;
    bool _lit = false;

};
