/**
 * Example T05 — Precision Stopwatch
 *
 * Demonstrates how to measure execution time of any operation
 * using the GetTimeUs() timer and DeltaTicks().
 */

#include <AGV_Core.h>
using namespace AGV_Core::Time;

// Example function to profile
void HeavyComputation() {
    volatile float sum = 0.0f;
    for (int i = 0; i < 5000; i++) {
        sum += i * 1.23456f;
    }
}

void setup() {
    Serial.begin(115200);
    Serial.println(F("=== Example T05: Precision Stopwatch ==="));
}

void loop() {
    uint32_t t0 = GetTimeUs();   // Start timestamp
    HeavyComputation();
    uint32_t t1 = GetTimeUs();   // End timestamp

    uint32_t dt = DeltaTicks(t1, t0); // Overflow-safe difference

    Serial.print(F("HeavyComputation() took "));
    Serial.print(dt);
    Serial.println(F(" us"));

    delay(1000);
}
