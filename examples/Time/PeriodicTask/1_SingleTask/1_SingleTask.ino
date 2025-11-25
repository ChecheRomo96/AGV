#include <AGV_Core.h>
using namespace AGV_Core::Time;

PeriodicTask task;

void PrintMessage(PeriodicTask& t, uint32_t now) {
  Serial.print(F("→ Tarea ejecutada: "));
  Serial.print(t.TicksToSeconds(now));
  Serial.println(F(" s"));
}

void setup() {
  Serial.begin(115200);

  task.SetFs(5.0f);         // 5 Hz
  task.SetTickFrequency(1e6); // microsegundos
}

void loop() {
  uint32_t now = AGV_Core::Time::GetTimeUs();

  if (task.Update(now) == PeriodicTask::UpdateStatus::Update) {
    PrintMessage(task, now);
  }
}
