#pragma once
#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include "HttpResponse.hpp"

class HttpWrapper {
public:
    HttpResponse post(const String& url, const String& payload);
    HttpResponse get(const String& url);
private:
    HttpResponse processResult(int code);
    HTTPClient _http;
};
