/**
 * Example T03 — GetTimeUs
 *
 * Displays a continuously increasing 32-bit microsecond counter.
 */

#include <AGV_Core.h>
using namespace AGV_Core::Time;

void setup() {
    Serial.begin(115200);
    Serial.println(F("=== Example T03: GetTimeUs ==="));
}

void loop() {
    uint32_t now = GetTimeUs();

    Serial.print(F("Microsecond time: "));
    Serial.println(now);

    delay(200);
}
