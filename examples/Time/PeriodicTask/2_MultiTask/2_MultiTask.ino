#include <AGV_Core.h>
using namespace AGV_Core::Time;

PeriodicTask fastTask;  // 10 Hz
PeriodicTask slowTask;  // 1 Hz

void PrintMessage(PeriodicTask& t, uint32_t now) {
  Serial.print(F("→ Tarea ejecutada: "));
  Serial.print(t.TicksToSeconds(now));
  Serial.println(F(" s"));
}

void setup() {
  Serial.begin(115200);

  fastTask.SetTickFrequency(1000000.0f);
  slowTask.SetTickFrequency(1000000.0f);

  fastTask.SetFs(10.0f);   // cada 100 ms
  slowTask.SetFs(1.0f);    // cada 1 segundo

  Serial.println(F("=== TwoTasks_DifferentRates ==="));
}

void loop() {
  uint32_t now = AGV_Core::Time::GetTimeUs();

  if (fastTask.Update(now) == PeriodicTask::UpdateStatus::NewMeasurement) {
    Serial.print(F("Fast Task "));
    PrintMessage(fastTask,now);
  }

  if (slowTask.Update(now) == PeriodicTask::UpdateStatus::NewMeasurement) {
    Serial.print(F("Slow Task "));
    PrintMessage(slowTask,now);
  }
}
