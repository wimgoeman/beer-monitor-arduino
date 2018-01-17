#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include "util/Logger.hpp"
#include "util/Led.hpp"
#include "http/HttpWrapper.hpp"
#include "constants.hpp"
#include "calcToken.hpp"

struct Leds {
    Led red = Led(0);
    Led blue = Led(2);
};

Leds leds;
ESP8266WiFiMulti WiFiMulti;

void setup() {
    INIT_LOGGING(115200);
    //Initiate WiFi connectivity
    LOG("Init started");
    leds.red.setLit(false); //Red LED indicates error state
    leds.blue.setLit(true); //Blue LED shows wifi not connected
    WiFi.mode(WIFI_STA);
    WiFiMulti.addAP(BM_WIFI_SSID, BM_WIFI_PASSPHRASE);
    delay(1000);
    LOG("Init completed");
}

bool checkWifi() {
    if (WiFiMulti.run() != WL_CONNECTED) {
        LOG("Wifi not connected!!");
        leds.blue.setLit(true);
        delay(1000);
        return false;
    }
    LOG("WiFi is connected");
    leds.blue.setLit(false);
    return true;
}

bool fetchTimestamp(HttpWrapper& http, String& timestamp) {
    HttpResponse res = http.get("http://192.168.0.248:8090/api/time");
    if (res.hasError() || res.getStatusCode() != 200) {
        if (!res.hasError()) {
            LOGF(256, "Error while getting time: [%d] %s", res.getStatusCode(), res.getPayload().c_str());
        }
        return false;
    }
    timestamp = res.getPayload();
    return true;
}

bool uploadReadings(HttpWrapper& http, const String& timestamp) {
    char jsonPayloadFmt[512];
    snprintf(jsonPayloadFmt, 512, "{\"temperature\": %2.1f, \"humidity\": %d, \"token\": \"%s\"}",
        (float)random(160,240) / 10,
        random(30,65),
        calcToken(timestamp).c_str()
        );
    String jsonPayload(jsonPayloadFmt);
    LOG("JSON payload: " + jsonPayload);
    HttpResponse res = http.post("http://192.168.0.248:8090/api/readings", jsonPayload);
    if (res.hasError() || res.getStatusCode() != 200) {
        if (!res.hasError()) {
            LOGF(256, "Error while uploading readings: [%d] %s", res.getStatusCode(), res.getPayload().c_str());
        }
        return false;
    }
    LOG("Readings uploaded!");
    return true;
}

void handleError() {
    //Light red LED for error
    leds.red.setLit(true);

    //Wait 10s before retrying
    delay(10000);
}

void loop() {
    LOG("Loop!");
    if (!checkWifi()) return;

    HttpWrapper http;
    String timestamp;
    if (!fetchTimestamp(http, timestamp)) {
        handleError();
        return;
    }
    if (!uploadReadings(http, timestamp)) {
        handleError();
        return;
    }

    //Dim error LED if it was still on
    leds.red.setLit(false);
    
    // Only update once per minute
    delay(60000);
}
