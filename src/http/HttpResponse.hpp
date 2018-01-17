#pragma once
#include <Arduino.h>
#include "HttpError.hpp"

class HttpResponse {
public:
    HttpResponse(int code, const String& message);
    HttpResponse(const HttpError& error);
    String getPayload();
    int getStatusCode();
    HttpError getError();
    bool hasError();
    String toString();
private:
    String _payload;
    int _statusCode;
    HttpError _error;
};
