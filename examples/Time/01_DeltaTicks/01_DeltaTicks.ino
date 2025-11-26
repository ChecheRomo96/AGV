/**
 * Example T01 — DeltaTicks
 *
 * Demonstrates how to measure elapsed time between two tick values,
 * even when the 32-bit microsecond counter overflows.
 */

#include <AGV_Core.h>
using namespace AGV_Core::Time;

void setup() {
    Serial.begin(115200);
    Serial.println(F("=== Example T01: DeltaTicks ==="));

    uint32_t t0 = 0xFFFFFF00;   // near overflow
    uint32_t t1 = 0x00000080;   // overflow occurred
    uint32_t dt = DeltaTicks(t1, t0);

    Serial.print(F("t0 = ")); Serial.print(t0); Serial.print(", "); Serial.println(t0, HEX);
    Serial.print(F("t1 = ")); Serial.print(t1); Serial.print(", "); Serial.println(t1, HEX);
    Serial.print(F("DeltaTicks = ")); Serial.println(dt);

    // Expected output ~ 0x180 = 384 ticks
}

void loop() {}
