#include <AGV_Core.h>
using namespace AGV_Core::Time;

class Blinker : public PeriodicTask {
public:
  Blinker(uint8_t pin) : _pin(pin) {}

  void Init() {
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, LOW);

    SetTickFrequency(1000000.0f);
    SetFs(2.0f);  // 2 Hz toggle
  }

  void UpdateBlink(uint32_t now) {
    if (Update(now) == UpdateStatus::Update) {
      digitalWrite(_pin, !digitalRead(_pin));
      Serial.print(F("Blinker "));
      Serial.print(F("→ Tarea ejecutada: "));
      Serial.print(TicksToSeconds(now), 2);
      Serial.println(F(" s"));
    }
  }

private:
  uint8_t _pin;
};

Blinker blinker(13);

void setup() {
  Serial.begin(115200);
  blinker.Init();
  Serial.println(F("=== DerivedTaskClass ==="));
}

void loop() {
  uint32_t now = AGV_Core::Time::GetTimeUs();
  blinker.UpdateBlink(now);
}
