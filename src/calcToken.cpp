#include <Arduino.h>
#include <SHA256.h>
#include "constants.hpp"

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
