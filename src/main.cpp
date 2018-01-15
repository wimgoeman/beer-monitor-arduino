#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <SHA256.h>
#include "constants.hpp"

ESP8266WiFiMulti WiFiMulti;

#define LED_RED 0
#define LED_BLUE 2

String calcToken(String timestamp);

void setup() {
    Serial.begin(115200);

    //Register LEDs
    pinMode(LED_BLUE, OUTPUT);
    pinMode(LED_RED, OUTPUT);

    //Initiate WiFi connectivity
    Serial.println("Init started");
    digitalWrite(LED_BLUE, LOW); //Blue led shows wifi not connected
    WiFi.mode(WIFI_STA);
    WiFiMulti.addAP(BM_WIFI_SSID, BM_WIFI_PASSPHRASE);
    delay(100);
    digitalWrite(LED_RED, HIGH);
    Serial.println("Init completed");
}

void loop() {
    Serial.println("Loop!");
    if (WiFiMulti.run() != WL_CONNECTED) {
        Serial.println("Wifi not connected!!");
        digitalWrite(LED_BLUE, LOW);
        delay(1000);
        return;
    }
    Serial.println("WiFi is connected");
    digitalWrite(LED_BLUE, HIGH);

    HTTPClient http;
    Serial.println("Get server timestamp");
    http.begin("http://192.168.0.248:8090/api/time");
    int httpCode = http.GET();
    String timestamp;
    if (httpCode == 200) {
        Serial.printf("HTTP request succesfull: %d\n", httpCode);
        timestamp = http.getString();
    } else {
        digitalWrite(LED_RED, LOW); //Indicate error
        if (httpCode < 0)
            Serial.println("HTTP request failed: " + http.errorToString(httpCode));
        else
            Serial.printf("HTTP request failed (%d): %s\n", httpCode, http.getString().c_str());
        delay(10000);
        return;
    }

    Serial.println("Attempting HTTP request");
    http.begin("http://192.168.0.248:8090/api/readings");
    http.addHeader("Content-Type", "application/json");
    char jsonPayloadFmt[512];
    snprintf(jsonPayloadFmt, 512, "{\"temperature\": %2.1f, \"humidity\": %d, \"token\": \"%s\"}",
        (float)random(160,240) / 10,
        random(30,65),
        calcToken(timestamp).c_str()
        );
    String jsonPayload(jsonPayloadFmt);
    // String jsonPayload = "{\"temperature\": 20.5, \"humidity\": 43, \"token\": \""
    //     +  + "\"}";
    Serial.println("Payload: " + jsonPayload);

    httpCode = http.POST(jsonPayload);
    if (httpCode == 200) {
        Serial.printf("HTTP request succesfull: %d\n", httpCode);
        Serial.println("Response payload: " + http.getString());
    } else {
        digitalWrite(LED_RED, LOW); //Indicate error
        if (httpCode < 0)
            Serial.println("HTTP request failed: " + http.errorToString(httpCode));
        else
            Serial.printf("HTTP request failed (%d): %s\n", httpCode, http.getString().c_str());
        delay(10000);
        return;
    }

    digitalWrite(LED_RED, HIGH); //Indicate no error
    delay(60000); // Only update once per minute
}

String calcToken(String timestamp) {
    Serial.println("Using timestamp " + timestamp);
    SHA256 sha256;
    sha256.resetHMAC(BM_SECRET, BM_SECRET_LENGTH);
    sha256.update(timestamp.c_str(), timestamp.length());
    char buffer[32];
    sha256.finalizeHMAC(BM_SECRET, BM_SECRET_LENGTH, buffer, 32);
    char result[65];
    result[64] = 0;
    for (int i = 0; i <  32; i++) {
        sprintf(&result[2*i], "%02x", buffer[i]);
    }

    return String(result);
}
