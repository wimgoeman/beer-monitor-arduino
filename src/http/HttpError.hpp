#pragma once
#include <Arduino.h>

class HttpError {
public:
    HttpError(int code, const String& message);
    String getMessage();
    int getCode();
    String toString();
private:
    String _message;
    int _code;
};
