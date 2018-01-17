#include "HttpWrapper.hpp"
#include "../util/Logger.hpp"

HttpResponse
HttpWrapper::post(const String& url, const String& payload) {
    LOGF(128, "HTTP: Posting to %s", url.c_str());
    _http.begin(url);
    //Would be better to have an HttpRequest object to set the headers on
    _http.addHeader("Content-Type", "application/json");
    int code = _http.POST(payload);
    return processResult(code);
}

HttpResponse
HttpWrapper::get(const String& url) {
    LOGF(128, "HTTP: Getting %s", url.c_str());
    _http.begin(url);
    int code = _http.GET();
    return processResult(code);
}

HttpResponse
HttpWrapper::processResult(int code) {
    String result = _http.getString();
    if (code < 0) {
        HttpError error = HttpError(code * -1, _http.errorToString(code));
        LOGF(512, "HTTP error occured: %s", error.toString().c_str());
        return HttpResponse(error);
    } else {
        return HttpResponse(code, result);
    }
}
