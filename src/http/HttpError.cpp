#include "HttpError.hpp"

HttpError::HttpError(int code, const String& message)
    : _code(code), _message(message) {
}

String
HttpError::getMessage() {
    return _message;
}

int
HttpError::getCode() {
    return _code;
}

String
HttpError::toString() {
    char buffer[512];
    snprintf(buffer, 512, "[%d] %s", _code, _message.c_str());
    return String(buffer);
}
