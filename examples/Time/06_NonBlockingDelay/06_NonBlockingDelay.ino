/**
 * Example T06 — Non-Blocking Delay (Microsecond Scheduler)
 *
 * Demonstrates how to implement a non-blocking timer using
 * GetTimeUs() + DeltaTicks().
 */

#include <AGV_Core.h>
using namespace AGV_Core::Time;

uint32_t lastEvent = 0;
const uint32_t interval = 250000; // 250 ms in microseconds

void setup() {
    Serial.begin(115200);
    Serial.println(F("=== Example T06: Non-Blocking Delay ==="));

    lastEvent = GetTimeUs();
}

void loop() {
    uint32_t now = GetTimeUs();

    // Non-blocking check
    if (DeltaTicks(now, lastEvent) >= interval) {

        lastEvent = now; // Reset timer safely

        Serial.print(F("Event triggered at "));
        Serial.print(now);
        Serial.println(F(" us"));
    }

    // Other code can run here — never blocked
}
