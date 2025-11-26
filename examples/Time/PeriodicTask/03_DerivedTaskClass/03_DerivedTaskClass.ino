/**
 * Example 03 — DerivedTask (Overriding OnUpdate)
 *
 * This example demonstrates how to create a class that inherits from PeriodicTask
 * and overrides the OnUpdate() callback. The PeriodicTask scheduler automatically
 * invokes OnUpdate() at the configured sampling frequency.
 *
 * The Blinker class toggles a digital pin every 500 ms (2 Hz).
 */

#include <AGV_Core.h>
using namespace AGV_Core::Time;

class Blinker : public PeriodicTask {
public:

  explicit Blinker(uint8_t pin)
    : _pin(pin) {}

  /** Configure hardware and scheduling parameters */
  void Init() {
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, LOW);

    SetTickFrequency(1e6);  // 1 tick = 1 microsecond
    SetFs(2.0f);            // 2 Hz → toggle every 0.5 seconds
  }

protected:

  /** Called automatically when Update() determines the task must run */
  void OnUpdate(uint32_t nowTicks) noexcept override {
    digitalWrite(_pin, !digitalRead(_pin));

    Serial.print(F("Blinker → Executed at "));
    Serial.print(TicksToSeconds(nowTicks), 3);
    Serial.println(F(" s"));
  }

private:
  uint8_t _pin;
};

// Instance
Blinker blinker(13);

void setup() {
  Serial.begin(115200);
  blinker.Init();

  Serial.println(F("=== Example 03: DerivedTask (OnUpdate Override) ==="));
}

void loop() {
  uint32_t now = AGV_Core::Time::GetTimeUs();
  blinker.Update(now);  // OnUpdate() is called automatically
}
