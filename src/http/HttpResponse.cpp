#include "HttpResponse.hpp"

HttpResponse::HttpResponse(int code, const String& payload)
    : _statusCode(code), _payload(payload), _error(HttpError(0, "No error")){
}

HttpResponse::HttpResponse(const HttpError& error)
    : _statusCode(0), _payload(""), _error(error){
}

String
HttpResponse::getPayload() {
    return _payload;
}

int
HttpResponse::getStatusCode() {
    return _statusCode;
}

HttpError
HttpResponse::getError() {
    return _error;
}

bool
HttpResponse::hasError() {
    return _error.getCode() > 0;
}

String
HttpResponse::toString() {
    char buffer[512];
    snprintf(buffer, 512, "[%d] %s", _statusCode, _payload.c_str());
    return String(buffer);
}
