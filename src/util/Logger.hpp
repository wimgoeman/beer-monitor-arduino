#pragma once
#include <Arduino.h>

#ifdef LOGGING
    #define INIT_LOGGING(BAUD_RATE) Serial.begin(BAUD_RATE); delay(1000);
#else
    #define INIT_LOGGING(BAUD_RATE)
#endif

#ifdef LOGGING
    //For sure this mess can be done in a cleaner way, but this worked when a man was tired
    #define LOGF(LENGTH, FORMAT, ...) { \
        char buf[LENGTH]; \
        String format; \
        format = format + __FILE__ + ":" + __LINE__ + " - " + FORMAT; \
        snprintf(buf, LENGTH, \
            format.c_str(), \
            __VA_ARGS__); \
        Serial.println(buf); \
    }
#else
    #define LOGF(...)
#endif

#ifdef LOGGING
    #define LOG(CHAR) { \
        String out; \
        out = out + __FILE__ + ":" + __LINE__ + " - " + CHAR; \
        Serial.println(out); \
    }
#else
    #define LOG(TEXT)
#endif
