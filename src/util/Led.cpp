#include "Led.hpp"
#include <Arduino.h>

Led::Led(uint8_t pin) {
    _pin = pin;
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
}

void
Led::setLit(bool lit) {
    if (lit == _lit)
        return;
    else if (!_lit) {
        //LOW == LED on
        digitalWrite(_pin, LOW);
        _lit = true;
    } else {
        //HIGH == LED off
        digitalWrite(_pin, HIGH);
    }
}
