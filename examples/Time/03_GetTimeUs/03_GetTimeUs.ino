/**
 * Example T04 — GetTimeMs
 *
 * Shows how to read the system millisecond counter using GetTimeMs().
 */

#include <AGV_Core.h>
using namespace AGV_Core::Time;

void setup() {
    Serial.begin(115200);
    Serial.println(F("=== Example T04: GetTimeMs ==="));
}

void loop() {
    uint32_t now = GetTimeMs();

    Serial.print(F("Millisecond time: "));
    Serial.println(now);

    delay(500);
}
